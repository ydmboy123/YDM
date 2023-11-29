#include <wdm.h>
#include "YFunction.h"
#include "YCreateDevice.h"
#include "YUnloadDelete.h"
#include "HookByRegisterCallbacks.h"




ULONG Flags;
extern "C"
void DriverUnloadFun(PDRIVER_OBJECT pd)
{
	UnistallMemoryProctect();
	DbgPrint("DriverUnloadFun \n");
	PKLDR_DATA_TABLE_ENTRY pLdrData = (PKLDR_DATA_TABLE_ENTRY)pd->DriverSection;
	//pLdrData->Flags = Flags;
	YUnloadDelete(pd);
	return;
}
extern "C"
NTSTATUS DriverEntry(PDRIVER_OBJECT pd, PUNICODE_STRING pUnicode)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING unicode_str = { 0 };
	RtlInitUnicodeString(&unicode_str, L"\\ydmboy");

	pd->DriverUnload = DriverUnloadFun;
	PKLDR_DATA_TABLE_ENTRY pLdrData = (PKLDR_DATA_TABLE_ENTRY)pd->DriverSection;
	Flags = pLdrData->Flags;
	pLdrData->Flags = pLdrData->Flags | 0x20;

	//pd->DriverSection->Flags = pd->DriverSection->Flags | 0x20;
	//pd->MajorFunction[IRP_MJ_CREATE] = YCreateRoutine;
	YCreateDevice(pd);
	UnistallAllProcessType();
	//setMemoryProtecte();
	DbgPrint("%wZ",unicode_str);


	return status;


}
