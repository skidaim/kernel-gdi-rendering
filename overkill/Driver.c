#include "utils.h"
#define PROCESS_NAME_MAX_LEN 260

#define PROCESS_VM_OPERATION   0x0008
#define PROCESS_VM_READ        0x0010
#define PROCESS_VM_WRITE       0x0020

#define PDB_FILE_TAG 'bPdb' // Tag for PDB file buffer
#define PE_FILE_TAG  'fPeD' // Tag for PE file buffer
#define DIR_BUF_TAG  'drsP' // Tag for Directory buffer
#define DBI_BUF_TAG  'ibDP' // Tag for DBI stream buffer
#define SYM_BUF_TAG  'mySP' // Tag for Symbol stream buffer
uintptr_t baseAddress = NULL;





#include "gamestuff.h"


BOOLEAN render = TRUE;


NTSTATUS ReadFileToBuffer(_In_ PUNICODE_STRING FilePath, _Out_ PKERNEL_BUFFER OutputBuffer)
{
	OBJECT_ATTRIBUTES objAttributes = { 0 };
	HANDLE hFile = NULL;
	NTSTATUS status;
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	FILE_STANDARD_INFORMATION fileInfo = { 0 };

	if (!FilePath || !OutputBuffer)
	{
		return STATUS_INVALID_PARAMETER;
	}

	if (KeGetCurrentIrql() != PASSIVE_LEVEL)
	{
		return STATUS_INVALID_LEVEL;
	}

	InitializeObjectAttributes(&objAttributes, FilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	status = ZwCreateFile(&hFile,
		GENERIC_READ,
		&objAttributes,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: ZwCreateFile failed: 0x%X\n", status);
		return status;
	}

	status = ZwQueryInformationFile(hFile, &ioStatusBlock, &fileInfo, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: ZwQueryInformationFile failed: 0x%X\n", status);
		ZwClose(hFile);
		return status;
	}

	OutputBuffer->Size = fileInfo.EndOfFile.LowPart;
	OutputBuffer->Buffer = ExAllocatePoolWithTag(NonPagedPool, OutputBuffer->Size, 'bPdb');

	if (!OutputBuffer->Buffer)
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: Failed to allocate buffer for PDB file\n");
		ZwClose(hFile);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	status = ZwReadFile(hFile, NULL, NULL, NULL, &ioStatusBlock, OutputBuffer->Buffer, (ULONG)OutputBuffer->Size, NULL, NULL);
	ZwClose(hFile);

	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: ZwReadFile failed: 0x%X\n", status);
		ExFreePoolWithTag(OutputBuffer->Buffer, 'bPdb');
		OutputBuffer->Buffer = NULL;
		OutputBuffer->Size = 0;
	}

	return status;
}

