#ifndef _APC_INJECT_H_
#define _APC_INJECT_H_


#include <Windows.h>
#include <TlHelp32.h>

//APCע��������Ŀ����̵�API����,���ͨ���Բ���,�󲿷�����²�����ʹ��
BOOL ApcInjectDll(char *pszProcessName, char *pszDllName);// APCע��

#endif