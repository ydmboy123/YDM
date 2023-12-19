#pragma once
#include "definition.h"

extern "C"
VOID EnumDriver(PDRIVER_OBJECT pDriverObject);


extern "C"
VOID EnumDriverByPKLDR(PKLDR_DATA_TABLE_ENTRY entry);

extern "C"
VOID EnumCallBack();
