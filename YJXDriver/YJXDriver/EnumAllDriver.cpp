#include "EnumAllDriver.h"

extern "C"
VOID EnumDriver(PDRIVER_OBJECT pDriverObject)
{
	PKLDR_DATA_TABLE_ENTRY entry = (PKLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	PKLDR_DATA_TABLE_ENTRY firstEntry = NULL;
	ULONG64 pDrvBase = 0;
	KIRQL OldIrql;
	firstEntry = entry;// 就是用来while循环判断
	
	while ((PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink != firstEntry)
	{
		DbgPrint("BASE=%p\tPATH=%wZ",entry->DllBase,entry->FullDllName);
		entry = (PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink;
	}
}
extern "C"
VOID EnumDriverByPKLDR(PKLDR_DATA_TABLE_ENTRY entry)
{
	PKLDR_DATA_TABLE_ENTRY firstEntry = NULL;
	ULONG64 pDrvBase = 0;
	KIRQL OldIrql;
	firstEntry = entry;// 就是用来while循环判断
	while ((PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink != firstEntry)
	{
		DbgPrint("BASE=%p\tPATH=%wZ",entry->DllBase,entry->FullDllName);
		entry = (PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink;
	}
}

extern "C"
VOID EnumCallBack()
{
	POB_CALLBACK pObCallback = NULL;
	LIST_ENTRY CallbackList = ((POBJECT_TYPE)(*PsThreadType))->CallbackList;
	pObCallback = (POB_CALLBACK)CallbackList.Flink;
}

