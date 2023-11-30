#pragma once
#include "HookByRegisterCallbacks.h"
#include "definition.h"

#define PROCESS_TERMINATE (0x0001)
#define PROCESS_CREATE_THREAD (0x0002)
#define PROCESS_SET_SESSIONID (0x0004)
#define PROCESS_VM_PERATION (0x0008)
#define PROCESS_VM_READ (0x0010)
#define PROCESS_VM_WRITE (0x0020)
#define PROCESS_DUP_HANDLE (0x0040)
#define PROCESS_CREATE_PROCESS (0x0080)
#define PROCESS_SET_QUOTA (0x0100)
#define PROCESS_SET_INFORMATION (0x0200)
#define PROCESS_QUERY_INFORMATION (0x0400)
#define PROCESS_SUSPEND_RESUME (0x0800)
#define PROCESS_QUERY_LIMITED_INFORMATION (0x1000)
#define PROCESS_SET_LIMITED_INFORMATION (0x2000)

//HANDLE gs_handleCallback = NULL;

//
//typedef struct _OB_CALLBACK_REGISTRATION
//{
//	USHORT Version;
//	USHORT OperationRegistrationCount;
//	UNICODE_STRING Altitude;
//	PVOID RegistrationContext;
//	OB_OPERATION_REGISTRATION *OperationRegistration;
//
//}OB_CALLBACK_REGISTRATION,*POB_CALLBACK_REGISTRATION;


extern "C"
OB_PREOP_CALLBACK_STATUS my_pre_callback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);

extern "C"
void setMemoryProtecte();

extern "C"
void UnistallMemoryProctect();

extern "C"
NTSTATUS UnistallAllProcessType();

extern "C"
bool GetProcessPsType(HANDLE hd);
