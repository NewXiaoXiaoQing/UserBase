#pragma once
#include <string>
#include <windows.h>

namespace ubase
{
	std::string GetErrorDescription(NTSTATUS code);//����GetLastError��ȡ������Ϣ
}
