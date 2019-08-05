#pragma once
#include <windows.h>

namespace ubase
{
	class Event
	{
	public:
		Event(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPCTSTR lpName = NULL);
		~Event();
		bool Wait(const DWORD time = INFINITE);	//�ȴ��¼�
		bool Set();								//�����¼�
		bool Reset();							//�����¼�(�����¼�δ����,�ֶ�ģʽʹ��)
		bool IsNamedEventExist();				//�ж������¼��Ƿ��Ѿ�����
		DWORD GetEventLastError();				//��ȡ����
	private:
		HANDLE m_handle;						//�¼����
		bool m_isNamedEventExist;				//�����¼��Ƿ��Ѿ�����
		DWORD m_dwErr;							//�������
	};

	Event::Event(BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName):m_isNamedEventExist(false), m_dwErr(0)
	{
		m_handle = CreateEvent(NULL, bManualReset, bInitialState, lpName);
		m_dwErr = GetLastError();

		if (m_dwErr == ERROR_ALREADY_EXISTS)	//�ж��¼��Ƿ����
		{
			m_isNamedEventExist = true;
		}
	}

	Event::~Event()
	{
		if (m_handle != NULL)
		{
			CloseHandle(m_handle);
			m_handle = NULL;
		}
	}

	bool Event::Wait(const DWORD time /* = INFINITE */)
	{
		return WAIT_OBJECT_0 == WaitForSingleObject(m_handle, time);
	}

	bool Event::Set()
	{
		return SetEvent(m_handle) == TRUE;
	}

	bool Event::Reset()
	{
		return ResetEvent(m_handle) == TRUE;
	}

	bool Event::IsNamedEventExist()
	{
		return m_isNamedEventExist;
	}

	DWORD Event::GetEventLastError()
	{
		return m_dwErr == ERROR_ALREADY_EXISTS ? 0 : m_dwErr;
	}
}