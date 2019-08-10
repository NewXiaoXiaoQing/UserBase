#include "TcpClientGo.h"
#include <Ws2tcpip.h>
#include <process.h>
#include "../Common/MyCriticalSection.h"
#include "../Log/log_file.h"
#include "../Integer/UserInteger.h"
#include "../Crypto/aes_helper.h"

#pragma comment(lib,"ws2_32.lib")

TcpClientGo::TcpClientGo(FunProcessRecvData* pProcessRecvData, std::string ipAddr, unsigned int port, std::string key) :m_connected(false)
{
	m_clientSocket = NULL;
	m_pProcessRecvData = pProcessRecvData;
	m_ipAddr = ipAddr;//���ӵ�IP��ַ
	m_port = port;//���ӵĶ˿�
	m_aesKey = key;//���ܵ���Կ
	m_packedHeader = "www.yuyincun.com";//���ͷ��־
	m_packedHeaderLength = m_packedHeader.size();//���ͷ��־�ĳ���
	::InitializeCriticalSection(&m_connectCS);
	::InitializeCriticalSection(&m_dataCS);
	::InitializeCriticalSection(&m_trueDataCS);

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(sockVersion, &data);

	_beginthread(MinitorConnectThread, 0, this);//����״̬���/����
}

TcpClientGo::~TcpClientGo()
{
	UnInit();
	::DeleteCriticalSection(&m_connectCS);
	::DeleteCriticalSection(&m_dataCS);
	::DeleteCriticalSection(&m_trueDataCS);
}

bool TcpClientGo::Init()
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
	serAddr.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddr.c_str(), (void*)& serAddr.sin_addr.S_un.S_addr);
	//��ָ��IP��ַ�Ͷ˿ڵķ��������
	if (connect(m_clientSocket, (sockaddr*)& serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		closesocket(m_clientSocket);
		return initResult;
	}

	//�������������ݵ��߳�
	_beginthread(DealRecvData, 0, this);
	//����������ʵ���ݵ��߳�
	_beginthread(DealTrueData, 0, this);
	//����������Ϣ���߳�
	_beginthread(RecvThread, 0, this);

	ubase::MyCriticalSection myCriticalSection(&m_connectCS);
	m_connected = true;
	initResult = true;

	return initResult;
}

void TcpClientGo::UnInit()
{
	if (m_clientSocket != NULL)
		closesocket(m_clientSocket);
	WSACleanup();
}

