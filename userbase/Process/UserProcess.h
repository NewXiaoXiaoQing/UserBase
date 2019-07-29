#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <process.h>
#include <TlHelp32.h>

namespace ubase
{
	bool PsEnableDebugPriv();//������ǰ����Ȩ�޺���("SeDebugPrivilege"����д����Ȩ��)
	bool PsEnableShutdownPriv();//������ǰ����Ȩ�޺���("SeShutdownPrivilege"�ػ�Ȩ��)
	bool PsEnableBackupPriv();//������ǰ����Ȩ�޺���("SeBackupPrivilege"��������Ȩ��)
	bool PsEnableRestorePriv();//������ǰ����Ȩ�޺���("SeRestorePrivilege"�ָ�����Ȩ��)
	bool PsGetPidsByProcessName(std::vector<DWORD> &vDwPids, const std::string &processName);//ͨ���������ƻ�ȡ����pid,�����������ִ�Сд
	bool PsGetAllProcessName(std::vector<std::string> &vsProcessName);//��ȡ���еĽ��̵�����
	bool PsRemoteThreadInjectDll(DWORD dwPid, char* szPath);//Զ���߳�ע��dll
	HMODULE PsFindRemoteMod(HANDLE hProcess, const wchar_t * pszModShortName);//��ȡָ�������е�ģ����
	bool PsRemoteThreadUnloadDll(DWORD dwPid, wchar_t *dllName);//Զ���߳�ж��dll		  
	bool PsZwCreateThreadExInjectDll(DWORD dwProcessId, char *pszDllFileName);//ʹ��ZwCreateThreadExʵ��Զ�߳�ע��(ͻ��SESSSION 0����)
	bool PsGetRemoteModulePath(const char* moduleName, short nPID, char* modulepath);//��ȡָ��������ָ��ģ���·��(����һ��)
	DWORD PsGetRemoteModuleBaseAddr(const char* moduleName, short nPID);//��ȡָ������ָ��ģ����ڴ��ַ
	HMODULE PsGetRemoteModuleHandle(const char* moduleName, short nPID);//��ȡָ������ָ��ģ��ľ��
}
