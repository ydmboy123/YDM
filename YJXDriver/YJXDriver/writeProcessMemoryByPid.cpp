#include "writeProcessMemoryByPid.h"

extern "C"
BOOL writeMemory(writeMemoryStruct* data)
{
	PEPROCESS process = NULL;
	if (!PsLookupProcessByProcessId((HANDLE)data->pid, &process))
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
	KAPC_STATE stack = { 0 };
	KeStackAttachProcess(process, &stack);
	PMDL mdl = IoAllocateMdl((PVOID)data->addres, data->size, 0, 0, NULL);
	if (!mdl)
	{
		if (getData)
		{
			ExFreePool(getData);
		}
		return FALSE;
	}
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
	RtlCopyMemory(writeData, dd, 10);
	if (writeMemory(&wm))
	{
		DbgPrint("WriteMemory successful\n");
	}
	else
	{
		DbgPrint("WriteMemory failed\n");
	}
}