bool TcpClientGo::SendData(std::string data)
{
	bool sendResult = false;
	if (IsConnected() == true)//ÿ�η������ݶ�Ҫ��֤�ͻ��˺͹���̨������״̬
	{
		std::string packedData = Pack(data);//���
		int iResult = send(m_clientSocket, packedData.c_str(), packedData.length(), 0);
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

void TcpClientGo::MinitorConnectThread(LPVOID args)
{
	TcpClientGo* p = (TcpClientGo*)args;
	unsigned int count = 0;
	while (true)
	{
		if (p->IsConnected() == false)//����Ͽ�������,����������
		{
			if (count == 60)
			{
				LOG(LogError, "Tcp�Ͽ�,��������...");
				count = 0;
			}
			else
				count++;

			bool reConResult = p->Init();//���³�ʼ��
			if (reConResult == true)
			{
				LOG(LogInfo, "TCP�������ӳɹ�...");
				count = 60;
			}
		}
		Sleep(500);//500ms����һ��
	}
}

void TcpClientGo::RecvThread(LPVOID args)
{
	TcpClientGo* p = (TcpClientGo*)args;
	while (true)
	{
		char buf[1024 * 4] = { 0 }; //4K���ݳ���,windows����ֵ
		bool connected = true;
		int ret = recv(p->m_clientSocket, buf, 1024 * 4, 0);
		if (ret > 0 && ret <= 1024 * 4)//����������
		{
			//buf[ret] = 0;
			p->AddRecvData(buf, ret);
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
			ubase::MyCriticalSection myCriticalSection(&p->m_connectCS);
			p->m_connected = false;
			//����Ͽ�,�����ÿͻ���socketΪ��
			closesocket(p->m_clientSocket);
			p->m_clientSocket = NULL;
			break;
		}
	}
	LOG(LogError, "TcpClientGo::RecvThread TCP�Ͽ�,�߳��˳�");
}

//�жϿͻ����Ƿ�����
bool TcpClientGo::IsConnected()
{
	ubase::MyCriticalSection myCriticalSection(&m_connectCS);
	return m_connected;
}

void TcpClientGo::DealRecvData(LPVOID args)
{
	TcpClientGo* p = (TcpClientGo*)args;
	while (true)
	{
		if (p->IsConnected() == true)
		{
			std::vector<std::string> vtData;
			bool result = p->GetRecvData(vtData);//һ����ȡ����������
			if (result == true)
			{
				for (std::vector<std::string>::iterator iter = vtData.begin(); iter != vtData.end(); iter++)
				{
					p->UnPack(*iter);//��������
				}
			}
		}
		else
		{
			LOG(LogWarning, "TCP���ӶϿ�,������ݴ����߳��˳�...");
			break;
		}

		Sleep(50);
	}
}

void TcpClientGo::DealTrueData(LPVOID args)
{
	TcpClientGo* p = (TcpClientGo*)args;
	while (true)
	{
		if (p->IsConnected() == true)
		{
			std::vector<std::string> vtTrueData;
			bool result = p->GetTrueData(vtTrueData);//һ����ȡ����������
			if (result == true)
			{
				for (std::vector<std::string>::iterator iter = vtTrueData.begin(); iter != vtTrueData.end(); iter++)
				{
					//1.�����ݽ���
					//2.������ͨ���ص��������ݸ�������
					std::string message = p->UnCryptData(*iter);
					p->m_pProcessRecvData((char*)message.c_str(), message.length());
				}
			}
		}
		else
		{
			LOG(LogWarning, "TCP���ӶϿ�,��ʵ���ݴ����߳��˳�...");
			break;
		}

		Sleep(50);
	}
}

void TcpClientGo::AddRecvData(char* data, unsigned int dataLength)
{
	ubase::MyCriticalSection mtp(&m_dataCS);
	std::string strData;
	strData.resize(dataLength);
	memcpy((char *)strData.c_str(), data, dataLength);
	m_originalData.push_back(strData);
}

bool TcpClientGo::GetRecvData(std::vector<std::string>& data)
{
	ubase::MyCriticalSection mtp(&m_dataCS);
	bool result = false;
	data = m_originalData;
	m_originalData.clear();
	if (data.size() != 0)
		result = true;
	return result;
}

void TcpClientGo::AddTrueData(std::string& trueData)
{
	ubase::MyCriticalSection mtp(&m_trueDataCS);
	m_trueData.push_back(trueData);
}

bool TcpClientGo::GetTrueData(std::vector<std::string>& trueData)
{
	ubase::MyCriticalSection mtp(&m_trueDataCS);
	bool result = false;
	trueData = m_trueData;
	m_trueData.clear();
	if (trueData.size() != 0)
		result = true;
	return result;
}

#define CONST_SAVEDATALENGTH 4 //�洢���ݳ��ȵ��ֽ���

void TcpClientGo::UnPack(std::string& data)
{
	std::string dealData = m_tmpData + data;//���������
	unsigned int length = dealData.length();
	int i = 0;
	for (i = 0; i < length; i++)
	{
		//��֤�����ȵĺϷ���
		if (length < i + m_packedHeaderLength + CONST_SAVEDATALENGTH)
		{
			break;
		}
		//��֤��ͷ��־�Ƿ�Ϸ�
		if (strncmp(dealData.c_str(), m_packedHeader.c_str(), m_packedHeaderLength) == 0)
		{
			//��ȡʵ�����ݵĳ���
			unsigned int messageLength = ubase::ConvertBytesToUInt((unsigned char*)(dealData.c_str() + m_packedHeaderLength), true);
			//��֤���ĳ����Ƿ�Ϸ�
			if (length < i + m_packedHeaderLength + CONST_SAVEDATALENGTH + messageLength)
			{
				break;
			}
			//��ȡ����������
			std::string trueData;
			trueData.resize(messageLength);
			memcpy((char*)trueData.c_str(), (char*)(dealData.c_str() + m_packedHeaderLength + CONST_SAVEDATALENGTH), messageLength);
			//ͨ����Ϣ���ݻ�����д���
			AddTrueData(trueData);
			//�ƶ�����ָ�뵽�µ�λ��
			i += m_packedHeaderLength + CONST_SAVEDATALENGTH + messageLength - 1;
		}
	}
	if (i == length)
	{
		m_tmpData = "";
		m_tmpData.resize(0);
	}
	else
	{
		m_tmpData = "";
		m_tmpData.resize(length - i);//ʣ�����ݵĳ���
		memcpy((char*)m_tmpData.c_str(), (char*)(dealData.c_str() + i), length - i);//��ʣ������ݱ���
	}
}

std::string TcpClientGo::Pack(std::string& data)
{
	std::string result;
	unsigned char binLength[CONST_SAVEDATALENGTH] = { 0 };
	std::string cryptedData = CryptData(data);
	ubase::ConvertUIntToBytes(cryptedData.length(), binLength, true);
	result.resize(m_packedHeaderLength + CONST_SAVEDATALENGTH + cryptedData.length());//�������
	memcpy((char*)result.c_str(), m_packedHeader.c_str(), m_packedHeaderLength);//����ͷ��־
	memcpy((char*)(result.c_str() + m_packedHeaderLength), binLength, CONST_SAVEDATALENGTH);//������ݳ���4�ֽ�
	memcpy((char*)(result.c_str() + m_packedHeaderLength + CONST_SAVEDATALENGTH), cryptedData.c_str(), cryptedData.length());//��������
	return result;
}

std::string TcpClientGo::UnCryptData(std::string data)
{
	std::string result;
	if (m_aesKey.empty() == false)
	{
		result = ctrDecoder(data, m_aesKey);
	}
	else
	{
		result = data;
	}
	return result;
}

std::string TcpClientGo::CryptData(std::string data)
{
	std::string result;
	if (m_aesKey.empty() == false)
	{
		result = ctrEncoder(data, m_aesKey);
	}
	else
	{
		result = data;
	}
	return result;
}