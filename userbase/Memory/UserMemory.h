#pragma once
#include <windows.h>
#include <process.h>
#include <Psapi.h>

namespace ubase
{
	MODULEINFO MmGetModuleInfo(char *szModule);//��ȡ���̶�Ӧģ�����Ϣ
	DWORD MmFindPattern(char *module, char *pattern, char *mask);//���ҷ��϶�Ӧģʽ���ڴ��ַ������:mask XXX?XXX??XX����?����ģ��ƥ�䣩
	void MmWriteToMemory(uintptr_t addressToWrite, char* valueToWrite, int byteNum);//д�뵱ǰ���̿ռ��ڴ�
	void MmReadFromMemory(uintptr_t addressToRead, char* varToWriteTo, int byteNum);//��ȡ��ǰ���̿ռ��ڴ�
	ULONG_PTR MmFindImageBase(HANDLE hProc, LPSTR lpCommandLine);//��ָ���������ҵ���Ӧģ��Ļ���ַ(�ܹ��ڽ��̸մ�����û���������߳�֮ǰ����(����ֻ��ntdll��exeģ�����ʱ))
}