NTSTATUS GetPdbSymbolStream(_In_ PVOID PdbBase, _Out_ PKERNEL_BUFFER SymbolStream)
{
	struct SuperBlock* superBlock = (struct SuperBlock*)PdbBase;

	if (memcmp(superBlock->FileMagic, PDB_MAGIC, sizeof(PDB_MAGIC)) != 0)
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: Invalid PDB magic signature.\n");
		return STATUS_INVALID_IMAGE_FORMAT;
	}

	const UINT32 blockSize = superBlock->BlockSize;
	const UINT32 numDirectoryBytes = superBlock->NumDirectoryBytes;
	const UINT32 numDirectoryBlocks = (numDirectoryBytes + blockSize - 1) / blockSize;

	UINT32* directoryBlockMap = (UINT32*)((UINT8*)PdbBase + superBlock->BlockMapAddr * blockSize);

	KERNEL_BUFFER directoryBuffer = { 0 };
	directoryBuffer.Size = numDirectoryBytes;
	directoryBuffer.Buffer = ExAllocatePoolWithTag(NonPagedPool, directoryBuffer.Size, 'drsP');
	if (!directoryBuffer.Buffer)
	{
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	UINT8* current_dir_pos = (UINT8*)directoryBuffer.Buffer;
	SIZE_T bytes_to_copy;
	for (UINT32 i = 0; i < numDirectoryBlocks; ++i)
	{
		UINT8* block_start = (UINT8*)PdbBase + directoryBlockMap[i] * blockSize;
		bytes_to_copy = min(blockSize, directoryBuffer.Size - (i * blockSize));
		memcpy(current_dir_pos, block_start, bytes_to_copy);
		current_dir_pos += bytes_to_copy;
	}

	UINT32* streamData = (UINT32*)directoryBuffer.Buffer;
	const UINT32 numStreams = *streamData++;
	const UINT32* streamSizes = streamData;
	UINT32* streamBlockIndices = (UINT32*)(streamSizes + numStreams);

	if (numStreams < 4)
	{
		ExFreePoolWithTag(directoryBuffer.Buffer, 'drsP');
		return STATUS_INVALID_IMAGE_FORMAT;
	}

	const UINT32 dbiStreamSize = streamSizes[3];
	UINT32* dbiStreamBlocks = streamBlockIndices;
	for (int i = 0; i < 3; i++)
	{
		dbiStreamBlocks += (streamSizes[i] + blockSize - 1) / blockSize;
	}

	KERNEL_BUFFER dbiStreamBuffer = { 0 };
	dbiStreamBuffer.Size = dbiStreamSize;
	dbiStreamBuffer.Buffer = ExAllocatePoolWithTag(NonPagedPool, dbiStreamBuffer.Size, 'ibDP');
	if (!dbiStreamBuffer.Buffer)
	{
		ExFreePoolWithTag(directoryBuffer.Buffer, 'drsP');
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	UINT8* current_dbi_pos = (UINT8*)dbiStreamBuffer.Buffer;
	const UINT32 numDbiBlocks = (dbiStreamSize + blockSize - 1) / blockSize;
	for (UINT32 i = 0; i < numDbiBlocks; ++i)
	{
		UINT8* block_start = (UINT8*)PdbBase + dbiStreamBlocks[i] * blockSize;
		bytes_to_copy = min(blockSize, dbiStreamBuffer.Size - (i * blockSize));
		memcpy(current_dbi_pos, block_start, bytes_to_copy);
		current_dbi_pos += bytes_to_copy;
	}

	struct DBIHeader* dbiHeader = (struct DBIHeader*)dbiStreamBuffer.Buffer;
	const UINT16 symbolRecordStreamIndex = dbiHeader->SymRecordStream;
	ExFreePoolWithTag(dbiStreamBuffer.Buffer, 'ibDP');

	if (symbolRecordStreamIndex >= numStreams)
	{
		ExFreePoolWithTag(directoryBuffer.Buffer, 'drsP');
		return STATUS_INVALID_IMAGE_FORMAT;
	}

	const UINT32 symbolStreamSize = streamSizes[symbolRecordStreamIndex];
	UINT32* symbolStreamBlocks = streamBlockIndices;
	for (int i = 0; i < symbolRecordStreamIndex; i++)
	{
		symbolStreamBlocks += (streamSizes[i] + blockSize - 1) / blockSize;
	}

	SymbolStream->Size = symbolStreamSize;
	SymbolStream->Buffer = ExAllocatePoolWithTag(NonPagedPool, SymbolStream->Size, 'mySP');
	if (!SymbolStream->Buffer)
	{
		ExFreePoolWithTag(directoryBuffer.Buffer, 'drsP');
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	UINT8* current_sym_pos = (UINT8*)SymbolStream->Buffer;
	const UINT32 numSymbolBlocks = (symbolStreamSize + blockSize - 1) / blockSize;
	for (UINT32 i = 0; i < numSymbolBlocks; ++i)
	{
		UINT8* block_start = (UINT8*)PdbBase + symbolStreamBlocks[i] * blockSize;
		bytes_to_copy = min(blockSize, SymbolStream->Size - (i * blockSize));
		memcpy(current_sym_pos, block_start, bytes_to_copy);
		current_sym_pos += bytes_to_copy;
	}

	ExFreePoolWithTag(directoryBuffer.Buffer, 'drsP');
	return STATUS_SUCCESS;
}


NTSTATUS FindSymbolRva(_In_ PKERNEL_BUFFER SymbolStream, _In_ PVOID PeFileBase, _In_ PCSTR SymbolName, _Out_ PULONG64 FinalRva)
{
	if (!SymbolStream || !SymbolStream->Buffer || !PeFileBase || !SymbolName || !FinalRva)
	{
		return STATUS_INVALID_PARAMETER;
	}

	// get the section headers from the PE file
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)PeFileBase;
	if (pDosHeader->e_magic != 'ZM') // "MZ"
	{
		return STATUS_INVALID_IMAGE_FORMAT;
	}

	PIMAGE_NT_HEADERS64 pNtHeaders = (PIMAGE_NT_HEADERS64)((UINT8*)PeFileBase + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != 'EP') // "PE\0\0"
	{
		return STATUS_INVALID_IMAGE_FORMAT;
	}

	PIMAGE_SECTION_HEADER pSectionHeaders = (PIMAGE_SECTION_HEADER)((UINT8*)&pNtHeaders->OptionalHeader + pNtHeaders->FileHeader.SizeOfOptionalHeader);

	// iterate through the PDB symbols
	UINT8* it = (UINT8*)SymbolStream->Buffer;
	const UINT8* end = it + SymbolStream->Size;

	while (it < end)
	{
		struct PUBSYM32* current = (struct PUBSYM32*)it;
		if (it + sizeof(UINT16) * 2 > end || it + current->reclen + 2 > end)
		{
			break; // no
		}

		if (current->rectyp == S_PUB32)
		{
			if (strcmp(current->name, SymbolName) == 0)
			{
				UINT16 sectionIndex = current->seg;

				if (sectionIndex > 0 && sectionIndex <= pNtHeaders->FileHeader.NumberOfSections)
				{
					// look up the section header
					PIMAGE_SECTION_HEADER symbolSection = &pSectionHeaders[sectionIndex - 1];

					// calculate the final RVA
					*FinalRva = symbolSection->VirtualAddress + current->off;

					return STATUS_SUCCESS;
				}
			}
		}
		it += current->reclen + 2;
	}

	return STATUS_NOT_FOUND;
}

#include <ntifs.h>

NTSTATUS WriteU32ToProcess(_In_ PEPROCESS Process, _In_ PVOID UserAddress, _In_ ULONG Value)
{
	SIZE_T bytes = 0;
	return MmCopyVirtualMemory(
		PsGetCurrentProcess(), // from current (kernel pseudo process)
		&Value,                // source
		Process,               // target process
		UserAddress,           // destination
		sizeof(Value),
		KernelMode,
		&bytes
	);
}

// Safe writer using protection flip + MmCopyVirtualMemory
static NTSTATUS WriteCodeIntoProcess(
	_In_ PEPROCESS Process,
	_In_ PVOID     Dst,
	_In_reads_bytes_(Size) const void* Src,
	_In_ SIZE_T    Size,
	_Out_writes_bytes_opt_(Size) void* Original // optional buffer to back up original bytes
)
{
	NTSTATUS status;
	HANDLE hProc = NULL;

	status = ObOpenObjectByPointer(
		Process, OBJ_KERNEL_HANDLE, NULL,
		PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
		*PsProcessType, KernelMode, &hProc);
	if (!NT_SUCCESS(status)) return status;


	// Backup original bytes (optional)
	if (Original) {
		SIZE_T got = 0;
		status = MmCopyVirtualMemory(Process, Dst, PsGetCurrentProcess(),
			Original, Size, KernelMode, &got);
		if (!NT_SUCCESS(status) || got != Size) { ZwClose(hProc); return status ? status : STATUS_PARTIAL_COPY; }
	}

	// Make RXW
	PVOID  protectBase = PAGE_ALIGN(Dst);
	SIZE_T protectSize = ADDRESS_AND_SIZE_TO_SPAN_PAGES(Dst, Size) * PAGE_SIZE;
	ULONG  oldProt = 0, tmpProt = 0;

	status = ZwProtectVirtualMemory(hProc, &protectBase, &protectSize,
		PAGE_EXECUTE_READWRITE, &oldProt);
	if (!NT_SUCCESS(status)) { ZwClose(hProc); return status; }

	// Write bytes
	SIZE_T wrote = 0;
	status = MmCopyVirtualMemory(PsGetCurrentProcess(), (PVOID)Src, Process, Dst, Size, KernelMode, &wrote);

	// Restore protection
	(void)ZwProtectVirtualMemory(hProc, &protectBase, &protectSize, oldProt, &tmpProt);

	if (!NT_SUCCESS(status) || wrote != Size) { ZwClose(hProc); return status ? status : STATUS_PARTIAL_COPY; }

	// Flush I-cache
	(void)ZwFlushInstructionCache(hProc, Dst, Size);
	ZwClose(hProc);
	return STATUS_SUCCESS;
}



uintptr_t PatchDwmOverlayTestMode()
{
	NTSTATUS status;
	KERNEL_BUFFER pdbFileBuffer = { 0 };
	KERNEL_BUFFER peFileBuffer = { 0 };
	KERNEL_BUFFER symbolStreamBuffer = { 0 };
	ULONG64 finalRva = 0;

	uintptr_t dwmcoreBase = NULL;
	PEPROCESS sourceProcess = GetGameProcess(L"dwm.exe");
	
	if (!sourceProcess || PsGetProcessExitStatus(sourceProcess) != STATUS_PENDING) {
		DbgPrintEx(0, 0, "cs2 not found\n");
		return 0x1234;
	}

	

	KAPC_STATE apcState;
	KeStackAttachProcess(sourceProcess, &apcState);

	PPEB pPeb = PsGetProcessPeb(sourceProcess);

	if (pPeb) {
		PPEB_LDR_DATA pLdr = pPeb->Ldr;
		if (pLdr) {
			PLIST_ENTRY pListHead = &pLdr->InMemoryOrderModuleList;
			PLIST_ENTRY pListEntry = pListHead->Flink;

			while (pListEntry != pListHead) {
				PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

				if (pEntry->DllBase) {
					UNICODE_STRING clientdll;
					RtlInitUnicodeString(&clientdll, L"\\dwmcore.dll");
					if (unicodestrstr(&pEntry->FullDllName, &clientdll)) {
						dwmcoreBase = (uintptr_t)pEntry->DllBase;
						break;
					}

				}

				pListEntry = pListEntry->Flink;
			}
		}
		else {
			DbgPrintEx(0, 0, "pLdr NULL\n");
		}
	}
	else {
		DbgPrintEx(0, 0, "pPeb NULL\n");
	}
	KeUnstackDetachProcess(&apcState);

	

	
	if (!dwmcoreBase) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "DWM_Patcher: Failed to get dwmcore.dll base address.\n");
		return 0x273647893;
	}
	
	PCSTR target =
		"?IsCandidateDirectFlipCompatbile@COverlayContext@@AEBA_NPEBVCCompositionSurfaceInfo@@PEAVISwapChainRealization@@AEBUDXGI_MULTIPLANE_OVERLAY_ATTRIBUTES@@W4DXGI_MODE_ROTATION@@I_N4@Z";
	ULONG64 rva = 0;

	UNICODE_STRING pdbPath = RTL_CONSTANT_STRING(L"\\SystemRoot\\System32\\dwmcore.pdb");
	UNICODE_STRING pePath = RTL_CONSTANT_STRING(L"\\SystemRoot\\System32\\dwmcore.dll");

	status = ReadFileToBuffer(&pdbPath, &pdbFileBuffer);
	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: Failed to read PDB file %wZ (0x%X).\n", &pdbPath, status);
		return status;
	}

	status = ReadFileToBuffer(&pePath, &peFileBuffer);
	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: Failed to read PE file %wZ (0x%X).\n", &pePath, status);
		ExFreePoolWithTag(pdbFileBuffer.Buffer, 'bPdb');
		return status;
	}

	// parse PDB to get the symbol stream.
	status = GetPdbSymbolStream(pdbFileBuffer.Buffer, &symbolStreamBuffer);
	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PDBParser: Failed to get symbol stream (0x%X).\n", status);
		ExFreePoolWithTag(pdbFileBuffer.Buffer, 'bPdb');
		ExFreePoolWithTag(peFileBuffer.Buffer, 'ePsP');
		return status;
	}

	ExFreePoolWithTag(pdbFileBuffer.Buffer, 'bPdb');

	// search for the symbol, calculating the correct RVA using the PE file data.
	PCSTR targetSymbol = "?IsCandidateDirectFlipCompatbile@COverlayContext@@AEBA_NPEBVCCompositionSurfaceInfo@@PEAVISwapChainRealization@@AEBUDXGI_MULTIPLANE_OVERLAY_ATTRIBUTES@@W4DXGI_MODE_ROTATION@@I_N4@Z";
	status = FindSymbolRva(&symbolStreamBuffer, peFileBuffer.Buffer, targetSymbol, &finalRva);

	uintptr_t in_memory_address = dwmcoreBase + finalRva;



	// open process handle with PROCESS_VM_OPERATION|READ|WRITE (OBJ_KERNEL_HANDLE)
	PVOID base = PAGE_ALIGN(in_memory_address);
	SIZE_T size = ADDRESS_AND_SIZE_TO_SPAN_PAGES(in_memory_address, 3) * PAGE_SIZE;
	ULONG oldProt = 0, tmp = 0;


	HANDLE hProc = NULL;
	status = ObOpenObjectByPointer(
		sourceProcess,               // PEPROCESS you already got from PsLookupProcessByName
		OBJ_KERNEL_HANDLE,           // create handle valid only in kernel
		NULL,
		PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
		*PsProcessType,
		KernelMode,
		&hProc
	);


	NTSTATUS st = ZwProtectVirtualMemory(hProc, &base, &size, PAGE_EXECUTE_READWRITE, &oldProt);
	SIZE_T wrote = 0;
	NTSTATUS st2 = MmCopyVirtualMemory(
		PsGetCurrentProcess(), (PVOID)"\x30\xC0\xC3",
		sourceProcess, in_memory_address, 3, KernelMode, &wrote);

	if (st2 == STATUS_PARTIAL_COPY || wrote != 3) {
		// move pointers forward by 'wrote' and try again (usually after protect flip)
	}

	ZwProtectVirtualMemory(hProc, &base, &size, oldProt, &tmp);

	ZwFlushInstructionCache(hProc, in_memory_address, 3);


	return 0;

}



