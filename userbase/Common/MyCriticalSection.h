#pragma once
#include <windows.h>

namespace ubase
{
	/*
		�ٽ�����װ��(���ڿ���������Χ���ٽ���)
	*/
	class MyCriticalSection
	{
	public:
		MyCriticalSection(CRITICAL_SECTION* criticalSection)
		{
			m_criticalSection = criticalSection;
			::EnterCriticalSection(m_criticalSection);//�����ٽ���
		}
		~MyCriticalSection()
		{
			::LeaveCriticalSection(m_criticalSection);//�˳��ٽ���
		}
	private:
		CRITICAL_SECTION* m_criticalSection;
	};

	/*
		�ٽ�����װ��(�����ֶ������ٽ���)
	*/
	class CriticalSection
	{
	public:
		CriticalSection()
		{
			::InitializeCriticalSection(&m_criticalSection);
		}

		~CriticalSection()
		{
			::DeleteCriticalSection(&m_criticalSection);
		}

		void Lock()
		{
			::EnterCriticalSection(&m_criticalSection);
		}

		void UnLock()
		{
			::LeaveCriticalSection(&m_criticalSection);
		}
	private:
		CRITICAL_SECTION m_criticalSection;
	};
}
