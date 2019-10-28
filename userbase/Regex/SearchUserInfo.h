#pragma once
#include <string>
#include <regex>

namespace REGEX
{
	std::string g_mailRetexStr = "([0-9A-Za-z\\-_\\.]+)@(([a-zA-Z0-9\\-])+\\.+[a-z]{2,3}(\\.[a-z]{2})?)";	//����
	std::string g_chinaTelRegexStr = "(13[0-9]|14[579]|15[0-3,5-9]|16[6]|17[0135678]|18[0-9]|19[89])\\d{8}";//�й���½�ֻ�����

	std::string SearchEmail(std::string data);		//��������
	std::string SearchChinaTel(std::string data);	//�����й���½�绰����
}

