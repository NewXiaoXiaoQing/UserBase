#ifndef _REG_AUTORUN_H_
#define _REG_AUTORUN_H_

/*
	��ע���ʽʵ���������ĸ��ַ���
*/

#include <Windows.h>

/*
	HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run
	//����Ҫ����Ա���
*/
BOOL RegAutoRunCurrentUser(char *lpszFileName, char *lpszValueName);

/*
	HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run
	//��Ҫ����Ա���
*/
BOOL RegAuotRunLocalMachine(char *lpszFileName, char *lpszValueName);


#endif