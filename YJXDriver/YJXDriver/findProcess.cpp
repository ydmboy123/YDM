#include "findProcess.h"

extern "C"
NTSTATUS FindProcessByNameC(const WCHAR* processName, PHANDLE processHandle) {
	PEPROCESS process;
	HANDLE processId = NULL;
	NTSTATUS status = STATUS_NOT_FOUND;

	DbgPrint("FindProcessnByNameC");
	// 获取系统进程信息列表


	for (ULONG i = 0; i<10000;i++)
	{
		// 获取进程名称
		if (STATUS_SUCCESS == PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)i, &process))
		{
			UNICODE_STRING processImageName = { 0 };
			ANSI_STRING ascII = { 0 };
			PCHAR pName = PsGetProcessImageFileName(process);
			DbgPrint("ImageName:%s", pName);

			RtlInitAnsiString(&ascII,pName);

			//RtlInitUnicodeString(&processImageName, &ascII);
			RtlAnsiStringToUnicodeString(&processImageName,&ascII,TRUE);

			DbgPrint("No%d:%wZ", i,processImageName);
			if (_wcsicmp(processImageName.Buffer, processName) == 0) {
				processId = (HANDLE)i;
				status = STATUS_SUCCESS;
				break;
			}
			ObDereferenceObject(process);
		}
	}

	if (NT_SUCCESS(status)) {
		// 找到进程后，获取进程句柄
		CLIENT_ID clientId;
		OBJECT_ATTRIBUTES objectAttributes;
		clientId.UniqueProcess = processId;
		clientId.UniqueThread = 0;

		InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
		status = ZwOpenProcess(processHandle, PROCESS_ALL_ACCESS, &objectAttributes, &clientId);
	}

	return status;
}
