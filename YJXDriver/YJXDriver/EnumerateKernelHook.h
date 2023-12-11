#pragma once
#include "definition.h"
#include "EnumAllDriver.h"
extern "C"  PSHORT *NtBuildNumber;

//获取文档化的函数
extern "C"
PVOID GetUndocumentFunctionAddress(IN PUNICODE_STRING pFunName,
	IN PUCHAR pStartAddress,
	IN UCHAR* pFeatureCode,
	IN ULONG FeatureCodeNum,
	ULONG SerSize,
	UCHAR SegCode,
	ULONG AddNum,
	BOOLEAN ByName);

extern "C"
ULONG EnumObRegisterCallBack();


extern "C"
BOOLEAN ObGetDriverNameByPoint(ULONG_PTR Point, WCHAR* szDriverName);

extern "C"
PVOID GetPsLoadedListModule();


extern "C"
PVOID GetMovPoint(PVOID pCallPoint);

extern "C"
BOOLEAN GetVersionAndHardCode();

extern "C"
BOOLEAN ObGetCallBacksAltitude(WCHAR* szDriverName, PUNICODE_STRING usAltitudeString, BOOLEAN bGetProcess);


