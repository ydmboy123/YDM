#pragma once
#include <ntifs.h>
#include <wdm.h>
#include <windef.h>
#define SYS_SYMlINK L"\\??\\DEVICE\\ydmboy"
typedef struct _CALLBACK_ENTRY CALLBACK_ENTRY;
typedef struct _CALLBACK_ENTRY_ITEM CALLBACK_ENTRY_ITEM;
#define EX_PUSH_LOCK ULONG_PTR



#define ADDRESS  0x00007FF72375C000
#define PID  8228
#define WRITE_DATA 
#define READ_DATA

typedef struct _KLDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	ULONG64 __Undefined1;
	ULONG64 __Undefined2;
	ULONG64 __Undefined3;
	ULONG64 NonPagedDebugInfo;
	ULONG64 DllBase;
	ULONG64 EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG   Flags;
	USHORT  LoadCount;
	USHORT  __Undefined5;
	ULONG64 __Undefined6;
	ULONG   CheckSum;
	ULONG   __padding1;
	ULONG   TimeDateStamp;
	ULONG   __padding2;
}KLDR_DATA_TABLE_ENTRY,LDR_DATA_TABLE_ENTRY,*PKLDR_DATA_TABLE_ENTRY;


typedef struct _OBJECT_TYPE {
	LIST_ENTRY TypeList;
	UNICODE_STRING Name;
	void* DefaultObject;
	unsigned __int8 Index;
	unsigned int TotalNumberOfObjects;
	unsigned int TotalNumberOfHandles;
	unsigned int HighWaterNumberOfObjects;
	unsigned int HighWaterNumberOfHandles;
	//_OBJECT_TYPE_INITIALIZER TypeInfo;
	unsigned __int8 Placeholder[0x78];
	EX_PUSH_LOCK TypeLock;
	unsigned int Key;
	LIST_ENTRY CallbackList;
}OBJECT_TYPE;
typedef struct _CALLBACK_ENTRY_ITEM {
	LIST_ENTRY EntryItemList;
	OB_OPERATION Operations;
	CALLBACK_ENTRY* CallbackEntry;
	POBJECT_TYPE ObjectType;
	POB_PRE_OPERATION_CALLBACK PreOperation;
	POB_POST_OPERATION_CALLBACK PostOperation;
	__int64 unk;
}CALLBACK_ENTRY_ITEM;

typedef struct _CALLBACK_ENTRY {
	__int16 Version;
	char buffer1[6];
	POB_OPERATION_REGISTRATION RegistrationContext;
	__int16 AltitudeLength1;
	__int16 AltitudeLength2;
	char buffer2[4];
	WCHAR* AltitudeString;
	CALLBACK_ENTRY_ITEM Items;
}CALLBACK_ENTRY;




