#include "utils.h"

pfnNtUserGetDC NtUserGetDC = NULL;
pfnNtUserReleaseDC NtUserReleaseDC = NULL;
pfnNtGdiPatBlt NtGdiPatBlt = NULL;
pfnGreSelectBrush GreSelectBrush = NULL;
pfnNtGdiCreateSolidBrush NtGdiCreateSolidBrush = NULL;
pfnNtGdiDeleteObjectApp NtGdiDeleteObjectApp = NULL;
pfnNtUserFindWindowEx NtUserFindWindowEx = NULL;
pfnNtGdiExtTextOutW NtGdiExtTextOutW = NULL;
pfnNtUserInvalidateRect NtUserInvalidateRect = NULL;
pfnNtUserGetDCEx NtUserGetDCEx = NULL;
pfnNtUserRedrawWindow NtUserRedrawWindow = NULL;
pfnEngBitBlt mEngBitBlt = NULL;
pfnNtUserGetAsyncKeyState NtUserGetAsyncKeyState = NULL;
void NtSleep(DWORD milliseconds)
{
	unsigned long long ms = milliseconds;
	ms = (ms * 1000) * 10;
	ms = ms * -1;
#ifdef _KERNEL_MODE
	KeDelayExecutionThread(KernelMode, 0, (PLARGE_INTEGER)&ms);
#else
	NtDelayExecution(0, (PLARGE_INTEGER)&ms);
#endif
}
void drawbox(int x, int y, int w, int h, int border) {

	hdc = NtUserGetDCEx(0x0, 0, 1);
	if (!hdc)
	{
		KdPrint(("NtUserGetDC Failed\n"));
		return FALSE;
	}

	brush = NtGdiCreateSolidBrush(RGB(255, 0, 0), NULL);
	if (!brush)
	{
		KdPrint(("NtGdiCreateSolidBrush Failed\n"));
		NtUserReleaseDC(hdc);
		return FALSE;
	}

	HBRUSH oldBrush = GreSelectBrush(hdc, brush);

	NtGdiPatBlt(hdc, x, y, w, border, PATCOPY);
	NtGdiPatBlt(hdc, x, y, border, h, PATCOPY);
	NtGdiPatBlt(hdc, x + w - border, y, border, h, PATCOPY);
	NtGdiPatBlt(hdc, x, y + h - border, w, border, PATCOPY);


	if (oldBrush)
		GreSelectBrush(hdc, oldBrush);

	NtUserReleaseDC(hdc);
	NtGdiDeleteObjectApp(brush);
	
	


	return TRUE;

}

int getkey() {
	spoofthread();
	int ret = NtUserGetAsyncKeyState(0x05) & 1;
	unspoofthread();
	return ret;
	
}


void refresh() {
	NtUserInvalidateRect(0, 0, TRUE);
}

