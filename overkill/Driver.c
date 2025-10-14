#include "utils.h"

uintptr_t baseAddress = NULL;





#include "gamestuff.h"


BOOLEAN render = TRUE;



void main()
{
	while (TRUE) {
		if (getkey()) {
			render = !render;
			if (render) {
				PatchDwm();
			}
			else {
				UnpatchDwm();
			}
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
		PEPROCESS sourceProcess = GetProcess(L"cs2.exe");

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
		read(baseAddress + 0x1E1FC08, &localPlayer, sizeof(uintptr_t)); //localplayercontroller
		if (!localPlayer) {
			//DbgPrintEx(0, 0, "localpawn NULL\n");

			continue;

		}

		uintptr_t localTeam = NULL;
		read(localPlayer + 0x3EB, &localTeam, sizeof(uintptr_t));
		if (!localTeam) {
			//DbgPrintEx(0, 0, "team NULL\n");
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
				//DbgPrintEx(0, 0, "playerTeam NULL\n");
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
	/*PatchDwm();
	return;*/
	PWORK_QUEUE_ITEM WorkItem = (PWORK_QUEUE_ITEM)ExAllocatePool(NonPagedPool, sizeof(WORK_QUEUE_ITEM));
	if (!WorkItem)
	{
		return STATUS_FAILED_DRIVER_ENTRY;
	}
	ExInitializeWorkItem(WorkItem, main, WorkItem);
	ExQueueWorkItem(WorkItem, DelayedWorkQueue);

	return STATUS_SUCCESS;
}