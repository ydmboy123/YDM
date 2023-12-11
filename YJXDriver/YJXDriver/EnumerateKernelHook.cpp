#include "EnumerateKernelHook.h"
#include <ntifs.h>
#include <windef.h>
#
static ULONG ObjectCallbackListOffset = 0;
//__declspec(dllimport) PSHORT NtBuildNumber;
//extern PSHORT NtBuildNumber;





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
	
	// 
	//EnumDriverByPKLDR(pBegin);
	//return FALSE;

	
	if (!pBegin)
	{
		return false;
	}
	Head = (PLIST_ENTRY64)pBegin->InLoadOrderLinks.Flink;
	Next = (PLIST_ENTRY64)Head->Flink;
	do
	{
		PKLDR_DATA_TABLE_ENTRY Entry = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		Next = (PLIST_ENTRY64)Next->Flink;
		if ((ULONG_PTR)Entry->DllBase <= Point && Point <= ((ULONG_PTR)Entry->DllBase + Entry->SizeOfImage))
		{
			if (szDriverName == NULL)
			{
				return FALSE;
			}
			RtlZeroMemory(szDriverName, 600);
			RtlCopyMemory(szDriverName, Entry->BaseDllName.Buffer, Entry->BaseDllName.Length);
			return TRUE;

		}
	}while (Next != (PLIST_ENTRY64)Next->Flink); 



	return FALSE;
}

