#pragma once
#include <windows.h>

namespace ubase
{
	__int64 TimerGetUnixTime();//��ȡ��ǰ��UNIXʱ���
	__int64 TimerGetProcessCreateTime(DWORD dwPid);//��ȡ���̵�����ʱ��
}