#pragma once
#include <vector>
#include <string>
#include <WinSock2.h>

typedef void __stdcall FunProcessRecvData(char* data, unsigned int dataLength);

class TcpClientGo
{
public:
	TcpClientGo(FunProcessRecvData* pProcessRecvData, std::string ipAddr, unsigned int port, std::string key = "");
	~TcpClientGo();
	bool Init();//��ʼ��
	void UnInit();//����ʼ��
	bool SendData(std::string data);//��������
	bool IsConnected();//�жϿͻ����Ƿ�������״̬

protected:
	static void MinitorConnectThread(LPVOID args);//����TCP����״̬���߳�
	static void RecvThread(LPVOID args);//������Ϣ�߳�
	static void DealRecvData(LPVOID args);//������Ϣ�߳�(��Ƴ������߳�,�������)
	static void DealTrueData(LPVOID args);//�������������ݵ��߳�
	void AddRecvData(char* data, unsigned int dataLength);//�����յ���ԭʼ���ݼ��뵽�������
	bool GetRecvData(std::vector<std::string>& data);//����������е�ԭʼ����ȫ��ȡ��
	void AddTrueData(std::string& trueData);//�����������ʵ������ӵ��������
	bool GetTrueData(std::vector<std::string>& trueData);//����������е���ʵ����ȫ��ȡ��
	void UnPack(std::string& data);//����
	std::string Pack(std::string& data);//���
	std::string UnCryptData(std::string data);//����
	std::string CryptData(std::string data);//����
private:
	SOCKET m_clientSocket;
	bool m_connected;
	std::string m_ipAddr;//IP��ַ
	unsigned int m_port;//�˿ں�
	std::vector<std::string> m_originalData;//ԭʼ������
	std::vector<std::string> m_trueData;//��������ʵ����
	std::string m_aesKey;//AES���ܵ���Կ,���Ϊ��,�򲻶��������ݽ��м���
	std::string m_tmpData;//�����İ�����
	std::string m_packedHeader;//��ͷ����
	unsigned int m_packedHeaderLength;//��ͷ���ݳ���
	CRITICAL_SECTION m_connectCS;//TCP����״̬�ٽ���
	CRITICAL_SECTION m_dataCS;//���������ٽ���
	CRITICAL_SECTION m_trueDataCS;//��ʵ�����ٽ���
	FunProcessRecvData* m_pProcessRecvData;//�û�����Ľ������ݵĻص�����(stdcall)
};
