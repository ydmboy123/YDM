#pragma once
//#include <wdm.h>
#include "definition.h"
//
extern "C"
NTSTATUS YCreateDevice(PDRIVER_OBJECT pd)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT dObj ;
	UNICODE_STRING unicode_str = { 0 };
	UNICODE_STRING unicode_symbolic = { 0 };
	RtlInitUnicodeString(&unicode_str, L"\\??\\ydmboy");
	RtlInitUnicodeString(&unicode_symbolic, SYS_SYMlINK);

	ntStatus = IoCreateDevice(pd, sizeof(pd->DriverExtension), &unicode_str, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN,FALSE,&dObj);
	if (ntStatus != STATUS_SUCCESS)
	{
		return ntStatus;
	}
	ntStatus = IoCreateSymbolicLink(&unicode_str,&unicode_symbolic);
	if (ntStatus != STATUS_SUCCESS)
	{
		return ntStatus;
	}
	else
	{
		if (dObj)
		{
			IoDeleteDevice(dObj);
		}
	}
	//IoCreateSymbolicLink
	return ntStatus;
	

}
