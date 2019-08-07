#include <string>
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "zlib\\zconf.h"
#include "zlib\\zlib.h"
#ifdef _DEBUG
	#pragma comment(lib, "debug\\zlibstat.lib")
#else
	#pragma comment(lib, "release\\zlibstat.lib")
#endif

#define MAX_SRC_FILE_SIZE (100*1024*1024)			// �����ļ����ƴ�СΪ 100M 

namespace ubase
{
	// ����ѹ��
	// ���룺��Ҫѹ���ļ���·��
	// ���������ѹ�����ѹ���������ݡ�����ѹ�����ѹ���������ݳ���
	BOOL Zlib_CompressData(char* pszCompressFileName, BYTE** ppCompressData, DWORD* pdwCompressDataSize);


	// ���ݽ�ѹ
	// ���룺��Ҫ��ѹ���ļ���·��
	// ��������ݽ�ѹ����������ݡ����ݽ�ѹ������ݳ���
	BOOL Zlib_UncompressData(char* pszUncompressFileName, BYTE** ppUncompressData, DWORD* pdwUncompressDataSize);


	// ���ݽ�ѹ
	// ���룺��Ҫ��ѹ�����������ݡ���Ҫ��ѹ�����������ݳ���
	// ��������ݽ�ѹ����������ݡ����ݽ�ѹ������ݳ���
	BOOL Zlib_UncompressData(BYTE* pCompressData, DWORD dwCompressDataSize, BYTE** ppUncompressData, DWORD* pdwUncompressDataSize);


	// �����ݴ洢Ϊ�ļ�
	// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
	BOOL SaveToFile(char* pszFileName, BYTE* pData, DWORD dwDataSize);


	// �����ݴ洢Ϊ�ļ�
	// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
	BOOL SaveToOriginalFile(char* pszFileName, BYTE* pData, DWORD dwDataSize);

	// ���ַ���ѹ��
	// ���룺ԭʼ�ַ���
	// ���������ѹ����Ķ�����
	std::string CompressData(const std::string data);

	// �������ƽ�ѹ��
	// ���룺ѹ����Ķ�����
	// �����ԭʼ�ַ���
	std::string UnCompressData(const std::string data);
}
