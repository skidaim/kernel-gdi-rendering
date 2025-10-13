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
	NtGdiPatBlt(hdc, x + w, y, border, h, PATCOPY);
	NtGdiPatBlt(hdc, x, y + h, w, border, PATCOPY);

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

// Example usage
NTSTATUS GetFullImagePathFromProcess(PEPROCESS process) {
	UNICODE_STRING imagePath;
	NTSTATUS status = GetProcessImagePath(process, &imagePath);

	if (NT_SUCCESS(status)) {
		DbgPrintEx(0, 0, "Process full image path: %wZ\n", &imagePath);
	}
	else {
		DbgPrintEx(0, 0, "Failed to get full image path\n");
	}

	return status;
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
PEPROCESS GetGameProcess(PWCH szName)
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