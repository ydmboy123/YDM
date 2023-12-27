#pragma once
#include "definition.h"


extern "C"
typedef NTSTATUS(*pfnPspTerminateProcess)(PEPROCESS pEprocess, NTSTATUS ExitCode);
