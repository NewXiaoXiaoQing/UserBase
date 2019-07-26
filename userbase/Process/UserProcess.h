#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <process.h>
#include <TlHelp32.h>

namespace ubase
{
	bool EnableDebugPriv();//������ǰ����Ȩ�޺���("SeDebugPrivilege"����д����Ȩ��)
	bool EnableShutdownPriv();//������ǰ����Ȩ�޺���("SeShutdownPrivilege"�ػ�Ȩ��)
	bool EnableBackupPriv();//������ǰ����Ȩ�޺���("SeBackupPrivilege"��������Ȩ��)
	bool EnableRestorePriv();//������ǰ����Ȩ�޺���("SeRestorePrivilege"�ָ�����Ȩ��)
	bool PsGetPidsByProcessName(std::vector<DWORD> &vDwPids, const std::string &processName);//ͨ���������ƻ�ȡ����pid,�����������ִ�Сд
	bool PsGetAllProcessName(std::vector<std::string> &vsProcessName);//��ȡ���еĽ��̵�����
	bool RemoteThreadInjectDll(DWORD dwPid, char* szPath);//Զ���߳�ע��dll
	HMODULE FindRemoteMod(HANDLE hProcess, const wchar_t * pszModShortName);//��ȡָ�������е�ģ����
	bool RemoteThreadUnloadDll(DWORD dwPid, wchar_t *dllName);//Զ���߳�ж��dll		  
	bool ZwCreateThreadExInjectDll(DWORD dwProcessId, char *pszDllFileName);//ʹ��ZwCreateThreadExʵ��Զ�߳�ע��(ͻ��SESSSION 0����)
}
