#include "writeProcessMemoryByPid.h"

extern "C"
BOOL writeMemory(writeMemoryStruct* data)
{
	DbgPrint("%d",data->data[0]);
	PEPROCESS process = NULL;
	if ((STATUS_SUCCESS ==
		PsLookupProcessByProcessId((HANDLE)data->pid, &process)) && process)
	{
	}
	else
	{
		return FALSE;
	}
	BYTE* getData;
	__try
	{
		getData = (BYTE*)ExAllocatePool(PagedPool, data->size);
	}
	__except (1)
	{
		return FALSE;
	}
	for (int i = 0; i < data->size; i++)
	{
		getData[i] = data->data[i];
	}
	KAPC_STATE stack = { 0 };
	KeStackAttachProcess(process, &stack);
	//__try
	//{
	//	ProbeForWrite((VOID*)data->addres,data->size,1);
	//}
	//__except (1)
	//{
	//	if (getData)
	//	{
	//		ExFreePool(getData);
	//	}
	//	return FALSE;
	//}
	PMDL mdl = IoAllocateMdl((PVOID)data->addres, data->size, 0, 0, NULL);
	if (!mdl)
	{
		if (getData)
		{
			ExFreePool(getData);
		}
		return FALSE;
	}
	//CSHORT mdFlags = mdl->MdlFlags;
	//mdl->MdlFlags = MDL_WRITE_OPERATION | MDL_ALLOCATED_FIXED_SIZE|MDL_PAGES_LOCKED;
	MmBuildMdlForNonPagedPool(mdl);
	BYTE* changeData = NULL;
	__try
	{
		changeData = (BYTE *)MmMapLockedPages(mdl, KernelMode);
		RtlCopyMemory(changeData, getData, data->size);
	}
	__except (1)
	{
		if (mdl)
		{
			IoFreeMdl(mdl);
		}
		if (getData)
		{
			ExFreePool(getData);
		}
		if (process)
		{
			ObDereferenceObject(process);
		}
		return FALSE;
	}
	//mdl->MdlFlags = mdFlags;
	if (mdl)
	{
		IoFreeMdl(mdl);
	}
	if (getData)
	{
		ExFreePool(getData);
	}
		KeUnstackDetachProcess(&stack);
	if (process)
	{
		ObDereferenceObject(process);
	}
	return TRUE;
}

extern "C"
void WriteMemoryDbgPrint()
{
	char dd[] = {0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc};
	BYTE* writeData = (BYTE*)ExAllocatePool(PagedPool, 10);//ÄÚ´æÐ¹Â¶
	writeMemoryStruct wm = { 0 };
	wm.pid = PID;
	wm.addres = ADDRESS;
	wm.data = writeData;
	wm.size = 10;
	RtlCopyMemory(writeData, dd, 10);
	if (writeMemory(&wm))
	{
		DbgPrint("WriteMemory successful\n");
	}
	else
	{
		DbgPrint("WriteMemory failed\n");
	}
	if (writeData)
	{
		ExFreePool(writeData);
	}
}

