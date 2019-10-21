#include "AutoRun.h"
#include "../String/UserString.h"

/*
	* ������ݷ�ʽ
	* szExePath[in]:Ҫ������ݷ�ʽ��exe�ļ�ȫ·��
	* szLinkName[in]:Ҫ�����Ŀ�ݷ�ʽ.link�ļ���ȫ·��
	* iIcon[in]:Ҫ������ݷ�ʽ��exe�ļ�RC��Դ�е�iconֵ��Ĭ��Ϊ0��
*/
BOOL PsCreateLink(LPCTSTR lpszExePath, CONST WCHAR* lpWzLinkPath, int iIcon)
{
	if (NULL == lpszExePath)
	{
		return ERROR_INVALID_PARAMETER;
	}

	DWORD dwRlt = ERROR_SUCCESS;
	HRESULT hres = S_OK;
	IShellLink* pShellLink = NULL;
	IPersistFile* pPersistFile = NULL;
	WCHAR szwShortCutName[MAX_PATH];
	try
	{
		::CoInitialize(NULL); // ��ʼ�� COM ��
		// ���� COM ���󲢻�ȡ��ʵ�ֵĽӿ�
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink);
		if (FAILED(hres))
		{
			throw(hres);
		}

		TCHAR szWorkPath[MAX_PATH] = { 0 };
		strcpy(szWorkPath, lpszExePath);
		LPTSTR pszEnd = _tcsrchr(szWorkPath, _T('\\'));
		if (pszEnd != NULL)
		{
			*pszEnd = NULL;
		}

		// ���ÿ�ݷ�ʽ�ĸ�������
		pShellLink->SetPath(lpszExePath); // ��ݷ�ʽ��ָ��Ӧ�ó�����
		pShellLink->SetDescription(_T("��ݷ�ʽ")); // ����
		pShellLink->SetWorkingDirectory(szWorkPath); // ���ù���Ŀ¼
		pShellLink->SetIconLocation(lpszExePath, iIcon);//ֱ��ȡexe�ļ��е�ͼ���������

		// ��ѯ IShellLink �ӿڴӶ��õ� IPersistFile �ӿ��������ݷ�ʽ
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);
		if (FAILED(hres))
		{
			throw(hres);
		}
		wsprintfW(szwShortCutName, L"%s", lpWzLinkPath);
		//ʹ�� IPersistFile �ӿڵ� Save() ���������ݷ�ʽ
		hres = pPersistFile->Save(szwShortCutName, TRUE);
	}
	catch (DWORD dwExpRlt)
	{
		dwRlt = dwExpRlt;
	}
	catch (...)
	{
	}

	if (pPersistFile != NULL) {
		pPersistFile->Release(); pPersistFile = NULL;
	}

	if (pShellLink != NULL) {
		pShellLink->Release(); pPersistFile = NULL;
	}
	CoUninitialize();
	return dwRlt;
}

/*
	* ���������ݷ�ʽ
	* szExePath[in]:Ҫ������ݷ�ʽ��exe�ļ�ȫ·��
	* szLinkName[in]:Ҫ�����Ŀ�ݷ�ʽ������
	* iIcon[in]:Ҫ������ݷ�ʽ��exe�ļ�RC��Դ�е�iconֵ��Ĭ��Ϊ0��
*/
void PsCreateDesktopLink(LPCTSTR szExePath, LPCTSTR szLinkName, int iIcon)
{
	TCHAR szDesktopPath[MAX_PATH] = { 0 };
	LPITEMIDLIST  ppidl = NULL;

	//��ȡ�����û���������ݷ�ʽ��·��
	HRESULT hSpecialPath = SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_DESKTOPDIRECTORY, &ppidl);
	if (hSpecialPath == S_OK)
	{
		BOOL flag = SHGetPathFromIDList(ppidl, szDesktopPath);
		CoTaskMemFree(ppidl);
	}
	else
	{
		//ʧ��
		return;
	}

	//���ÿ�ݷ�ʽ.link�ļ���ȫ·��
	TCHAR szExeLinkFullPath[MAX_PATH] = { 0 };
	wsprintf(szExeLinkFullPath, _T("%s\\%s.lnk"), szDesktopPath, szLinkName);
	std::wstring wzExeLinkFullPathStr = ubase::StrGbkToUnicode(szExeLinkFullPath);
	//��ʼ������ݷ�ʽ
	PsCreateLink(szExePath, wzExeLinkFullPathStr.c_str(), iIcon);
	// ֪ͨshell�йر仯
	SHChangeNotify(SHCNE_CREATE | SHCNE_INTERRUPT, SHCNF_FLUSH | SHCNF_PATH, szExeLinkFullPath, 0);
}

BOOL PsAutoRunStartup(CONST CHAR* szSrcFileFullPath, CONST CHAR* szLinkName)
{
	BOOL bRet = FALSE;
	CHAR szStartupPath[MAX_PATH] = { 0 };
	//��ȡ��������Ŀ¼��·��
	bRet = SHGetSpecialFolderPathA(NULL, szStartupPath, CSIDL_STARTUP, TRUE);
	if (bRet == FALSE)
	{
		return FALSE;
	}

	//���ÿ�ݷ�ʽ.link�ļ���ȫ·��
	TCHAR szLinkFullPath[MAX_PATH] = { 0 };
	wsprintf(szLinkFullPath, _T("%s\\%s.lnk"), szStartupPath, szLinkName);
	//��ȡ���ӵľ���·��(unicode)
	std::wstring wzExeLinkFullPath = ubase::StrGbkToUnicode(szLinkFullPath);
	PsCreateLink(szSrcFileFullPath, wzExeLinkFullPath.c_str(), 0);//�ڿ��������ļ����д�����ݷ�ʽ
	// ֪ͨshell�йر仯
	SHChangeNotify(SHCNE_CREATE | SHCNE_INTERRUPT, SHCNF_FLUSH | SHCNF_PATH, szLinkFullPath, 0);
	return TRUE;
}