#include "HookByRegisterCallbacks.h"
#include "definition.h"

extern "C"  PCHAR PsGetProcessImageFileName(IN PEPROCESS pProcess);



extern "C"
NTSTATUS UnistallAllProcessType()
{
	OBJECT_TYPE* pspt = *(POBJECT_TYPE*)PsProcessType;
	PLIST_ENTRY head = (PLIST_ENTRY)&(pspt->CallbackList);
	PLIST_ENTRY current = head->Blink;
	while (current != head)
	{
		CALLBACK_ENTRY_ITEM* item = (CALLBACK_ENTRY_ITEM*)current;
		CALLBACK_ENTRY* entry = item->CallbackEntry;
		if (entry)
		{
			ObUnRegisterCallbacks(entry);
		}
		current = current->Blink;
	}
	return STATUS_SUCCESS;

}


extern "C"
OB_PREOP_CALLBACK_STATUS my_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{

	//LARGE_INTEGER delayTime = { 0 };
	//delayTime.QuadPart = -50000000;
	//KeDelayExecutionThread(KernelMode, FALSE, &delayTime);


	//DbgPrint("yjx:sys eEPROCESS=%p",OperationInformation->Object);
	PEPROCESS process = (PEPROCESS)OperationInformation->Object;
	//PEPROCESS process = IoThreadToProcess((PETHREAD)OperationInformation->Object);
	PCHAR processName = PsGetProcessImageFileName(process);


	int i = strlen(processName);
	int j = strlen("calc.exe");
	//if (_stricmp(processName, "calculator.exe") != 0)
	//if(_stricmp(processName,"SecurityHealthService.exe")==0)
	if(_stricmp(processName,"calc.exe")==0)
	{

		//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
		//OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
		//DbgPrint("ydmboy:TERMINATE");





		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess |= PROCESS_TERMINATE;
		OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess |= PROCESS_TERMINATE;
		DbgPrint("ydmboy:ALL_PROCESS");

		//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = PROCESS_ALL_ACCESS;
	
		//if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		//{

		//if (OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE)
		//{
			//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			//OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			//DbgPrint("calc Process 2\n");
		//}
		//if (OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess & PROCESS_TERMINATE)
		//{

		//	OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE; 
		//}

		//}
		//if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE) //{
		//	OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
		//}
		//else if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		//{
		//	OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
		//}
		//return STATUS_UNSUCCESSFUL;
	}


	//OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
	return OB_PREOP_SUCCESS;


	if (OperationInformation->KernelHandle)
	{

	}
	else
	{

		ACCESS_MASK OriginalPermination = OperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess;
		//ACCESS_MASK OriginalPerminationBackup = OriginalPermination;
		ACCESS_MASK NewPermination = OperationInformation->Parameters->CreateHandleInformation.DesiredAccess;
		//OriginalPermination &= ~PROCESS_TERMINATE;
		OriginalPermination &= ~PROCESS_VM_READ;
		OriginalPermination &= ~PROCESS_VM_WRITE;

		//OriginalPermination &= ~PROCESS_VM_WRITE;
		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = OriginalPermination;
		DbgPrint("yjx:原始权限 %x； 新的权限=%x",OriginalPermination,NewPermination);
	}
	return OB_PREOP_SUCCESS;
}

HANDLE hd = NULL;
HANDLE gs_handleCallback = NULL;
extern "C"
void setMemoryProtecte()
{
	OB_CALLBACK_REGISTRATION ob1_callback_reg = { 0 };
	OB_OPERATION_REGISTRATION ob2_operation = { 0 };
	RtlInitUnicodeString(&ob1_callback_reg.Altitude, L"31001");
	ob1_callback_reg.RegistrationContext = NULL;
	ob1_callback_reg.Version = OB_FLT_REGISTRATION_VERSION;
	ob1_callback_reg.OperationRegistrationCount = 1; // 只注册一个回调
	ob1_callback_reg.OperationRegistration = &ob2_operation;
	
	ob2_operation.ObjectType = PsProcessType;
	ob2_operation.Operations |= OB_OPERATION_HANDLE_CREATE;
	//ob2_operation.Operations |= OB_OPERATION_HANDLE_DUPLICATE;

	ob2_operation.PostOperation = NULL;
	ob2_operation.PreOperation = my_pre_callback;
	NTSTATUS status = ObRegisterCallbacks(&ob1_callback_reg, &gs_handleCallback);
	switch (status)
	{
	case STATUS_SUCCESS:
	{
		DbgPrint("Success:YJX:sys SetMemoryProtecte gs_handleCallback=%p \n",gs_handleCallback); 
		break;
	}
	case STATUS_INVALID_PARAMETER:
	{
		DbgPrint("Failed:STATUS_INVALID_PARAMETER");
		break;
	}
	case STATUS_FLT_INSTANCE_ALTITUDE_COLLISION
		:
	{
		DbgPrint("Failed:STATUS_FLT_INSTANCE_ALTITUDE_COLLISION");
		break;
	}
	case STATUS_ACCESS_DENIED:
	{
		DbgPrint("Failed:STATUS_ACCESS_DENIED");
		break;
	}
	case STATUS_INSUFFICIENT_RESOURCES:
	{
		DbgPrint("Failed:STATUS_INSUFFICIENT_RESOURCES");
		break;
	}
	default:
		break;
	}
}

extern "C"
void UnistallMemoryProctect()
{
	if (gs_handleCallback && GetProcessPsType(gs_handleCallback))
	{
		ObUnRegisterCallbacks(gs_handleCallback);
		gs_handleCallback = NULL;
	}
	else
	{
		UNREFERENCED_PARAMETER(gs_handleCallback);
	}
	DbgPrint("yjx:sys 卸载内存保护");
}


extern "C"
bool GetProcessPsType(HANDLE hd)
{
	OBJECT_TYPE* pspt = *(POBJECT_TYPE*)PsProcessType;
	PLIST_ENTRY head = (PLIST_ENTRY)&(pspt->CallbackList);
	PLIST_ENTRY current = head;
	while ((current = current->Blink) != head )
	{

		CALLBACK_ENTRY_ITEM* item = (CALLBACK_ENTRY_ITEM*)current;
		CALLBACK_ENTRY* entry = item->CallbackEntry;
		if (entry == hd)
		{
			return true;
		}
	}
	return false;

}

