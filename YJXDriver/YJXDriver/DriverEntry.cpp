#include "YFunction.h"
#include "YCreateDevice.h"
#include "YUnloadDelete.h"
#include "HookByRegisterCallbacks.h"
#include "readSysMemory.h"
#include "writeProcessMemoryByPid.h"
#include "EnumerateKernelHook.h"
#include "EnumAllDriver.h"
#include "YTerminateProcess.h"
#include "findProcess.h"


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
	DbgPrint("_DRIVER_OBJECT:%d",pd->Type);
	DbgPrint("DRIVER_NAME:%wZ",pd->DriverName);
	DbgPrint("Driver_star:%llx",pd->DriverStart);
	DbgPrint("Driver_Ob_addr:%llx",pd->DeviceObject);
	DbgPrint("Driver_Section:%llx",pd->DriverSection);
	DbgPrint("Driver_DriverExtension:%llx",pd->DriverExtension);
	DbgPrint("Driver_HardwareDatabase:%wZ",pd->HardwareDatabase);
	DbgPrint("Driver_DriverInit%llx", pd->DriverInit);

/*
nt!PspTerminateProcess:
fffff801`173110a8 48895c2408      mov     qword ptr [rsp+8],rbx
fffff801`173110ad 48896c2410      mov     qword ptr [rsp+10h],rbp
fffff801`173110b2 4889742418      mov     qword ptr [rsp+18h],rsi
fffff801`173110b7 57              push    rdi
fffff801`173110b8 4156            push    r14
fffff801`173110ba 4157            push    r15
fffff801`173110bc 4883ec20        sub     rsp,20h
fffff801`173110c0 418bf9          mov     edi,r9d
*/




	UCHAR spCode[] ={ 0x41,0x8b,0xe8,0x4c,0x8b,0xfa,0x48,0x8b,0xd9,0x0f,0x0d,0x89,0x04,0x03,0x00,0x00,0x8b,0x81,0x04,0x03,0x00,0x00,0x44,0x8b,0xd0,0x41,0x83,0xca,0x08,0xf0,0x44,0x0f,0xb1,0x91,0x04,0x03,0x00,0x00 };

	//DbgPrint("sizeOf%d", sizeof(spCode));
	//for (int i = 0; i < sizeof(spCode); i++)
	//{
	//	DbgPrint("%x",spCode[i]);
	//}
	//ZwTerminateProcess();

	HANDLE processHandle = { 0 };
	FindProcessByNameC(L"notepad.exe",&processHandle);



	//YTerminateProcess(pd,spCode,sizeof(spCode));
	

	//DbgPrint("Driver_object_a");


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
