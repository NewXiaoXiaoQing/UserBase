#pragma once
#include <stdio.h>
#include <windows.h>


BOOL CALLBACK KeyMainDlg(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);//���̰��������ػص�����
BOOL KeyInputInit(HWND hWnd);//ע��ԭʼ�����豸
BOOL KeyInputGetData(LPARAM lParam);//��ȡԭʼ��������
void KeyInputSaveKey(USHORT usVKey);//���水����Ϣ