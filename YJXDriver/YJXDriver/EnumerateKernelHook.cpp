#include "EnumerateKernelHook.h"

static ULONG ObjectCallbackListOffset = 0;
extern PSHORT NtBuildNumber;

PVOID GetMovPoint(PVOID pCallPoint);

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

ULONG EnumObRegisterCallBack();

typedef struct _LDR_DATA
{
	struct _LIST_ENTRY InLoadOrderLinks;
	struct _LIST_ENTRY InMemoryOrderLinks;
	struct _LIST_ENTRY InInitializeationOrderLinks;
	VOID* DllBase;
	VOID* EntryPoint;
	ULONG32 SizeImage;
	UINT8 _PADDING0_[0x4];
	struct _UNICODE_STRING FullDllName;
	struct _UNICODE_STRING BaseDllname;
	ULONG32 Flags;
	UINT16 LoadCount;

};

extern "C"
BOOLEAN ObGetDriverNameByPoint(ULONG_PTR Point, WCHAR* szDriverName)
{
	PKLDR_DATA_TABLE_ENTRY pBegin = NULL;
	PLIST_ENTRY64 Head = NULL;
	PLIST_ENTRY64 Next = NULL;
	pBegin = (PKLDR_DATA_TABLE_ENTRY)GetPsLoadedListModule();
	if (!pBegin)
	{
		return false;
	}
	Head = (PLIST_ENTRY64)&pBegin->InLoadOrderLinks.Flink;
	Next = (PLIST_ENTRY64)&Head->Flink;
	do
	{
		PKLDR_DATA_TABLE_ENTRY Entry = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		Next = (PLIST_ENTRY64)&Next->Flink;
		if ((ULONG_PTR)Entry->DllBase <= Point && Point <= ((ULONG_PTR)Entry->DllBase + Entry->SizeOfImage))
		{
			if (szDriverName == NULL)
			{
				return FALSE;

			}
			RtlZeroMemory(szDriverName, 600);
			RtlCopyMemory(szDriverName, Entry->BaseDllName.Buffer, Entry->BaseDllName.Length);

		}
	}while (Next != (PLIST_ENTRY64)&Next->Flink);


	
}

extern "C"
PVOID GetPsLoadedListModule()
{
	UNICODE_STRING usRtlPcToFileHeader = RTL_CONSTANT_STRING(L"RtlPcToFileHeader");
	UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
	PVOID Point = NULL;
	static PVOID PsLoadedListModule = NULL;
	UCHAR shellCode[] = "\x48\x8b\x0d\x60\x60\x60\x60\x48\x85\xc9";
	if (PsLoadedListModule)
		return PsLoadedListModule;
	/* 判断是否是win10以上版本*/
		
	if (*NtBuildNumber > 9600)
	{
		PsLoadedListModule = MmGetSystemRoutineAddress(&usPsLoadedModuleList);
		return PsLoadedListModule;
	}
	Point = GetUndocumentFunctionAddress(&usRtlPcToFileHeader,NULL,shellCode,10,0xff,0x60,0,TRUE);
	if (Point == NULL || !MmIsAddressValid(Point))
	{
		return NULL;
	}
	
	Point = GetMovPoint(Point);
	if (Point == NULL || !MmIsAddressValid(Point))
	{
		return NULL;
	}
	PsLoadedListModule = Point;
	return PsLoadedListModule;
}

extern "C"
PVOID GetUndocumentFunctionAddress(IN PUNICODE_STRING pFunName,
	IN PUCHAR pStartAddress,
	IN UCHAR* pFeatureCode,
	IN ULONG FeatureCodeNum,
	ULONG SerSize,
	UCHAR SegCode,
	ULONG AddNum,
	BOOLEAN ByName)
{
	ULONG dwIndex = 0;
	PUCHAR pFunAddress = NULL;
	ULONG dwCodeNum = 0;
	if (pFeatureCode == NULL || FeatureCodeNum >=15 ||SerSize>=0x1024)
	{
		return NULL;
	}
	if (ByName)
	{
		if (pFunName == NULL || MmIsAddressValid(pStartAddress))
		{
			pFunAddress = (PUCHAR)MmGetSystemRoutineAddress(pFunName);
			if (pFunAddress == NULL)
			{
				return NULL;
			}
		}
		else
		{
			if (pStartAddress == NULL || MmIsAddressValid(pStartAddress))
			{
				return NULL;

			}
			pFunAddress = pStartAddress;
		}
	}
	for (dwIndex = 0; dwIndex < SerSize; dwIndex++)
	{
		__try
		{
			if (pFunAddress[dwIndex] == pFeatureCode[dwCodeNum] || pFeatureCode[dwCodeNum] == SegCode)
			{
				dwCodeNum++;
				if (dwCodeNum == FeatureCodeNum)
				{
					return pFunAddress + dwIndex - dwCodeNum + 1 + AddNum;
				}
			}

		}
		__except(EXCEPTION_EXECUTE_HANDLER) 
		{
			return 0;
		}
	}
}