void main()
{
	//return PatchDwmOverlayTestMode();
	while (TRUE) {
		if (getkey()) {
			render = !render;
		}
		if (!render) {
			continue;
		}
		/*spoofthread();
		if (!getkey()) {
			unspoofthread();
			continue;
		}
		
		drawbox(400 + i, 400 + i, 100, 100, 2);
		i++;
		refresh();
		NtSleep(7);
		unspoofthread();*/
		PEPROCESS sourceProcess = GetGameProcess(L"cs2.exe");

		if (!sourceProcess || PsGetProcessExitStatus(sourceProcess) != STATUS_PENDING) {
			DbgPrintEx(0, 0, "cs2 not found\n");
			continue;
		}

		KAPC_STATE apcState;
		KeStackAttachProcess(sourceProcess, &apcState);

		PPEB pPeb = PsGetProcessPeb(sourceProcess);

		if (pPeb) {
			PPEB_LDR_DATA pLdr = pPeb->Ldr;
			if (pLdr) {
				PLIST_ENTRY pListHead = &pLdr->InMemoryOrderModuleList;
				PLIST_ENTRY pListEntry = pListHead->Flink;

				while (pListEntry != pListHead) {
					PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

					if (pEntry->DllBase) {						
						UNICODE_STRING clientdll;
						RtlInitUnicodeString(&clientdll, L"\\client.dll");
						if (unicodestrstr(&pEntry->FullDllName, &clientdll)) {
							baseAddress = (uintptr_t)pEntry->DllBase;
							break;
						}
						
					}

					pListEntry = pListEntry->Flink;
				}
			}
			else {
				DbgPrintEx(0, 0, "pLdr NULL\n");
			}
		}
		else {
			DbgPrintEx(0, 0, "pPeb NULL\n");
		}
		KeUnstackDetachProcess(&apcState);

		if (!baseAddress) {
			//DbgPrintEx(0, 0, "baseAddress NULL\n");
			continue;

		}
		uintptr_t entitylist = NULL;
		read(baseAddress + 0x1D16758, &entitylist, sizeof(uintptr_t)); //dwEntityList
		if (!entitylist) {
			//DbgPrintEx(0, 0, "entitylist NULL\n");
			continue;

		}
		//uintptr_t localpawn = NULL;
		//read(baseAddress + 0x1BF1FA0, &localpawn, sizeof(uintptr_t));
		//if (!localpawn) {
		//	//DbgPrintEx(0, 0, "localpawn NULL\n");

		//	continue;

		//}
		uintptr_t localPlayer = NULL;
		read(baseAddress + 0x1E1FC08, &localPlayer, sizeof(uintptr_t));
		if (!localPlayer) {
			//DbgPrintEx(0, 0, "localpawn NULL\n");

			continue;

		}

		uintptr_t localTeam = NULL;
		read(localPlayer + 0x3EB, &localTeam, sizeof(uintptr_t));
		if (!localTeam) {
			//DbgPrintEx(0, 0, "currentpawn NULL\n");
			continue;
		}

		uintptr_t listentry = NULL;
		read(entitylist + 16, &listentry, sizeof(uintptr_t));
		if (!listentry) {
			//DbgPrintEx(0, 0, "listentry NULL\n");
			continue;
		}
		spoofthread();
		for (int i = 1; i <= 64; i++) {
			uintptr_t playercontroller = NULL;
			read(listentry + i*0x78, &playercontroller, sizeof(uintptr_t));
			if (!playercontroller) {
				//DbgPrintEx(0, 0, "playercontroller NULL\n");
				continue;
			}
			//2060
			uintptr_t entity;
			read(listentry + 120 * (i & 0x1FF), &playercontroller, sizeof(uintptr_t));
			int playerpawnhandle = 0;
			read(playercontroller + 0x8FC, &playerpawnhandle, sizeof(int)); // m_hPlayerPawn 
			if (!playerpawnhandle) {
				//DbgPrintEx(0, 0, "playerpawnhandle NULL\n");
				continue;
			}
			uintptr_t listentry2 = NULL;
			read(entitylist + 8*((playerpawnhandle & 0x7fff) >>9 ) + 0x10, &listentry2, sizeof(uintptr_t));
			if (!listentry2) {
				//DbgPrintEx(0, 0, "listentry2 NULL\n");
				continue;
			}
			uintptr_t currentpawn = NULL;
			read(listentry2 + 0x78 * (playerpawnhandle & 0x1ff), &currentpawn, sizeof(uintptr_t));
			if (!currentpawn) {
				//DbgPrintEx(0, 0, "currentpawn NULL\n");
				continue;
			}

			
			uintptr_t playerTeam = NULL;
			read(currentpawn + 0x3EB, &playerTeam, sizeof(uintptr_t));
			if (!playerTeam) {
				//DbgPrintEx(0, 0, "currentpawn NULL\n");
				continue;
			}
			
			int playerHealth = NULL;
			read(currentpawn + 0x34C, &playerHealth, sizeof(int));
			if (playerHealth <= 0) {
				continue;
			}
			if (localTeam == playerTeam) {
				continue;
			}
			//4900
			Vector3 playerpos = { 0.f, 0.f, 0.f };
			read(currentpawn + 0x15B8, &playerpos, sizeof(Vector3)); // 0x1324
			//DbgPrintEx(0, 0, "world: %d, %d, %d\n", (int)playerpos.x, (int)playerpos.y, (int)playerpos.z);
			if (playerpos.x == 0) {
				//DbgPrintEx(0, 0, "playerpos NULL\n");
				continue;
			}
			Vector3 playerhead = { playerpos.x, playerpos.y, playerpos.z + 75.f };
			read(baseAddress + 0x1E339D0, &viewm, sizeof(viewmatrix)); //dwViewMatrix 
			int x, y;
			int hx, hy;
			if (!WorldToScreen(playerpos, &x, &y) || !WorldToScreen(playerhead, &hx, &hy)) {
				continue;
			}

			if (x <= 0.0f || y <= 0.0f )
			{
				continue;
			}

			const float height = y - hy;
			const float width = height / 2.4f;
			drawbox((hx - (width / 2.f)), hy, width, height, 2);


		}
		NtSleep(2);
		unspoofthread();
		

	}

	
	PsTerminateSystemThread(STATUS_SUCCESS);
}


uintptr_t DriverEntry() {

	init();
	//return PatchDwmOverlayTestMode();
	PWORK_QUEUE_ITEM WorkItem = (PWORK_QUEUE_ITEM)ExAllocatePool(NonPagedPool, sizeof(WORK_QUEUE_ITEM));
	if (!WorkItem)
	{
		return STATUS_FAILED_DRIVER_ENTRY;
	}
	ExInitializeWorkItem(WorkItem, main, WorkItem);
	ExQueueWorkItem(WorkItem, DelayedWorkQueue);

	return STATUS_SUCCESS;
}