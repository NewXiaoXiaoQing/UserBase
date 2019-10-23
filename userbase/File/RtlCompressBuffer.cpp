#include "RtlCompressBuffer.h"

void ShowRtlCompressBufferError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintfA(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBoxA(NULL, szErr, "ERROR", MB_OK);
#endif
}

// ����ѹ��
BOOL CompressData(BYTE *pUncompressData, DWORD dwUncompressDataLength, BYTE **ppCompressData, DWORD *pdwCompressDataLength)
{
	BOOL bRet = FALSE;
	NTSTATUS status = 0;
	HMODULE hModule = NULL;
	typedef_RtlGetCompressionWorkSpaceSize RtlGetCompressionWorkSpaceSize  = NULL;
	typedef_RtlCompressBuffer RtlCompressBuffer = NULL;
	DWORD dwWorkSpaceSize = 0, dwFragmentWorkSpaceSize = 0;
	BYTE *pWorkSpace = NULL;
	BYTE *pCompressData = NULL;
	DWORD dwCompressDataLength = 4096;
	DWORD dwFinalCompressSize = 0;
	do
	{
		// ���� ntdll.dll 
		hModule = ::LoadLibraryA("ntdll.dll");
		if (NULL == hModule)
		{
			ShowRtlCompressBufferError("LoadLibrary");
			break;
		}

		// ��ȡ RtlGetCompressionWorkSpaceSize ������ַ
		RtlGetCompressionWorkSpaceSize = (typedef_RtlGetCompressionWorkSpaceSize)::GetProcAddress(hModule, "RtlGetCompressionWorkSpaceSize");
		if (NULL == RtlGetCompressionWorkSpaceSize)
		{
			ShowRtlCompressBufferError("GetProcAddress");
			break;
		}

		// ��ȡ RtlCompressBuffer ������ַ
		RtlCompressBuffer = (typedef_RtlCompressBuffer)::GetProcAddress(hModule, "RtlCompressBuffer");
		if (NULL == RtlCompressBuffer)
		{
			ShowRtlCompressBufferError("GetProcAddress");
			break;
		}

		// ��ȡWorkSpqce��С
		status = RtlGetCompressionWorkSpaceSize(COMPRESSION_FORMAT_LZNT1 | COMPRESSION_ENGINE_STANDARD, &dwWorkSpaceSize, &dwFragmentWorkSpaceSize);
		if (0 != status)
		{
			ShowRtlCompressBufferError("RtlGetCompressionWorkSpaceSize");
			break;
		}

		// ���붯̬�ڴ�
		pWorkSpace = new BYTE[dwWorkSpaceSize];
		if (NULL == pWorkSpace)
		{
			ShowRtlCompressBufferError("new");
			break;
		}
		::RtlZeroMemory(pWorkSpace, dwWorkSpaceSize);
		
		while (TRUE)
		{
			// ���붯̬�ڴ�
			pCompressData = new BYTE[dwCompressDataLength];
			if (NULL == pCompressData)
			{
				ShowRtlCompressBufferError("new");
				break;
			}
			::RtlZeroMemory(pCompressData, dwCompressDataLength);

			// ����RtlCompressBufferѹ������
			RtlCompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, 4096, &dwFinalCompressSize, (PVOID)pWorkSpace);
			if (dwCompressDataLength < dwFinalCompressSize)
			{
				// �ͷ��ڴ�
				if (pCompressData)
				{
					delete[]pCompressData;
					pCompressData = NULL;
				}
				dwCompressDataLength = dwFinalCompressSize;
			}
			else
			{
				break;
			}
		}

		// ����
		*ppCompressData = pCompressData;
		*pdwCompressDataLength = dwFinalCompressSize;
		bRet = TRUE;

	} while(FALSE);

	// �ͷ�
	if (pWorkSpace)
	{
		delete[]pWorkSpace;
		pWorkSpace = NULL;
	}
	if (hModule)
	{
		::FreeLibrary(hModule);
	}

	return bRet;
}


// ���ݽ�ѹ��
BOOL UncompressData(BYTE *pCompressData, DWORD dwCompressDataLength, BYTE **ppUncompressData, DWORD *pdwUncompressDataLength)
{
	BOOL bRet = FALSE;
	HMODULE hModule = NULL;
	typedef_RtlDecompressBuffer RtlDecompressBuffer = NULL;
	BYTE *pUncompressData = NULL;
	DWORD dwUncompressDataLength = 4096;
	DWORD dwFinalUncompressSize = 0;
	do
	{
		// ���� ntdll.dll 
		hModule = ::LoadLibraryA("ntdll.dll");
		if (NULL == hModule)
		{
			ShowRtlCompressBufferError("LoadLibrary");
			break;
		}

		// ��ȡ RtlDecompressBuffer ������ַ
		RtlDecompressBuffer = (typedef_RtlDecompressBuffer)::GetProcAddress(hModule, "RtlDecompressBuffer");
		if (NULL == RtlDecompressBuffer)
		{
			ShowRtlCompressBufferError("GetProcAddress");
			break;
		}

		while (TRUE)
		{
			// ���붯̬�ڴ�
			pUncompressData = new BYTE[dwUncompressDataLength];
			if (NULL == pUncompressData)
			{
				ShowRtlCompressBufferError("new");
				break;
			}
			::RtlZeroMemory(pUncompressData, dwUncompressDataLength);

			// ����RtlCompressBufferѹ������
			RtlDecompressBuffer(COMPRESSION_FORMAT_LZNT1, pUncompressData, dwUncompressDataLength, pCompressData, dwCompressDataLength, &dwFinalUncompressSize);
			if (dwUncompressDataLength < dwFinalUncompressSize)
			{
				// �ͷ��ڴ�
				if (pUncompressData)
				{
					delete[]pUncompressData;
					pUncompressData = NULL;
				}
				dwUncompressDataLength = dwFinalUncompressSize;
			}
			else
			{
				break;
			}
		}

		// ����
		*ppUncompressData = pUncompressData;
		*pdwUncompressDataLength = dwFinalUncompressSize;
		bRet = TRUE;

	} while (FALSE);

	// �ͷ�
	if (hModule)
	{
		::FreeLibrary(hModule);
	}

	return bRet;
}
