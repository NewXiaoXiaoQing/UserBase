#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <list>
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
	void StrLowerString(std::string& str);//��stringת��ΪСд
	void StrLowerWString(std::wstring& str);//��wstringת��ΪСд
	void StrUpperString(std::string& str);//��wstringת��Ϊ��д
	void StrUpperWString(std::wstring& str);//��wstringת��Ϊ��д
	std::string StrMakeLowerString(const std::string& src);//��stringת��ΪСд,���ı�Դ�ַ�����ֵ
	std::wstring StrMakeLowerWString(const std::wstring& src);//��wstringת��ΪСд,���ı�Դ�ַ�����ֵ
	std::string StrMakeUpperString(const std::string& src);//��stringת��Ϊ��д,���ı�Դ�ַ�����ֵ
	std::wstring StrMakeUpperWString(const std::wstring& src);//��wstringת��Ϊ��д,���ı�Դ�ַ�����ֵ
	std::string BinaryToHexString(const void* binary, size_t length);//��������ת��Ϊ�ַ���
	std::string HexStringToBinary(const char* input, size_t length);//���ַ���ת��Ϊ������
	char StrHexCharToInt8(char c);//16��ֹ�ַ�ת����
	bool StrValidateUTF8Stream(const void* stream, unsigned length);//�ж��ֽ����Ƿ���UTF8��ʽ
	bool StrValidateGB2312Stream(const void* stream, unsigned length);//�ж��ֽ����Ƿ���GB2312��ʽ
	bool StrValidateGBKStream(const void* stream, unsigned length);//�ж��ֽ����Ƿ���GBK��ʽ
	bool StrValidateBIG5Stream(const void* stream, unsigned length);//�ж��ֽ����Ƿ���BIG5(������)��ʽ
	std::string& StrStringTrimLeft(std::string& input);//��string�����ַ������ȥ�ո�
	std::string& StrStringTrimRight(std::string& input);//��string�����ַ����ұ�ȥ�ո�
	std::string& StrStringTrim(std::string& input);//��string�����ַ�������ȥ�ո�
	std::wstring& StrWStringTrimLeft(std::wstring& input);//��wstring�����ַ������ȥ�ո�
	std::wstring& StrWStringTrimRight(std::wstring& input);//��wstring�����ַ����ұ�ȥ�ո�
	std::wstring& StrWStringTrim(std::wstring& input);//��wstring�����ַ�������ȥ�ո�
	size_t StrStringReplaceAll(const std::string& find, const std::string& replace, std::string& output);//�滻string�����ַ����е������ı�
	size_t StrWStringReplaceAll(const std::wstring& find, const std::wstring& replace, std::wstring& output);//�滻wstring�����ַ����е������ı�
	std::list<std::string> StrStringTokenize(const char* input, const char* delimitor);//�ҵ�string�����ַ���������ͨ��delimitor��ʶ�ָ��Ԫ��
	std::list<std::wstring> StrWStringTokenize(const wchar_t* input, const wchar_t* delimitor);//�ҵ�wstring�����ַ���������ͨ��delimitor��ʶ�ָ��Ԫ��

	bool StrBase64Encode(const std::string& input, std::string* output);//base64����
	bool StrBase64Decode(const std::string& input, std::string* output);//base64����
}
