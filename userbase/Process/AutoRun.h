#pragma once
#include <ShlObj.h>
#include <tchar.h>
#include <windows.h>


BOOL PsCreateLink(LPCTSTR  lpszExePath, CONST WCHAR* lpWzLinkPath, int iIcon = 0); //����һ����ݷ�ʽ
void PsCreateDesktopLink(LPCTSTR szExePath, LPCTSTR szLinkName, int iIcon = 0);	//���������ݷ�ʽ
BOOL PsAutoRunStartup(CONST CHAR* szSrcFilePath, CONST CHAR* szLinkName);//��Startup�ļ�����ӿ�ݷ�ʽʵ�ֳ��򿪻�����