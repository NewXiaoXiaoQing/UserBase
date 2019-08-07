#if !defined(AFX_IOCPMODESVR_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)
#define AFX_IOCPMODESVR_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ClientList.h"
#include <winsock2.h>
#pragma  comment(lib,"ws2_32.lib")

//����˿�
#define SVRPORT 80
//��������С
#define BUFFER_SIZE 1024 * 4//1024 * 200
//��������
#define RECV_POSTED 0
//��������
#define SEND_POSTED 1
//���������
typedef struct _PER_HANDLE_DATA
{
	unsigned long IpAddr;
	SOCKET sClient;
}PER_HANDLE_DATA, * LPPER_HANDLE_DATA;
//IO��������
typedef struct _PER_IO_OPERATION_DATA
{
	//�ص��ṹ
	OVERLAPPED OverLapped;
	//���ݻ�����
	WSABUF DataBuf;
	char Buf[BUFFER_SIZE];
	//�������ͱ�ʾ���͡����ջ�رյ�
	bool OperType;
}PER_IO_OPERATION_DATA, * PPER_IO_OPERATION_DATA;

//�ص��������ݺ���ԭ��
typedef void __stdcall ProcessRecvData(unsigned long sIP,
	SOCKET sClient,
	char* pData,
	unsigned long DataLength,
	void* pContext);

DWORD WINAPI ServerWorkerProc(LPVOID lParam);
DWORD WINAPI ListenProc(LPVOID lParam);

//#################��ɶ˿�socket###################
class CIocpModeSvr
{
public:
	CIocpModeSvr();
	virtual ~CIocpModeSvr();
public:
	void* m_pContext;
	//��ʼ��
	bool Init(ProcessRecvData* pProcessRecvData, void* pContext, unsigned long iSvrPort = SVRPORT);
	//����ʼ��
	void UnInit();
	/*  ���ڷ�����Ϣ�ĺ�����*/
public:

	bool SendMsg(SOCKET sClient, char* pData, unsigned long Length);
	bool SendMsgToAll(char* pData, unsigned long Length);

	bool DisConnectClient(SOCKET sClient);
	void DisConnectAll();

	void RemoveSocket(SOCKET sClient);
public:
	//��ñ���Ip��ַ
	const char* GetLocalIpAdd() {
		if (IsStart)return HostIpAddr;
		else return NULL;
	}
	//��÷�����ʹ�õĶ˿ں�
	unsigned short GetSvrPort() {
		if (IsStart)return m_SvrPort;
		else return 0;
	}

protected:
	int InitNetWork(unsigned int SvrPort = SVRPORT,
		char* pHostIpAddress = NULL);

	ProcessRecvData* m_pProcessRecvData;
private:
	//��ɾ��
	HANDLE CompletionPort;
	//����IP
	char  HostIpAddr[32];
	//�ͻ���Ϣ�б�
	DCLinkedList<PER_HANDLE_DATA> ClientList;
	//�ͻ���Ϣ�ٽ籣����
	CRITICAL_SECTION cInfoSection;
	//�����Ƿ��Ѿ�����
	bool IsStart;
	//����SOCKET
	SOCKET ListenSocket;
	//�����߳̾�������ڷ���ʼ��ʱ���������߳�
	HANDLE ListenThreadHandle;
	//����˿�
	unsigned short m_SvrPort;

	friend DWORD WINAPI ServerWorkerProc(LPVOID lParam);
	friend DWORD WINAPI ListenProc(LPVOID lParam);
};

#endif // !defined(AFX_IOCPMODESVR_H__46FFF420_23C3_4356_A88D_AEBDA61EA186__INCLUDED_)
