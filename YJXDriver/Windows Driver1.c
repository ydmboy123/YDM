#include "Hook.h"

VOID PageProtectOff()
{
	ULONG_PTR cr0;
	//__disable();										//�����ж�
	Irql = KeRaiseIrqlToDpcLevel();						//������DpcLevelȻ�󱣴�ԭ����IRQL
	cr0 = __readcr0();									//��ȡcr0
	cr0 &= 0xfffffffffffeffff;							//��ҳд�뱣��λ��������
	__writecr0(cr0);									//д��cr0
}

VOID PageProtectOn()
{
	ULONG_PTR cr0;
	cr0 = __readcr0();									//��ȡcr0
	cr0 |= 0x10000;										//��ԭҳ����λ
	__writecr0(cr0);									//д��cr0
	//__enable();										//��������ж�����
	KeLowerIrql(Irql);									//����IRQL�������ֵ
}

ULONG_PTR GetFuncAddress(PWSTR FuncName)
{

	UNICODE_STRING uFunctionName;
	RtlInitUnicodeString(&uFunctionName, FuncName);
	return (ULONG_PTR)MmGetSystemRoutineAddress(&uFunctionName);
}

NTSTATUS __fastcall MyPsLookupProcessByProcessId(__in HANDLE ProcessId, __deref_out PEPROCESS *Process)
{
	NTSTATUS RetStatus;
	return STATUS_ACCESS_DENIED;
	RetStatus = ((PSLOOKUPPROCESSBYPROCESSID)(OldFunc))(ProcessId, Process);
	if (NT_SUCCESS(RetStatus) && strstr((CHAR*)PsGetProcessImageFileName(*Process), "calc"))
	{
		KdPrint(("������ͨ��PID��ȡ��������EPROCESS\n"));
		*Process = NULL;
		return STATUS_ACCESS_DENIED;
	}

	return RetStatus;
}

VOID StartHook()
{
	ULONG_PTR PsLookupAddress;

	PsLookupAddress = GetFuncAddress(L"PsLookupProcessByProcessId");
	
	OldFunc = (UCHAR*)sfExAllocate(sizeof(OldCode) + sizeof(JmpOld));					//�ȷ����ڴ�������������ʧ��Ҳû����Ҫ�����ˣ���Ϊ������ת�����ĺ���

	if (OldFunc == NULL)
	{
		KdPrint(("�����ڴ�ʧ�ܣ�\n"));
		return;
	}

	*(ULONG_PTR*)(JmpOld + 6) = PsLookupAddress + 15;									//��ת��ԭ������15���ֽڴ�

	*(ULONG_PTR*)(NewCode + 6) = (ULONG_PTR)MyPsLookupProcessByProcessId;				//��ת��ַָ�������Լ��ĺ���

	RtlCopyMemory((PVOID)OldCode, (PVOID)PsLookupAddress, sizeof(OldCode));				//����ԭ�������������15���ֽڱ�������

	PageProtectOff();

	RtlCopyMemory((PVOID)PsLookupAddress, (PVOID)NewCode, sizeof(NewCode));				//����ת������ԭ������

	PageProtectOn();

	RtlCopyMemory((PVOID)OldFunc, (PVOID)OldCode, sizeof(OldCode));						//����ԭ����ʮ����ֽڳ���

	RtlCopyMemory((PVOID)(OldFunc + sizeof(OldCode)), (PVOID)JmpOld, sizeof(JmpOld));	//������ת��ԭ������ʮ����ֽڴ���ָ�����
}

VOID StopHook()
{
	ULONG_PTR PsLookupAddress;

	PsLookupAddress = GetFuncAddress(L"PsLookupProcessByProcessId");

	PageProtectOff();

	RtlCopyMemory((PVOID)PsLookupAddress, (PVOID)OldCode, sizeof(OldCode));				//��ԭ������ָ��

	PageProtectOn();

	sfExFree(OldFunc);
}

VOID Unload(PDRIVER_OBJECT DriverObject)
{
	if (fCode)
	{
		ExFreePoolWithTag(fCode, "ydmy");
	}
	//StopHook();
	KdPrint(("Unload Success!\n"));
}
VOID HookFunc()
{
	DbgPrint("HookFunc\n");
	//if (fCode)
	//{
	//	//__asm
	//	//{
	//	//	//mov eax,1
	//	//	//mov rip,fCode
	//	//}
	//}
}

VOID test()
{
	
	ULONG_PTR PsLookupAddr = NULL;
	PsLookupAddr = GetFuncAddress(L"PsLookupProcessByProcessId");
	if (!PsLookupAddr || !JmpOld)
		return;


	//PVOID YPsLookupProcessByProcess = sfExAllocate(siezeof(oldCode))
	fCode = (char*)ExAllocatePoolWithTag(NonPagedPool, sizeof(OldCode) + sizeof(NewCode), "ydmy");
	

	RtlCopyMemory(OldCode,PsLookupAddr,sizeof(OldCode)); 
	RtlCopyMemory(NewCode+6,&PsLookupAddr,sizeof(void*));
	RtlCopyMemory(fCode, OldCode, sizeof(OldCode));
	RtlCopyMemory(fCode + sizeof(OldCode), NewCode, sizeof(NewCode));
	
	*(ULONG_PTR*)(JmpOld + 6) = HookFunc;


	//RtlCopyMemory((PVOID)JmpOld, (PVOID) PsLookupAddr, sizeof(JmpOld));
	
	
	PageProtectOff();
	RtlCopyMemory(PsLookupAddr, (PVOID) JmpOld, sizeof(JmpOld));
	PageProtectOn();


}
VOID testA(int a, char b)// 2B
{
	DbgPrint("asd");
	return NULL;
}


void HookFcodeTest()
{
	//fCode = 
}

//ULONG_PTR GetCurrentInstructionPointer();
//VOID JmpAnyFunc(__int64 addr); 

extern void JmpAnyFunc(void* addr);

void JmpAddr(void* addr)
{
	((void(*)())addr)();
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegString)
{	
	KdPrint(("Entry Driver!\n"));
	
	ULONG_PTR* ptr = testA;
	((void(*)())ptr)();

	fCode = sfExAllocate(0x2b);
	RtlCopyMemory(fCode, testA, 0x2b);



	//((void (*)())&fCode)();
	//((void (*)())fCode)();



	//DbgPrint("%llx",fCode);
	//DbgPrint("%llx",&fCode);


	//JmpAddr(fCode);

	    //((void (*)())addr)();
	//((void(*)())&fCode)();


	//DbgPrintf("%d",sizeof(testA));

	//ULONG_PTR rip = GetCurrentInstructionPointer();
	//rip = 0;

	//DbgPrint("%llx",rip);
	// ����rip�б����˵�ǰָ��ĵ�ַ

	//test();
	//testA(1,'a');
	//StartHook();
	//PVOID nVOid = ExAllocatePoolWithTag(NonPagedPool, 10, "ydma");
	//RtlCopyMemory(nVOid, L"asd", sizeof(L"asd"));
	//if (nVOid)
	//{
	//	ExFreePoolWithTag(nVOid, "ydma");
	//}

// test
	//ULONG_PTR pAddr = GetFuncAddress(L"RtlCopyMemory");
	//ULONG_PTR pAddr = GetFuncAddress(L"PsLookupProcessByProcessId");
	//ULONG pIntAddr = (ULONG)pAddr;
	//int x = 15;
	//DbgPrint("Func:%I64x\n",pIntAddr);
	//DbgPrint("x:%I64x\n",x);
// test

	DriverObject->DriverUnload = Unload;
	return STATUS_SUCCESS;
}