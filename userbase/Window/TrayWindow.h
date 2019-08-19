#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <locale.h>
#include <commctrl.h>
#include <atlstr.h>

typedef struct _TrayInfo
{
	RECT rect;				//ͼ������
	std::string title;		//ͼ����ʾ����
	std::string filePath;	//ͼ���Ӧ���ļ�·��
	DWORD pid;				//ͼ���Ӧ����ID
}TrayInfo;

class TrayWindow
{
public:
	TrayWindow();
	~TrayWindow();
	BOOL IsWow64();															//�ж��Ƿ���64λ����ϵͳ
	HWND FindTrayWnd();														//�ҵ����̾��
	HWND FindNotifyIconOverflowWindow();									//�ҵ�С���̾��(������С�������������ͼ��)
	bool EnumNotifyWindow(HWND hWnd, std::vector<TrayInfo>& trayInfoVec);	//���ݴ��ھ��,��ȡ������������е�����ͼ�����Ϣ
};
