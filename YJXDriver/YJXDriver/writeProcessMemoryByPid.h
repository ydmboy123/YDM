#pragma once
#include "definition.h"

typedef struct
{
	DWORD pid;
	DWORD64 addres;;
	DWORD size;
	BYTE* data;
}writeMemoryStruct;

extern "C"
BOOL writeMemory(writeMemoryStruct* data);

extern "C"
void WriteMemoryDbgPrint();
