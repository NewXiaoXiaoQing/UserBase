#pragma once
#include <ShlObj.h>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>


BOOL PsCreateOneLink(LPCTSTR  lpszExePath, LPCTSTR lpszLnkPath, int iIcon); //����һ����ݷ�ʽ
void PsCreateDesktopLink(LPCTSTR szExePath, LPCTSTR szLinkName, int iIcon);	//���������ݷ�ʽ