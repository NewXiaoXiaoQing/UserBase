#pragma once
#include <WinSock2.h>

typedef void __stdcall FuncProcessRecvData(char* data, unsigned int dataLength);

class TcpClient
{
public:
	TcpClient(FuncProcessRecvData* pProcessRecvData);
	~TcpClient();
	bool Init(char* ip, unsigned short port);
	void UnInit();
	bool SendData(char* data, unsigned int length);
	bool IsConnected();//�жϿͻ����Ƿ�������״̬

protected:
	static void RecvThread(LPVOID args);
private:
	SOCKET m_clientSocket;
	FuncProcessRecvData* m_pProcessRecvData;
	bool m_connected;
	CRITICAL_SECTION m_criticalSection;
};
