#include "RegAutoRun.h"

BOOL RegAutoRunCurrentUser(char *lpszFileName, char *lpszValueName)
{
	//Ĭ��Ȩ��
	HKEY hKey;
	//��ע����
	if (ERROR_SUCCESS != ::RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey))
	{
		return FALSE;
	}
	//�޸�ע���ֵ��ʵ�ֿ�������
	if (ERROR_SUCCESS != ::RegSetValueExA(hKey, lpszValueName, 0, REG_SZ, (BYTE *)lpszFileName, (1 + ::lstrlen(lpszFileName))))
	{
		::RegCloseKey(hKey);
		return FALSE;
	}
	//�ر�ע����
	::RegCloseKey(hKey);

	return TRUE;
}


BOOL RegAuotRunLocalMachine(char *lpszFileName, char *lpszValueName)
{
	//����ԱȨ��
	HKEY hKey;
	//��ע����
	if (ERROR_SUCCESS != ::RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey))
	{
		return FALSE;
	}
	//�޸�ע���ֵ��ʵ�ֿ�������
	if (ERROR_SUCCESS != ::RegSetValueExA(hKey, lpszValueName, 0, REG_SZ, (BYTE *)lpszFileName, (1 + ::lstrlen(lpszFileName))))
	{
		::RegCloseKey(hKey);
		return FALSE;
	}
	//�ر�ע����
	::RegCloseKey(hKey);

	return TRUE;
}