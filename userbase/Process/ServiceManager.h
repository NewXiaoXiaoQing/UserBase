#ifndef _HKXIAOYU118_SERVICE_MANAGER_H_
#define _HKXIAOYU118_SERVICE_MANAGER_H_


#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")


class ServiceManager
{
public:
	ServiceManager();
	~ServiceManager();

public:

	/*
		0 ���ط���
		1 ��������
		2 ֹͣ����
		3 ɾ������
	*/
	void ShowError(char* lpszText);
	BOOL SystemServiceCtrl(char* lpszExePath, int iOperateType);
};

#endif