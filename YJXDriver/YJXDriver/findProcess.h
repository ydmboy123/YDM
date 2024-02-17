#pragma once
#include "definition.h"


// reutnr to the process by open ,It would be closed;
extern "C"
NTSTATUS FindProcessByNameC(const WCHAR* processName, PHANDLE processHandle);
