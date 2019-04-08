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
	std::wstring StrGbkToUnicodeV2(const char *lpcszString);//gbkתunicode�汾2(�õ�windows��api����)
	std::string StrUnicodeToUtf8(const std::wstring &wstr);//unicodeתutf8
	std::wstring StrUtf8ToUnicode(const std::string &str);//utf8תunicode
	std::string StrUtf8ToGbk(const std::string &str);//utf8תgbk
	std::string StrGbkToUtf8(const std::string &str);//gbkתutf8
	std::string &StdStrFormat(std::string & _str, const char * _Format, ...);//std::string�汾��sprintf����
	std::string StrGetRandomStr(unsigned int count);//����ָ�����ȵ�����ַ���(����������ɵ�)
	std::string StrGetRandomStrEx(unsigned int count, char *characters);//ͨ��ָ�����ַ�������������ַ���(����������ɵ�)
	void Byte2Hex(unsigned char bData, unsigned char hex[]);//�ַ�ת16������ʾ
	std::string StrCvtByteToString(unsigned char *buffer, unsigned int bufferSize);//���ڴ��ַ���16���Ƶ���ʽ���ֳ���
	std::string StrTrim(std::string &str);//ȥ��std::string�ַ����������ߵĿո�
	std::string BinToHex(const std::string& strBin, bool bIsUpper = false);
	std::string HexToBin(const std::string& strHex);
}
