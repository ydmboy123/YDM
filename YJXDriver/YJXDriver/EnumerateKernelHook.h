#pragma once
#include "definition.h"
#include "EnumerateKernelHook.h"

extern "C"
BOOLEAN ObGetDriverNameByPoint(ULONG_PTR Point, WCHAR* szDriverName);

extern "C"
PVOID GetPsLoadedListModule();

