#include "utils.h"
#include "gamestuff.h"


uintptr_t baseAddress = NULL;

BOOLEAN render = TRUE;

void main()
{
	if (render) {
		PatchDwm();
	}
	while (TRUE) {
		if (getkey()) {
			render = !render;
			if (render) {
				PatchDwm();
			}
			else {
				UnpatchDwm();
			}
			NtSleep(150);
		}
		if (!render) {
			NtSleep(100);
			continue;
		}

		PEPROCESS sourceProcess = GetProcess(L"cs2.exe");

		if (!sourceProcess || PsGetProcessExitStatus(sourceProcess) != STATUS_PENDING) {
			// DbgPrintEx(0, 0, "cs2 not found\n");
			NtSleep(1000); 
			continue;
		}

		KAPC_STATE apcState;
		KeStackAttachProcess(sourceProcess, &apcState);

		PPEB pPeb = PsGetProcessPeb(sourceProcess);

		// Resolve Base Address (client.dll)
		if (!baseAddress && pPeb) {
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
		}
		KeUnstackDetachProcess(&apcState);

		if (!baseAddress) {
			continue;
		}

		uintptr_t entitylist = NULL;
		read(baseAddress + 0x24AF268, &entitylist, sizeof(uintptr_t)); // dwEntityList
		if (!entitylist) continue;
		//return 0x327489;
		uintptr_t localPlayer = NULL;
		read(baseAddress + 0x22F4188, &localPlayer, sizeof(uintptr_t)); // dwLocalPlayerController
		if (!localPlayer) continue;

		int localTeam = 0;
		read(localPlayer + 0x3F3, &localTeam, sizeof(int)); // m_iTeamNum
		
		spoofthread();

		// Loop through players (1 to 64)
		read(baseAddress + 0x230FF20, &viewm, sizeof(viewmatrix));
		for (int i = 1; i <= 64; i++) {

			uintptr_t listentry = NULL;
			read(entitylist + (8 * ((i & 0x7FFF) >> 9) + 16), &listentry, sizeof(uintptr_t));

			if (!listentry) continue;
			uintptr_t playercontroller = NULL;
			read(listentry + 112 * (i & 0x1FF), &playercontroller, sizeof(uintptr_t));
			if (!playercontroller) continue;

			if (playercontroller == localPlayer) continue;

			int playerpawnhandle = 0;
			read(playercontroller + 0x90C, &playerpawnhandle, sizeof(int)); // m_hPlayerPawn
			if (!playerpawnhandle) continue;

			uintptr_t listentry2 = NULL;
			read(entitylist + 8 * ((playerpawnhandle & 0x7FFF) >> 9) + 16, &listentry2, sizeof(uintptr_t));
			if (!listentry2) continue;

			uintptr_t currentpawn = NULL;
			read(listentry2 + 112 * (playerpawnhandle & 0x1FF), &currentpawn, sizeof(uintptr_t));
			if (!currentpawn) continue;
			
			// 6. Check Team
			int playerTeam = 0;
			read(currentpawn + 0x3F3, &playerTeam, sizeof(int)); // m_iTeamNum
			if (playerTeam == localTeam) continue;
			
			// 7. Check Health
			int playerHealth = 0;
			read(currentpawn + 0x354, &playerHealth, sizeof(int)); // m_iHealth
			if (playerHealth <= 0 || playerHealth > 100) continue;

			Vector3 playerpos = { 0.f, 0.f, 0.f };
			read(currentpawn + 0x1588, &playerpos, sizeof(Vector3)); // m_vOldOrigin

			if (playerpos.x == 0.f && playerpos.y == 0.f) continue;
			
			Vector3 playerhead = { playerpos.x, playerpos.y, playerpos.z + 75.f };

			int x, y;
			int hx, hy;
			
			if (!WorldToScreen(playerpos, &x, &y) || !WorldToScreen(playerhead, &hx, &hy)) {
				continue;
			}

			if (x <= 0 || y <= 0 ) continue;

			const float height = (float)(y - hy);
			const float width = height / 2.4f;
			drawbox((float)(hx - (width / 2.f)), (float)hy, width, height, 2);
		}
		
		unspoofthread();
		NtSleep(1);
	}
	

	PsTerminateSystemThread(STATUS_SUCCESS);
}


uintptr_t DriverEntry() {

	init();
	PWORK_QUEUE_ITEM WorkItem = (PWORK_QUEUE_ITEM)ExAllocatePool(NonPagedPool, sizeof(WORK_QUEUE_ITEM));
	if (!WorkItem)
	{
		return STATUS_FAILED_DRIVER_ENTRY;
	}
	ExInitializeWorkItem(WorkItem, main, WorkItem);
	ExQueueWorkItem(WorkItem, DelayedWorkQueue);

	return STATUS_SUCCESS;
}