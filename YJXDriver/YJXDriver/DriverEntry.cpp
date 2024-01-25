#include "YFunction.h"
#include "YCreateDevice.h"
#include "YUnloadDelete.h"
#include "HookByRegisterCallbacks.h"
#include "readSysMemory.h"
#include "writeProcessMemoryByPid.h"
#include "EnumerateKernelHook.h"
#include "EnumAllDriver.h"



ULONG Flags;
extern "C"
void DriverUnloadFun(PDRIVER_OBJECT pd)
{

	UnistallMemoryProctect();
	DbgPrint("DriverUnloadFun \n");
	PKLDR_DATA_TABLE_ENTRY pLdrData = (PKLDR_DATA_TABLE_ENTRY)pd->DriverSection;
	pLdrData->Flags = Flags;
	YUnloadDelete(pd);
	return;
}


typedef struct _MY_FILE_INFO
{
	LIST_ENTRY list_entry;
	PFILE_OBJECT file_Object;
	UNICODE_STRING file_Name;
	LARGE_INTEGER file_length;
}MY_FILE_INFO,*PMY_FILE_INFO;

extern "C"
NTSTATUS DriverEntry(PDRIVER_OBJECT pd, PUNICODE_STRING pUnicode)
{

	//UNREFERENCED_PARAMETER(gs_handleCallback);
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING unicode_str = { 0 };
	RtlInitUnicodeString(&unicode_str, L"\\ydmboy");

	pd->DriverUnload = DriverUnloadFun;
	PKLDR_DATA_TABLE_ENTRY pLdrData = (PKLDR_DATA_TABLE_ENTRY)pd->DriverSection;
	Flags = pLdrData->Flags;
	pLdrData->Flags = pLdrData->Flags | 0x20;

	DbgPrint("PDRIVER_OBJECT:%x",pd);

	/*CONTAINING_RECORD*/

	// Get the PsTerminateProcess


	



	//pd->DriverSection->Flags = pd->DriverSection->Flags | 0x20;
	//pd->MajorFunction[IRP_MJ_CREATE] = YCreateRoutine;
	//YCreateDevice(pd);
	//EnumDriver(pd);
	
	
	//UnistallAllProcessType();
	//WriteMemoryDbgPrint();
	//readMemoryPrint();
	//setMemoryProtecte();

	//EnumObRegisterCallBack();
	//EnumObjectTypeProcessCallBack();
	//EnumObjectTypeThreadCallBack();
//=== LIST_ENTRY BEGIN

	//LIST_ENTRY list = { 0 };
	//InitializeListHead(&list);
	//PMY_FILE_INFO pmFile = (PMY_FILE_INFO)ExAllocatePoolWithTag(PagedPool,sizeof(MY_FILE_INFO), 1);
	//InsertHeadList(&list, (PLIST_ENTRY)pmFile);
//=== LIST_ENTRY END
	
	
	DbgPrint("%wZ",unicode_str);


	return status;


}
