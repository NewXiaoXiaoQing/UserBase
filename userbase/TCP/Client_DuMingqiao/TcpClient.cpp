#include "TcpClient.h"
#include <Ws2tcpip.h>
#include <process.h>
#include "../Common/MyCriticalSection.h"
#include "../Log/log_file.h"

#pragma comment(lib,"ws2_32.lib")

TcpClient::TcpClient(FuncProcessRecvData* pProcessRecvData) :m_connected(false)
{
	m_clientSocket = 0;
	m_pProcessRecvData = pProcessRecvData;
	::InitializeCriticalSection(&m_criticalSection);//��ʼ���ٽ���

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(sockVersion, &data);
}


TcpClient::~TcpClient()
{
	UnInit();
	::DeleteCriticalSection(&m_criticalSection);//����ʼ���ٽ���
}

bool TcpClient::Init(char* ip, unsigned short port)
{
	bool initResult = false;

	m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_clientSocket == INVALID_SOCKET)
	{
		return initResult;
	}

	//���ö�д��ʱʱ��Ϊ3S
	int timeout = 3000;
	int ret = setsockopt(m_clientSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)& timeout, sizeof(timeout));
	if (ret == -1)
	{
		//����д��ʱʧ��,��ʼ��ʧ��
		closesocket(m_clientSocket);
		return initResult;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, (void*)& serAddr.sin_addr.S_un.S_addr);
	//��ָ��IP��ַ�Ͷ˿ڵķ��������
	if (connect(m_clientSocket, (sockaddr*)& serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		closesocket(m_clientSocket);
		return initResult;
	}

	//����������Ϣ���߳�
	_beginthread(RecvThread, 0, this);

	ubase::MyCriticalSection myCriticalSection(&m_criticalSection);
	m_connected = true;
	initResult = true;

	return initResult;
}

void TcpClient::UnInit()
{
	closesocket(m_clientSocket);
	WSACleanup();
}

bool TcpClient::SendData(char* data, unsigned int length)
{
	bool sendResult = false;
	if (IsConnected() == true)//ÿ�η������ݶ�Ҫ��֤�ͻ��˺͹���̨������״̬
	{
		int iResult = send(m_clientSocket, data, length, 0);
		if (iResult == SOCKET_ERROR)
		{
			DWORD errorCode = WSAGetLastError();
			LOG(LogError, "send�������ݷ����˴���,�������:%d", errorCode);
		}
		else
			sendResult = true;
	}
	return sendResult;
}

void TcpClient::RecvThread(LPVOID args)
{
	TcpClient* p = (TcpClient*)args;
	while (true)
	{
		char buf[1024 * 4] = { 0 }; //4K���ݳ���,windows����ֵ
		bool connected = true;
		int ret = recv(p->m_clientSocket, buf, 1024 * 4, 0);
		if (ret > 0 && ret < 1024 * 4)//�����ǳ����,�����ֱ���ӵ�
		{
			buf[ret] = 0;
			p->m_pProcessRecvData(buf, ret);
		}
		else if (ret == 0)//�������ŵĶϿ�
		{
			connected = false;
		}
		else if (ret == SOCKET_ERROR)
		{
			DWORD errorCode = WSAGetLastError();
			if (errorCode == WSAENOTCONN || errorCode == WSAECONNRESET)//Զ������ǿ�ƶϿ�
			{
				connected = false;
				LOG(LogError, "Զ������ǿ�ƶϿ�����,�������:%d", errorCode);
			}
		}

		if (connected == false)
		{
			//���ÿͻ���û������
			ubase::MyCriticalSection myCriticalSection(&p->m_criticalSection);
			p->m_connected = false;
			//����Ͽ�,�����ÿͻ���socketΪ��
			closesocket(p->m_clientSocket);
			p->m_clientSocket = NULL;
			break;
		}
	}
	LOG(LogError, "TcpClient::RecvThread TCP�Ͽ�,�߳��˳�");
}

//�жϿͻ����Ƿ�����
bool TcpClient::IsConnected()
{
	ubase::MyCriticalSection myCriticalSection(&m_criticalSection);
	return m_connected;
}