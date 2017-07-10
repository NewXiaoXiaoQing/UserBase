#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <windows.h>
#include <assert.h>
#include <Wincrypt.h>

namespace ubase
{
	std::string StrUnicodeToGbk(const std::wstring &wstr);//unicodeתgbk
	std::wstring StrGbkToUnicode(const std::string &str);//gbkתunicode
	std::string StrUnicodeToUtf8(const std::wstring &wstr);//unicodeתutf8
	std::wstring StrUtf8ToUnicode(const std::string &str);//utf8תunicode
	std::string StrGetRandomStr(unsigned int count);//����ָ�����ȵ�����ַ���(����������ɵ�)
	std::string StrGetRandomStrEx(unsigned int count, char *characters);//ͨ��ָ�����ַ�������������ַ���(����������ɵ�)
}
