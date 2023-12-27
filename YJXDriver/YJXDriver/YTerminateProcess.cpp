#include "YTerminateProcess.h"


extern "C"
NTSTATUS YTerminateProcess(IN PDRIVER_OBJECT driverObject,UCHAR g_szSpecialCode)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PKLDR_DATA_TABLE_ENTRY pLdrDataTableEntry = NULL;
	pfnPspTerminateProcess  pspTerminateProcess = NULL;
	PEPROCESS pEprocess = NULL;
	pLdrDataTableEntry = (PKLDR_DATA_TABLE_ENTRY)driverObject->DriverSection;
	pLdrDataTableEntry = CONTAINING_RECORD(pLdrDataTableEntry,LDR_DATA_TABLE_ENTRY,InLoadOrderLinks);



}
