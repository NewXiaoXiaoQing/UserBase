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
}
