#pragma once
#include <vector>
#include <string>
#include <winsock2.h>
#include <Iphlpapi.h>
#include <windows.h>

namespace ubase
{
	bool GetLocalMac(std::vector<std::string> &vsMacAddress);//��ȡ��������������MAC��ַ
};