#include "readSysMemory.h"


extern "C"
int readProcessMemoryByPid(ReadMemoryStruct * data)
{
	PEPROCESS rProcess = NULL;
	if ( STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)data->pid,&rProcess) && rProcess)
	{
		PVOID getData = NULL;
		__try
		{
			getData = ExAllocatePool(PagedPool,data->size);
		}
		__except (1)
		{
			return FALSE;
		}
		KAPC_STATE stack = { 0 };
		KeStackAttachProcess((PRKPROCESS)rProcess,&stack);
		__try
		{
			ProbeForRead((VOID*)data->address, data->size, 1);
			RtlCopyMemory(getData,(VOID *)data->address,data->size);
		}
		__except (1)
		{
			return FALSE;
		}
		ObDereferenceObject(rProcess);
		KeUnstackDetachProcess(&stack);
		RtlCopyMemory(data->data,getData,data->size);
		ExFreePool(getData);
	}
	return true;
}

extern "C"
void readMemoryPrint()
{
	ReadMemoryStruct ptr = { 0 };
	ptr.pid = PID;
	ptr.address = ADDRESS;
	ptr.size = 100;
	ptr.data = (BYTE*)ExAllocatePool(PagedPool, ptr.size);
	
	readProcessMemoryByPid(&ptr);

	for (size_t i = 0; i < 100; i++)
	{
		DbgPrint("%x",ptr.data[i]);
	}
}

