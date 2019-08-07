#pragma once
#include <windows.h>
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