NTSTATUS read(PVOID address, void* buffer, SIZE_T size) {
	SIZE_T bytesRead = 0;

	PEPROCESS Process;
	PsLookupProcessByProcessId(pid, &Process);

	NTSTATUS status = MmCopyVirtualMemory(
		Process,
		address,
		PsGetCurrentProcess(),
		buffer,
		size,
		KernelMode,
		&bytesRead
	);

	if (!NT_SUCCESS(status) || bytesRead != size) {
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}


int init() {
	PVOID win32base = GetModuleBase("win32kbase.sys");
	PVOID win32kfull = GetModuleBase("win32kfull.sys");

	UNICODE_STRING FuncName = RTL_CONSTANT_STRING(L"PsGetThreadProcess");
	PVOID pfnPsGetThreadProcess = MmGetSystemRoutineAddress(&FuncName);
	if (!MmIsAddressValid(pfnPsGetThreadProcess))
		return 0;

	ThreadProcessOffset = *(PULONG)((PUCHAR)pfnPsGetThreadProcess + 3);
	spoofthread();
	NtUserGetDC = (pfnNtUserGetDC)RtlFindExportedRoutineByName(win32base, "NtUserGetDC");
	NtUserReleaseDC = (pfnNtUserReleaseDC)RtlFindExportedRoutineByName(win32base, "NtUserReleaseDC");
	NtGdiPatBlt = (pfnNtGdiPatBlt)RtlFindExportedRoutineByName(win32kfull, "NtGdiPatBlt");
	GreSelectBrush = (pfnGreSelectBrush)RtlFindExportedRoutineByName(win32base, "GreSelectBrush");
	NtGdiCreateSolidBrush = (pfnNtGdiCreateSolidBrush)RtlFindExportedRoutineByName(win32kfull, "NtGdiCreateSolidBrush");
	NtGdiDeleteObjectApp = (pfnNtGdiDeleteObjectApp)RtlFindExportedRoutineByName(win32base, "NtGdiDeleteObjectApp");
	NtUserFindWindowEx = (pfnNtUserFindWindowEx)RtlFindExportedRoutineByName(win32kfull, "NtUserFindWindowEx");
	NtGdiExtTextOutW = (pfnNtGdiExtTextOutW)RtlFindExportedRoutineByName(win32kfull, "NtGdiExtTextOutW");
	NtUserInvalidateRect = (pfnNtUserInvalidateRect)RtlFindExportedRoutineByName(win32kfull, "NtUserInvalidateRect");
	NtUserGetDCEx = (pfnNtUserGetDCEx)RtlFindExportedRoutineByName(win32kfull, "NtUserGetDCEx");
	NtUserRedrawWindow = (pfnNtUserRedrawWindow)RtlFindExportedRoutineByName(win32kfull, "NtUserRedrawWindow");
	mEngBitBlt = (pfnEngBitBlt)RtlFindExportedRoutineByName(win32kfull, "EngBitBlt");
	NtUserGetAsyncKeyState = (pfnNtUserGetAsyncKeyState)RtlFindExportedRoutineByName(win32base, "NtUserGetAsyncKeyState");


	unspoofthread();

	if (!NtUserGetDC || !NtGdiPatBlt || !GreSelectBrush ||
		!NtUserReleaseDC || !NtGdiCreateSolidBrush || !NtGdiDeleteObjectApp)
	{
		return FALSE;
	}
	return 1;

}


PVOID GetModuleBase(PCHAR szModuleName)
{
	PVOID result = 0;
	ULONG length = 0;

	ZwQuerySystemInformation(SystemModuleInformation, &length, 0, &length);
	if (!length) return result;

	const unsigned long tag = 'MEM';
	PSYSTEM_MODULE_INFORMATION system_modules = (PSYSTEM_MODULE_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, length, tag);
	if (!system_modules) return result;

	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, system_modules, length, 0);
	if (NT_SUCCESS(status))
	{
		for (size_t i = 0; i < system_modules->ulModuleCount; i++)
		{
			char* fileName = (char*)system_modules->Modules[i].ImageName + system_modules->Modules[i].ModuleNameOffset;
			if (!strcmp(fileName, szModuleName))
			{
				result = system_modules->Modules[i].Base;
				break;
			}
		}
	}
	ExFreePoolWithTag(system_modules, tag);
	return result;
}

ULONG GetActiveProcessLinksOffset()
{
	UNICODE_STRING FunName = { 0 };
	RtlInitUnicodeString(&FunName, L"PsGetProcessId");

	/*
	.text:000000014007E054                   PsGetProcessId  proc near
	.text:000000014007E054
	.text:000000014007E054 48 8B 81 80 01 00+                mov     rax, [rcx+180h]
	.text:000000014007E054 00
	.text:000000014007E05B C3                                retn
	.text:000000014007E05B                   PsGetProcessId  endp
	*/

	PUCHAR pfnPsGetProcessId = (PUCHAR)MmGetSystemRoutineAddress(&FunName);
	if (pfnPsGetProcessId && MmIsAddressValid(pfnPsGetProcessId) && MmIsAddressValid(pfnPsGetProcessId + 0x7))
	{
		for (size_t i = 0; i < 0x7; i++)
		{
			if (pfnPsGetProcessId[i] == 0x48 && pfnPsGetProcessId[i + 1] == 0x8B)
			{
				return *(PULONG)(pfnPsGetProcessId + i + 3) + 8;
			}
		}
	}
	return 0;
}


typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE           Reserved1[16];
	PVOID          Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

NTSTATUS GetProcessImagePath(PEPROCESS process, PUNICODE_STRING imagePath) {
	PROCESS_BASIC_INFORMATION pbi;
	NTSTATUS status;

	// Query basic information about the process to get the PEB base address
	status = ZwQueryInformationProcess(process, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
	if (!NT_SUCCESS(status)) {
		DbgPrintEx(0, 0, "ZwQueryInformationProcess failed\n");
		return status;
	}

	// PEB base address is now in pbi.PebBaseAddress
	PPEB peb = (PPEB)pbi.PebBaseAddress;
	if (peb == NULL) {
		DbgPrintEx(0, 0, "pebaddress null\n");
		return STATUS_UNSUCCESSFUL;
	}

	// The PEB structure contains the address of the ProcessParameters structure
	PRTL_USER_PROCESS_PARAMETERS processParameters = (PRTL_USER_PROCESS_PARAMETERS)peb->ProcessParameters;
	if (processParameters == NULL) {
		DbgPrintEx(0, 0, "processParameters null\n");
		return STATUS_UNSUCCESSFUL;
	}

	// ProcessParameters->ImagePathName contains the full path to the executable
	*imagePath = processParameters->ImagePathName;

	return STATUS_SUCCESS;
}


int processes = 0;

PEPROCESS GetProcessByName(PCHAR szName)
{
	PEPROCESS Process = NULL;
	PCHAR ProcessName;  // Changed from PCHAR to PUNICODE_STRING
	PLIST_ENTRY pHead = NULL;
	PLIST_ENTRY pNode = NULL;

	ULONG64 ActiveProcessLinksOffset = GetActiveProcessLinksOffset();
	if (!ActiveProcessLinksOffset)
	{
		KdPrint(("GetActiveProcessLinksOffset failed\n"));
		return NULL;
	}

	Process = PsGetCurrentProcess();
	pHead = (PLIST_ENTRY)((ULONG64)Process + ActiveProcessLinksOffset);
	pNode = pHead;

	do
	{
		Process = (PEPROCESS)((ULONG64)pNode - ActiveProcessLinksOffset);
		ProcessName = PsGetProcessImageFileName(Process);
		//KdPrint(("%s\n", ProcessName));
		if (!strcmp(szName, ProcessName))
		{
			return Process;
		}



		pNode = pNode->Flink;
	} while (pNode != pHead);

	return NULL;
}


PUNICODE_STRING unicodestrstr(
	PUNICODE_STRING Haystack,
	PUNICODE_STRING Needle
) {
	if (!Haystack || !Needle || !Haystack->Buffer || !Needle->Buffer)
		return NULL;

	if (Needle->Length == 0)
		return Haystack;

	USHORT haystackLen = Haystack->Length / sizeof(WCHAR);
	USHORT needleLen = Needle->Length / sizeof(WCHAR);

	if (needleLen > haystackLen)
		return NULL;

	for (USHORT i = 0; i <= haystackLen - needleLen; i++) {
		if (RtlCompareMemory(Haystack->Buffer + i, Needle->Buffer, Needle->Length) == Needle->Length) {
			return (PUNICODE_STRING)&Haystack->Buffer[i];
		}
	}

	return NULL;
}

// we need this as game file can be more than 14 characters with multiple instances (FortniteClient-Win64-Shipping.exe and FortniteClient-Win64-Shipping_EAC.exe. so we need the full image name.
PEPROCESS GetProcess(PWCH szName)
{
	PEPROCESS Process = NULL;
	PUNICODE_STRING ProcessName = NULL;  // Changed from PCHAR to PUNICODE_STRING
	PLIST_ENTRY pHead = NULL;
	PLIST_ENTRY pNode = NULL;

	ULONG64 ActiveProcessLinksOffset = GetActiveProcessLinksOffset();
	if (!ActiveProcessLinksOffset)
	{
		KdPrint(("GetActiveProcessLinksOffset failed\n"));
		return NULL;
	}

	Process = PsGetCurrentProcess();
	pHead = (PLIST_ENTRY)((ULONG64)Process + ActiveProcessLinksOffset);
	pNode = pHead;

	do
	{
		Process = (PEPROCESS)((ULONG64)pNode - ActiveProcessLinksOffset);
		if (PsGetProcessExitStatus(Process) != STATUS_PENDING) {
			pNode = pNode->Flink;
			continue;
		}
		NTSTATUS status = SeLocateProcessImageName(Process, &ProcessName);
		if (NT_SUCCESS(status) && ProcessName) {
			UNICODE_STRING TargetName;
			RtlInitUnicodeString(&TargetName, szName);
			if (unicodestrstr(ProcessName, &TargetName)) {
				ExFreePool(ProcessName);
				pid = PsGetProcessId(Process);
				return Process;
			}

			ExFreePool(ProcessName);

		}


		/*int pid = PsGetProcessId(Process);
		if (pid == 6448) {
			return Process;
		}*/



		pNode = pNode->Flink;
	} while (pNode != pHead);

	return NULL;
}

PETHREAD GetProcessMainThread(PEPROCESS Process)
{
	PETHREAD ethread = NULL;

	KAPC_STATE kApcState;

	KeStackAttachProcess(Process, &kApcState);

	HANDLE hThread = NULL;

	NTSTATUS status = ZwGetNextThread(NtCurrentProcess(), NULL, THREAD_ALL_ACCESS,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 0, &hThread);

	if (NT_SUCCESS(status))
	{

		status = ObReferenceObjectByHandle(hThread, THREAD_ALL_ACCESS,
			*PsThreadType, KernelMode, (PVOID*)&ethread, NULL);
		NtClose(hThread);

		if (!NT_SUCCESS(status))
		{
			ethread = NULL;
		}
	}

	KeUnstackDetachProcess(&kApcState);
	return ethread;
}




int spoofthread() {
	PEPROCESS maskprocess = GetProcessByName("dwm.exe");
	if (!maskprocess) {
		DbgPrintEx(0, 0, "dwm not found\n");
		return 0;
	}
	PETHREAD thread = GetProcessMainThread(maskprocess);
	PVOID maskthread = PsGetThreadWin32Thread(thread);
	if (!maskthread)
	{
		KdPrint(("win32thread failed\n"));
		return FALSE;
	}
	PKTHREAD currentThread = KeGetCurrentThread();

	OriginalWin32Thread = PsGetCurrentThreadWin32Thread();
	OriginalProcess = PsGetThreadProcess(currentThread);

	KeStackAttachProcess(maskprocess, &apc_state);

	PsSetThreadWin32Thread(currentThread, maskthread, PsGetCurrentThreadWin32Thread());

	*(PEPROCESS*)((char*)currentThread + ThreadProcessOffset) = maskprocess;
	return 1;
}


int unspoofthread()
{
	PKTHREAD currentThread = KeGetCurrentThread();
	if (currentThread) {
		PsSetThreadWin32Thread(currentThread, OriginalWin32Thread, PsGetCurrentThreadWin32Thread());
		*(PEPROCESS*)((char*)currentThread + ThreadProcessOffset) = OriginalProcess;
	}

	KeUnstackDetachProcess(&apc_state);

	return TRUE;
}

PVOID GetProcessBaseAddress(int pid)
{
	PEPROCESS pProcess = NULL;
	if (pid == 0) return STATUS_UNSUCCESSFUL;

	NTSTATUS NtRet = PsLookupProcessByProcessId(pid, &pProcess);
	if (NtRet != STATUS_SUCCESS) return NtRet;

	PVOID Base = PsGetProcessSectionBaseAddress(pProcess);
	ObDereferenceObject(pProcess);
	return Base;
}

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
// ====== State ======
static PEPROCESS g_DwmProcess = NULL;

static uintptr_t g_AddrOverlaysEnabled = 0; // ?OverlaysEnabled@COverlayContext@@AEBA_NXZ
static uintptr_t g_AddrIsCandDFC = 0; // ?IsCandidateDirectFlipCompatbile@COverlayContext@@AEBA_N...

static UCHAR     g_OrigOE[3] = { 0 };
static UCHAR     g_OrigIF[3] = { 0 };
static BOOLEAN   g_HaveBackupOE = FALSE;
static BOOLEAN   g_HaveBackupIF = FALSE;
static BOOLEAN   g_IsPatchedOE = FALSE;
static BOOLEAN   g_IsPatchedIF = FALSE;


NTSTATUS GetAddressDwm(void)
{
	if (g_AddrOverlaysEnabled && g_AddrIsCandDFC && g_DwmProcess)
		return STATUS_SUCCESS;

	NTSTATUS status;
	KERNEL_BUFFER pdbFileBuffer = { 0 };
	KERNEL_BUFFER peFileBuffer = { 0 };
	KERNEL_BUFFER symbolStreamBuffer = { 0 };

	ULONG64 rvaOE = 0, rvaIF = 0;
	uintptr_t dwmcoreBase = 0;

	PEPROCESS sourceProcess = GetProcess(L"dwm.exe");
	g_DwmProcess = sourceProcess;

	if (!sourceProcess || PsGetProcessExitStatus(sourceProcess) != STATUS_PENDING) {
		DbgPrintEx(0, 0, "dwm not found?1?1?!\n");
		return (NTSTATUS)-1;
	}

	// Find dwmcore.dll base
	KAPC_STATE apcState;
	KeStackAttachProcess(sourceProcess, &apcState);
	PPEB pPeb = PsGetProcessPeb(sourceProcess);
	if (pPeb) {
		PPEB_LDR_DATA pLdr = pPeb->Ldr;
		if (pLdr) {
			PLIST_ENTRY head = &pLdr->InMemoryOrderModuleList;
			for (PLIST_ENTRY e = head->Flink; e != head; e = e->Flink) {
				PLDR_DATA_TABLE_ENTRY ent = CONTAINING_RECORD(e, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
				if (ent->DllBase) {
					UNICODE_STRING dwmcoreUS;
					RtlInitUnicodeString(&dwmcoreUS, L"\\dwmcore.dll");
					if (unicodestrstr(&ent->FullDllName, &dwmcoreUS)) {
						dwmcoreBase = (uintptr_t)ent->DllBase;
						break;
					}
				}
			}
		}
	}
	KeUnstackDetachProcess(&apcState);

	if (!dwmcoreBase)
		return (NTSTATUS)-2;

	UNICODE_STRING pdbPath = RTL_CONSTANT_STRING(L"\\SystemRoot\\System32\\dwmcore.pdb");
	UNICODE_STRING pePath = RTL_CONSTANT_STRING(L"\\SystemRoot\\System32\\dwmcore.dll");

	status = ReadFileToBuffer(&pdbPath, &pdbFileBuffer);
	if (!NT_SUCCESS(status)) return status;

	status = ReadFileToBuffer(&pePath, &peFileBuffer);
	if (!NT_SUCCESS(status)) { ExFreePoolWithTag(pdbFileBuffer.Buffer, 'bPdb'); return status; }

	status = GetPdbSymbolStream(pdbFileBuffer.Buffer, &symbolStreamBuffer);
	ExFreePoolWithTag(pdbFileBuffer.Buffer, 'bPdb');
	if (!NT_SUCCESS(status)) { ExFreePoolWithTag(peFileBuffer.Buffer, 'ePsP'); return status; }

	// Resolve both RVAs from same symbol stream
	PCSTR symOE = "?OverlaysEnabled@COverlayContext@@AEBA_NXZ";
	PCSTR symIF = "?IsCandidateDirectFlipCompatbile@COverlayContext@@AEBA_NPEBVCCompositionSurfaceInfo@@PEAVISwapChainRealization@@AEBUDXGI_MULTIPLANE_OVERLAY_ATTRIBUTES@@W4DXGI_MODE_ROTATION@@I_N4@Z";

	status = FindSymbolRva(&symbolStreamBuffer, peFileBuffer.Buffer, symOE, &rvaOE);
	if (NT_SUCCESS(status))
		status = FindSymbolRva(&symbolStreamBuffer, peFileBuffer.Buffer, symIF, &rvaIF);

	ExFreePoolWithTag(peFileBuffer.Buffer, 'ePsP');
	ExFreePoolWithTag(symbolStreamBuffer.Buffer, 'SymB');

	if (!NT_SUCCESS(status)) return status;

	g_AddrOverlaysEnabled = dwmcoreBase + rvaOE;
	g_AddrIsCandDFC = dwmcoreBase + rvaIF;
	return STATUS_SUCCESS;
}

NTSTATUS PatchDwm(void)
{
	if (!g_AddrOverlaysEnabled || !g_AddrIsCandDFC || !g_DwmProcess) {
		NTSTATUS st = GetAddressDwm();
		if (!NT_SUCCESS(st)) return st;
	}

	NTSTATUS status;
	HANDLE hProc = NULL;

	status = ObOpenObjectByPointer(
		g_DwmProcess, OBJ_KERNEL_HANDLE, NULL,
		PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
		*PsProcessType, KernelMode, &hProc);
	if (!NT_SUCCESS(status)) return status;

	if (!g_IsPatchedOE) {
		const UCHAR patchOE[3] = { 0x30, 0xC0, 0xC3 };
		PVOID  base = PAGE_ALIGN((PVOID)g_AddrOverlaysEnabled);
		SIZE_T size = ADDRESS_AND_SIZE_TO_SPAN_PAGES((PVOID)g_AddrOverlaysEnabled, 3) * PAGE_SIZE;
		ULONG  oldProt = 0, tmp = 0;

		status = ZwProtectVirtualMemory(hProc, &base, &size, PAGE_EXECUTE_READWRITE, &oldProt);
		if (!NT_SUCCESS(status)) goto done;

		if (!g_HaveBackupOE) {
			SIZE_T got = 0;
			status = MmCopyVirtualMemory(g_DwmProcess, (PVOID)g_AddrOverlaysEnabled,
				PsGetCurrentProcess(), g_OrigOE,
				3, KernelMode, &got);
			if (!NT_SUCCESS(status) || got != 3) {
				ZwProtectVirtualMemory(hProc, &base, &size, oldProt, &tmp);
				goto done;
			}
			g_HaveBackupOE = TRUE;
		}

		SIZE_T wrote = 0;
		status = MmCopyVirtualMemory(PsGetCurrentProcess(), (PVOID)patchOE,
			g_DwmProcess, (PVOID)g_AddrOverlaysEnabled,
			3, KernelMode, &wrote);

		ZwProtectVirtualMemory(hProc, &base, &size, oldProt, &tmp);
		ZwFlushInstructionCache(hProc, (PVOID)g_AddrOverlaysEnabled, 3);

		if (!NT_SUCCESS(status) || wrote != 3) goto done;

		g_IsPatchedOE = TRUE;
	}

	if (!g_IsPatchedIF){

		const UCHAR patchIF[3] = { 0x30, 0xC0, 0xC3 };
		PVOID  base = PAGE_ALIGN((PVOID)g_AddrIsCandDFC);
		SIZE_T size = ADDRESS_AND_SIZE_TO_SPAN_PAGES((PVOID)g_AddrIsCandDFC, 3) * PAGE_SIZE;
		ULONG  oldProt = 0, tmp = 0;

		status = ZwProtectVirtualMemory(hProc, &base, &size, PAGE_EXECUTE_READWRITE, &oldProt);
		if (!NT_SUCCESS(status)) goto done;

		if (!g_HaveBackupIF) {
			SIZE_T got = 0;
			status = MmCopyVirtualMemory(g_DwmProcess, (PVOID)g_AddrIsCandDFC,
				PsGetCurrentProcess(), g_OrigIF,
				3, KernelMode, &got);
			if (!NT_SUCCESS(status) || got != 3) {
				ZwProtectVirtualMemory(hProc, &base, &size, oldProt, &tmp);
				goto done;
			}
			g_HaveBackupIF = TRUE;
		}

		SIZE_T wrote = 0;
		status = MmCopyVirtualMemory(PsGetCurrentProcess(), (PVOID)patchIF,
			g_DwmProcess, (PVOID)g_AddrIsCandDFC,
			3, KernelMode, &wrote);

		ZwProtectVirtualMemory(hProc, &base, &size, oldProt, &tmp);
		ZwFlushInstructionCache(hProc, (PVOID)g_AddrIsCandDFC, 3);

		if (!NT_SUCCESS(status) || wrote != 3) goto done;

		g_IsPatchedIF = TRUE;
	}

done:
	if (hProc) ZwClose(hProc);
	return status;
}

NTSTATUS UnpatchDwm(void)
{
	if (!g_DwmProcess || !g_AddrOverlaysEnabled || !g_AddrIsCandDFC)
		return STATUS_INVALID_DEVICE_STATE;

	NTSTATUS status = STATUS_SUCCESS;
	HANDLE hProc = NULL;

	status = ObOpenObjectByPointer(
		g_DwmProcess, OBJ_KERNEL_HANDLE, NULL,
		PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
		*PsProcessType, KernelMode, &hProc);
	if (!NT_SUCCESS(status)) return status;

	if (g_IsPatchedOE && g_HaveBackupOE) {
		PVOID  base = PAGE_ALIGN((PVOID)g_AddrOverlaysEnabled);
		SIZE_T size = ADDRESS_AND_SIZE_TO_SPAN_PAGES((PVOID)g_AddrOverlaysEnabled, 3) * PAGE_SIZE;
		ULONG  oldProt = 0, tmp = 0;

		NTSTATUS st = ZwProtectVirtualMemory(hProc, &base, &size, PAGE_EXECUTE_READWRITE, &oldProt);
		if (NT_SUCCESS(st)) {
			SIZE_T wrote = 0;
			st = MmCopyVirtualMemory(PsGetCurrentProcess(), g_OrigOE,
				g_DwmProcess, (PVOID)g_AddrOverlaysEnabled,
				3, KernelMode, &wrote);
			ZwProtectVirtualMemory(hProc, &base, &size, oldProt, &tmp);
			ZwFlushInstructionCache(hProc, (PVOID)g_AddrOverlaysEnabled, 3);
			if (NT_SUCCESS(st) && wrote == 3) g_IsPatchedOE = FALSE;
			else status = st ? st : STATUS_PARTIAL_COPY;
		}
		else status = st;
	}

	if (g_IsPatchedIF && g_HaveBackupIF) {
		PVOID  base = PAGE_ALIGN((PVOID)g_AddrIsCandDFC);
		SIZE_T size = ADDRESS_AND_SIZE_TO_SPAN_PAGES((PVOID)g_AddrIsCandDFC, 3) * PAGE_SIZE;
		ULONG  oldProt = 0, tmp = 0;

		NTSTATUS st = ZwProtectVirtualMemory(hProc, &base, &size, PAGE_EXECUTE_READWRITE, &oldProt);
		if (NT_SUCCESS(st)) {
			SIZE_T wrote = 0;
			st = MmCopyVirtualMemory(PsGetCurrentProcess(), g_OrigIF,
				g_DwmProcess, (PVOID)g_AddrIsCandDFC,
				3, KernelMode, &wrote);
			ZwProtectVirtualMemory(hProc, &base, &size, oldProt, &tmp);
			ZwFlushInstructionCache(hProc, (PVOID)g_AddrIsCandDFC, 3);
			if (NT_SUCCESS(st) && wrote == 3) g_IsPatchedIF = FALSE;
			else status = st ? st : STATUS_PARTIAL_COPY;
		}
		else status = st;
	}

	if (hProc) ZwClose(hProc);
	return status;
}



