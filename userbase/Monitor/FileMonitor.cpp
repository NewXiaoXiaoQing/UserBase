#include "FileMonitor.h"

void W2C(wchar_t *pwszSrc, int iSrcLen, char *pszDest, int iDestLen)
{
	::RtlZeroMemory(pszDest, iDestLen);
	::WideCharToMultiByte(CP_ACP,
		0,
		pwszSrc,
		(iSrcLen / 2),
		pszDest,
		iDestLen,
		NULL,
		NULL);
}

void MonitorFileThread(LPVOID args)
{
	char* pszDirectory = (char*)args; //Ŀ¼��β������'\'��β,����������

	//��Ŀ¼��ȡ�ļ����
	HANDLE hDirectory = CreateFileA(pszDirectory, FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (hDirectory == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG
		MessageBoxA(NULL, "CreateFile Failed", "����", MB_OK);
#endif
		return;
	}

	char szTemp[MAX_PATH] = { 0 };
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	DWORD dwBufferSize = 4096;

	BYTE* pBuf = new BYTE[dwBufferSize];
	if (pBuf == NULL)
	{
#ifdef _DEBUG
		MessageBoxA(NULL, "malloc Failed", "����", MB_OK);
#endif
		return;
	}

	FILE_NOTIFY_INFORMATION *pFileNotifyInfo = (FILE_NOTIFY_INFORMATION *)pBuf;

	// ��ʼѭ�����ü��
	do
	{
		::RtlZeroMemory(pFileNotifyInfo, dwBufferSize);
		// ���ü��Ŀ¼
		bRet = ::ReadDirectoryChangesW(hDirectory,
			pFileNotifyInfo,
			dwBufferSize,
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME |			// �޸��ļ���
			FILE_NOTIFY_CHANGE_ATTRIBUTES |			// �޸��ļ�����
			FILE_NOTIFY_CHANGE_LAST_WRITE,			// ���һ��д��
			&dwRet,
			NULL,
			NULL);
		if (FALSE == bRet)
		{
#ifdef _DEBUG
			MessageBoxA(NULL, "ReadDirectoryChangesW Failed", "����", MB_OK);
#endif
			break;
		}
		//�����ַ�ת����խ�ַ�
		W2C((wchar_t *)(&pFileNotifyInfo->FileName), pFileNotifyInfo->FileNameLength, szTemp, MAX_PATH);
		//�ж��ļ��Ĳ�������(���кܶ�����,���Ը�����Ҫ�������)
		switch (pFileNotifyInfo->Action)
		{
		case FILE_ACTION_ADDED:
		{
			//�����ļ�
			break;
		}
		default:
		{
			break;
		}
		}


	} while (bRet);
	//�رվ��, �ͷ��ڴ�
	::CloseHandle(hDirectory);
	delete[] pBuf;
	pBuf = NULL;
}