#pragma once
#include <windows.h>

bool EnableDebugPriv();//������ǰ����Ȩ�޺���("SeDebugPrivilege"����д����Ȩ��)
bool EnableShutdownPriv();//������ǰ����Ȩ�޺���("SeShutdownPrivilege"�ػ�Ȩ��)
bool EnableBackupPriv();//������ǰ����Ȩ�޺���("SeBackupPrivilege"��������Ȩ��)
bool EnableRestorePriv();//������ǰ����Ȩ�޺���("SeRestorePrivilege"�ָ�����Ȩ��)