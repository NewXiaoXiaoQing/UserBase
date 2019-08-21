#pragma once
#include <windows.h>

/*
	�رյ�ǰ�̵߳�·���ض���(xp�²���ʹ��,��Ϊxp��û��Wow64����)
*/
class FsRedirector
{
public:
	FsRedirector(bool wow64)
		: m_wow64(wow64)
	{
#ifndef XP_BUILD
		if (wow64)
			Wow64DisableWow64FsRedirection(&m_fsRedirection);
#endif
	}

	~FsRedirector()
	{
#ifndef XP_BUILD
		if (m_wow64)
			Wow64RevertWow64FsRedirection(m_fsRedirection);
#endif
	}

private:
	PVOID m_fsRedirection = nullptr;
	bool m_wow64;
};
