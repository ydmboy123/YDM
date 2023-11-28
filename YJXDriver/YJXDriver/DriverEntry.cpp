#include <wdm.h>
#include "YFunction.h"
#include "YCreateDevice.h"
#include "YUnloadDelete.h"


extern "C"
void DriverUnloadFun(PDRIVER_OBJECT pd)
{
	DbgPrint("DriverUnloadFun \n");
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
	pd->MajorFunction[IRP_MJ_CREATE] = YCreateRoutine;
	YCreateDevice(pd);
	DbgPrint("%wZ",unicode_str);

	aaa
	return status;


}
