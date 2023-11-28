//#include "YCreateDevice.h"


//extern "C"
//NTSTATUS YCreateDevice(PDRIVER_OBJECT pd)
//{
//	NTSTATUS ntStatus = STATUS_SUCCESS;
//	PDEVICE_OBJECT dObj ;
//	UNICODE_STRING unicode_str = { 0 };
//
//	int i = FILE_DEVICE_UNKNOW;
//	RtlInitUnicodeString(&unicode_str, L"\\DEVICE\\ydmboy");
//
//	IoCreateDevice(pd, sizeof(pd->DriverExtension), &unicode_str, 0x00000022, FILE_DEVICE_SECURE_OPEN,FALSE,&dObj);
//	//IoCreateSymbolicLink
//	return ntStatus;
//	
//
//}