extern "C"
PVOID GetPsLoadedListModule()
{
	// 通过特征码定位函数
	UNICODE_STRING usRtlPcToFileHeader = RTL_CONSTANT_STRING(L"RtlPcToFileHeader");
	UNICODE_STRING usPsLoadedModuleList = RTL_CONSTANT_STRING(L"PsLoadedModuleList");
	PVOID Point = NULL;
	static PVOID PsLoadedListModule = NULL;
	UCHAR shellCode[] = "\x48\x8b\x0d\x60\x60\x60\x60\x48\x85\xc9";
	if (PsLoadedListModule)
		return PsLoadedListModule;
	/* 判断是否是win10以上版本*/
		
	if ((*(SHORT*)NtBuildNumber) > (SHORT)9600)
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



extern "C"
PVOID GetMovPoint(PVOID pCallPoint)
{
	ULONG dwOffset = 0;
	ULONG_PTR returnAddress = 0;
	LARGE_INTEGER returnAddressTemp = { 0 };
	PUCHAR pFunAddress = NULL;
	if (pCallPoint == NULL || !MmIsAddressValid(pCallPoint))
	{
		return NULL;
	}
	pFunAddress = (PUCHAR)pCallPoint;
	RtlCopyMemory(&dwOffset, (PVOID)(pFunAddress + 3), sizeof(ULONG));
	if((dwOffset&0x10000000) == 0x10000000)
	{
		dwOffset = dwOffset + 7 + (ULONG)pFunAddress;
		returnAddressTemp.QuadPart = (ULONG_PTR)pFunAddress & 0xFFFFFFFF00000000;
		returnAddressTemp.LowPart = dwOffset;
		returnAddress = returnAddressTemp.QuadPart;
		return (PVOID)returnAddress;
	}
	returnAddress = (ULONG_PTR)(dwOffset + 7 + pFunAddress);
	return (PVOID)returnAddress;
}

extern "C"
ULONG EnumObRegisterCallBack()
{
	ULONG c = 0;
	PLIST_ENTRY CurrentEntry = NULL;
	POB_CALLBACK pObCallback;
	ULONG64 ObProcessCallbackListHead = 0;
	ULONG64 ObThreadCallbackListHead = 0;
	WCHAR* szDriverBaseName = NULL;
	szDriverBaseName = (WCHAR*) ExAllocatePool(NonPagedPool,600);
	if (szDriverBaseName == NULL)
	{
		return FALSE;
	}
	RtlZeroMemory(szDriverBaseName,600);
	GetVersionAndHardCode();
	ObProcessCallbackListHead = *(ULONG64*)PsProcessType + ObjectCallbackListOffset;
	ObThreadCallbackListHead = *(ULONG64*)PsThreadType + ObjectCallbackListOffset;
	CurrentEntry = ((PLIST_ENTRY)ObProcessCallbackListHead)->Flink;
	if (CurrentEntry == NULL || !MmIsAddressValid(CurrentEntry))
	{
		ExFreePool(szDriverBaseName);
		return 0;
	}
	do
	{
		pObCallback = (POB_CALLBACK)CurrentEntry;
		if (pObCallback->ObHandle != 0)
		{
			if (ObGetDriverNameByPoint(pObCallback->PreCall, szDriverBaseName))
			{
				DbgPrint("yjx:Sys->DriverName=%S OBHandle=%p Index=%wZ PreCall=%p PostCall=%p",szDriverBaseName,pObCallback->ObHandle,&pObCallback->ObHandle->AltitudeString,pObCallback->PreCall,pObCallback->PostCall);
				c++;
			}
		}
		CurrentEntry = CurrentEntry->Flink;
	} while (CurrentEntry != (PLIST_ENTRY)ObProcessCallbackListHead);
	DbgPrint("yjx:sys->线程对象回调\n");
	CurrentEntry = ((PLIST_ENTRY)ObThreadCallbackListHead)->Flink;
	if (CurrentEntry == NULL || !MmIsAddressValid(CurrentEntry))
	{
		ExFreePool(szDriverBaseName);
		return c;
	}
	do
	{
		pObCallback = (POB_CALLBACK)CurrentEntry;
		if (pObCallback->ObHandle != 0)
		{
			if (ObGetDriverNameByPoint(pObCallback->PreCall, szDriverBaseName))
			{
				DbgPrint("yjx:Sys->DriverName=%S OBHandle=%p Index=%wZ PreCall=%p PostCall=%p", szDriverBaseName, pObCallback->ObHandle, &pObCallback->ObHandle->AltitudeString, pObCallback->PreCall, pObCallback->PostCall);
				c++;
			}
		}
		CurrentEntry = CurrentEntry->Flink;
	} while (CurrentEntry != (PLIST_ENTRY)ObThreadCallbackListHead);
	DbgPrint("yjx:SysObCallback Count:%ld\n",c);
	ExFreePool(szDriverBaseName);
	return c;


}

extern "C"
BOOLEAN GetVersionAndHardCode()
{
	BOOLEAN b = FALSE;
	switch (*(SHORT*) NtBuildNumber)
	{
	case 7600:
		break;
	case 7601:
	{
		ObjectCallbackListOffset = 0xC0;
	}
	case 9200:
	{
		ObjectCallbackListOffset = 0xC8;
		b = TRUE;
		break;
	}
	case 9600:
	{
		ObjectCallbackListOffset = 0xC8;
		b = TRUE;
		break;
	}

	default:
	{
		if (*(SHORT*)NtBuildNumber > 10000)
		{
			ObjectCallbackListOffset = 0xc8;
			b = TRUE;
		}
		break;
	}
	}
	return b;

}

extern "C"
BOOLEAN ObGetCallBacksAltitude(WCHAR* szDriverName, PUNICODE_STRING usAltitudeString,BOOLEAN bGetProcess)
{
	BOOLEAN bRet = FALSE;
	PLIST_ENTRY CurrEntry = NULL;
	POB_CALLBACK pObCallback;
	ULONG_PTR ObCallbackListHead = 0;
	WCHAR* szDriverBaseName = NULL;
	GetVersionAndHardCode();
	if (bGetProcess)
		ObCallbackListHead = (*(ULONG_PTR*)PsProcessType) + ObjectCallbackListOffset;
	else
		ObCallbackListHead = (*(ULONG_PTR*)PsThreadType) + ObjectCallbackListOffset;
	CurrEntry = ((PLIST_ENTRY)ObCallbackListHead)->Flink;
	if (CurrEntry == NULL || !MmIsAddressValid(CurrEntry))
	{
		return bRet;
	}
	if (szDriverName == NULL || usAltitudeString == NULL || usAltitudeString->Buffer == NULL)
	{
		return FALSE;
	}
	szDriverBaseName = (WCHAR*)ExAllocatePool(NonPagedPool,600);

	if (szDriverBaseName == NULL)
	{
		return FALSE;
	}
	RtlZeroMemory(szDriverBaseName,600);
	do
	{
		pObCallback = (POB_CALLBACK)CurrEntry;
		if (pObCallback->ObHandle != 0)
		{
			DbgPrint("yjx:SysObHandler: &p\n",pObCallback->ObHandle);
			DbgPrint("yjx:SysIndex:%wZ \n",&pObCallback->ObHandle->AltitudeString);
			DbgPrint("yjx:SysPreCall:%p\n",pObCallback->PreCall);
			DbgPrint("yjx:SysPostCall:%p\n",pObCallback->PostCall);
			if (!_wcsnicmp(szDriverBaseName, szDriverName,wcslen(szDriverName) * 2))
			{
				bRet = TRUE;
	/*			RtlCopyMemory(usAltitudeString->Buffer);*/
				RtlCopyMemory(usAltitudeString->Buffer, pObCallback->ObHandle->AltitudeString.Buffer, pObCallback->ObHandle->AltitudeString.Length);
				usAltitudeString->Length = pObCallback->ObHandle->AltitudeString.Length;
				usAltitudeString->MaximumLength = 600;
				break;
			}
		}
		CurrEntry = CurrEntry->Flink;
	} while (CurrEntry != (PLIST_ENTRY)ObCallbackListHead);
	ExFreePool(szDriverBaseName);
}



