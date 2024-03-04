#pragma once
#include "definition.h"


extern "C"
NTSTATUS YTerminateProcess(IN PDRIVER_OBJECT driverObject, UCHAR g_szSpecialCode[], ULONG32 sCodeLen);

extern "C" PVOID SearchFunction(PKLDR_DATA_TABLE_ENTRY pLdr, UCHAR* szSpecialCode, int offsetAddress);

