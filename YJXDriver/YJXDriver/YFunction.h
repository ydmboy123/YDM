#pragma once
#include "definition.h"
extern "C"
NTSTATUS YCreateRoutine(PDEVICE_OBJECT pObj, PIRP pIrp)
{
	int i = FILE_DEVICE_UNKNOWN;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	DbgPrint("YCreateRoutine \n");
	pIrp->IoStatus.Status = ntStatus;
	pIrp->IoStatus.Information = 0;
	IofCompleteRequest(pIrp, IO_NO_INCREMENT); 
	return ntStatus;
}
