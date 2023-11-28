#pragma once
#include "definition.h"

extern "C" NTSTATUS YUnloadDelete(PDRIVER_OBJECT pdj)
{

	NTSTATUS ntStatus = STATUS_SUCCESS;
	UNICODE_STRING symStrUnicode = { 0 };
	RtlInitUnicodeString(&symStrUnicode, SYS_SYMlINK);
	IoDeleteSymbolicLink(&symStrUnicode);
	while (pdj->DeviceObject)
	{
		IoDeleteDevice(pdj->DeviceObject);
	}
	return ntStatus;
}