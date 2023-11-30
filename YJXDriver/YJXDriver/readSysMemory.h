#pragma once

#include "definition.h"
#include <windef.h>


typedef struct __ReadMemoryStruct
{
	DWORD pid;
	DWORD64 address;
	DWORD size;
	BYTE* data;
}ReadMemoryStruct;

extern "C"
int readProcessMemoryByPid(ReadMemoryStruct * data);

extern "C"
void readMemoryPrint();

