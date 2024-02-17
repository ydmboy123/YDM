#include "YTerminateProcess.h"


typedef NTSTATUS(*pfnPspTerminateProcess)(PEPROCESS pEprocess, NTSTATUS ExitCode);
	//UCHAR g_szSpecialCode[] ={ 0x41,0x8b,0xe8,0x4c,0x8b,0xfa,0x48,0x8b,0xd9,0x0f,0x0d,0x89,0x04,0x03,0x00,0x00,0x8b,0x81,0x04,0x03,0x00,0x00,0x44,0x8b,0xd0,0x41,0x83,0xca,0x08,0xf0,0x44,0x0f,0xb1,0x91,0x04,0x03,0x00,0x00 };

ULONG g_uPID = 468; // 要关闭的进程PID
//UINT32 g_uSpecialCodeLen = sizeof(g_szSpecialCode);


extern "C"
NTSTATUS YTerminateProcess(IN PDRIVER_OBJECT driverObject,UCHAR g_szSpecialCode[],ULONG32 sCodeLen)
{
	//DbgPrint("In YTerminateProcess \n");
	//DbgPrint("sizeOf%d", sizeof(g_szSpecialCode));
	//for (int i = 0; i < sizeof(g_szSpecialCode); i++)
	//{
	//	DbgPrint("%x",g_szSpecialCode[i]);
	//}

	NTSTATUS status = STATUS_SUCCESS;
	PKLDR_DATA_TABLE_ENTRY pLdrDataTableEntry = NULL;
	pfnPspTerminateProcess pPspTerminateProcess = NULL;
	PEPROCESS pEprocess = NULL;


	pLdrDataTableEntry = (PKLDR_DATA_TABLE_ENTRY)driverObject->DriverSection;
	pLdrDataTableEntry = CONTAINING_RECORD(pLdrDataTableEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

	//DbgPrint("DllBaseAddr:%llx",pLdrDataTableEntry->DllBase);
	//DbgPrint("驱动加载完成\r\n");

	pLdrDataTableEntry = (PKLDR_DATA_TABLE_ENTRY)pLdrDataTableEntry->InLoadOrderLinks.Flink;

	do
	{
		pLdrDataTableEntry = CONTAINING_RECORD(pLdrDataTableEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		// 如果存在则搜索特征码
		if (pLdrDataTableEntry->DllBase)
		{
			pPspTerminateProcess = (pfnPspTerminateProcess)
				SearchFunction(pLdrDataTableEntry,g_szSpecialCode,sCodeLen);
		}

		if (pPspTerminateProcess)
			break;

		// 遍历
		pLdrDataTableEntry = (PKLDR_DATA_TABLE_ENTRY)pLdrDataTableEntry->InLoadOrderLinks.Flink;

	} while ((UINT32)pLdrDataTableEntry != (UINT32)driverObject->DriverSection);

	if (pPspTerminateProcess)
	{
		status = PsLookupProcessByProcessId((HANDLE)g_uPID, &pEprocess);
		if (NT_SUCCESS(status))
		{
			status = pPspTerminateProcess(pEprocess, 0);

			if (NT_SUCCESS(status))
			{
				DbgPrint("使用 PspTerminateProcess 关闭进程成功, PID = %d\r\n", g_uPID);
			}
		}
		else
		{
			DbgPrint("PsLookupProcessByProcessId Error 0x%X\r\n", status);
		}
	}

	return STATUS_SUCCESS;





	//pLdrDataTableEntry->InLoadOrderLinks;
	





	return STATUS_SUCCESS;

}


extern "C" PVOID SearchFunction(PKLDR_DATA_TABLE_ENTRY pLdr, UCHAR* szSpecialCode, int offsetAddress)
{
	PUCHAR DllBase = (PUCHAR)pLdr->DllBase;
	UINT32 SizeOfImage = pLdr->SizeOfImage;

	//DbgPrint("%s",szSpecialCode);
	//return STATUS_SUCCESS;
    PVOID pFuncAddr = NULL;
    UINT32 uEnd = (UINT32)DllBase + SizeOfImage - sizeof(szSpecialCode);    // 减去特征码后的长度
    UINT32 i = 0;
    BOOLEAN bOk = TRUE;

    while ((UINT32)DllBase <= uEnd)
    {
        bOk = TRUE;
        for (i = 0; i <sizeof(szSpecialCode) ; i++)
        {
            if (!MmIsAddressValid(&DllBase[i]) || DllBase[i] != szSpecialCode[i])
            //if (!MmIsAddressValid(&DllBase[i]) || DllBase[i] != g_szSpecialCode[i])
            {
                bOk = FALSE;
                break;
            }
        }

        if (bOk)
        {
            pFuncAddr = (PVOID)(DllBase - 27);
            DbgPrint("找到特征码，内存地址为%p\r\n", pFuncAddr);
            break;
        }
        DllBase++;
    }

    return pFuncAddr;
}
