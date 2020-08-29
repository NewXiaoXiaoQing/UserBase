#pragma once
#include <vector>
#include <string>
#include <winsock2.h>
#include <Iphlpapi.h>
#include <windows.h>
#include <tchar.h>

namespace ubase
{
	enum TCP_STATE
	{
		NONE,
		CLOSED,
		LISTENING,
		SYN_SENT,
		SEN_RECEIVED,
		ESTABLISHED,
		FIN_WAIT,
		FIN_WAIT2,
		CLOSE_WAIT,
		CLOSING,
		LAST_ACK,
		TIME_WAIT
	};

	bool GetLocalMac(std::vector<std::string> &vsMacAddress);//��ȡ��������������MAC��ַ
	int GetNetworkAdapter(std::vector<std::string> &vtMacs);//��ȡ���е���ʵ������ַ
	bool GetTcpConnect(std::vector<std::string> &tcp, TCP_STATE tcpstate, DWORD pid);//ͨ������ID��TCP״̬ɸѡ��������
};