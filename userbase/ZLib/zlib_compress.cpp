#include "zlib_compress.h"

namespace ubase
{
	// 数据压缩
	// 输入：将要压缩文件的路径
	// 输出：数据压缩后的压缩数据内容、数据压缩后的压缩数据内容长度
	BOOL Zlib_CompressData(char* pszCompressFileName, BYTE** ppCompressData, DWORD* pdwCompressDataSize)
	{
		// 注意可能出现压缩后的文件比压缩前的文件大的现象!!!

		// 打开文件 并 获取文件数据
		HANDLE hFile = ::CreateFile(pszCompressFileName, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return FALSE;
		}

		DWORD dwFileSize = ::GetFileSize(hFile, NULL);       // 获取文件大小
		if (MAX_SRC_FILE_SIZE < dwFileSize)
		{
			::CloseHandle(hFile);
			return FALSE;
		}

		// 判断是否满足大小限制条件
		if (MAX_SRC_FILE_SIZE < dwFileSize)
		{
			::CloseHandle(hFile);
			return FALSE;
		}
		DWORD dwDestDataSize = dwFileSize;

		BYTE* pSrcData = new BYTE[dwFileSize];
		if (NULL == pSrcData)
		{
			::CloseHandle(hFile);
			return FALSE;
		}
		BYTE* pDestData = new BYTE[dwDestDataSize];
		if (NULL == pDestData)
		{
			::CloseHandle(hFile);
			return FALSE;
		}

		DWORD dwRet = 0;
		::ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);	 // 读取文件数据
		if ((0 >= dwRet) ||
			(dwRet != dwFileSize))
		{
			delete[]pDestData;
			pDestData = NULL;
			delete[]pSrcData;
			pSrcData = NULL;
			::CloseHandle(hFile);
			return FALSE;
		}

		// 压缩数据
		/*
		int compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
		compress函数将source缓冲区中的内容压缩到dest缓冲区。sourceLen表示source缓冲区的大小（以字节计）。
		注意：函数的第二个参数destLen是传址调用，当调用函数时，destLen表示dest缓冲区大小（初始值不能为0哦），
		( destLen > (sourceLen + 12) * 100.1% )；当函数退出后，destLen表示压缩后缓冲区的实际大小。
		此时，destLen/sourceLen正好是压缩率!!!
		返回值：
		-5 : 输出缓冲区不够大；
		-4 : 没有足够的内存；
		0 : 表示成功；
		*/
		int iRet = 0;
		do
		{
			iRet = compress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
			if (0 == iRet)
			{
				// 成功
				break;
			}
			else if (-5 == iRet)
			{
				// 输出缓冲区不够大, 以 100KB 大小递增
				delete[]pDestData;
				pDestData = NULL;
				dwDestDataSize = dwDestDataSize + (100 * 1024);
				pDestData = new BYTE[dwDestDataSize];
				if (NULL == pDestData)
				{
					delete[]pSrcData;
					pSrcData = NULL;
					::CloseHandle(hFile);
					return FALSE;
				}
			}
			else
			{
				// 没有足够的内存 或 其他情况
				delete[]pDestData;
				pDestData = NULL;
				delete[]pSrcData;
				pSrcData = NULL;
				::CloseHandle(hFile);
				return FALSE;
			}
		} while (TRUE);
		// 返回数据
		*ppCompressData = pDestData;
		*pdwCompressDataSize = dwDestDataSize;

		// 释放
		//	delete[]pDestData;
		//	pDestData = NULL;
		delete[]pSrcData;
		pSrcData = NULL;
		::CloseHandle(hFile);

		return TRUE;
	}


	// 数据解压
	// 输入：将要解压缩文件的路径
	// 输出：数据解压后的数据内容、数据解压后的内容长度
	BOOL Zlib_UncompressData(char* pszUncompressFileName, BYTE** ppUncompressData, DWORD* pdwUncompressDataSize)
	{
		// 注意可能出现压缩后的文件比压缩前的文件大的现象!!!

		// 打开文件 并 获取文件数据
		HANDLE hFile = ::CreateFile(pszUncompressFileName, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return FALSE;
		}

		DWORD dwFileSize = ::GetFileSize(hFile, NULL);       // 获取文件大小
		DWORD dwDestDataSize = MAX_SRC_FILE_SIZE;

		BYTE* pSrcData = new BYTE[dwFileSize];
		if (NULL == pSrcData)
		{
			::CloseHandle(hFile);
			return FALSE;
		}
		BYTE* pDestData = new BYTE[dwDestDataSize];
		if (NULL == pDestData)
		{
			::CloseHandle(hFile);
			return FALSE;
		}

		DWORD dwRet = 0;
		::ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);	 // 读取文件数据
		if ((0 >= dwRet) ||
			(dwRet != dwFileSize))
		{
			delete[]pDestData;
			pDestData = NULL;
			delete[]pSrcData;
			pSrcData = NULL;
			::CloseHandle(hFile);
			return FALSE;
		}

		// 解压缩数据
		/*
		int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
		compress函数将source缓冲区中的内容压缩到dest缓冲区。sourceLen表示source缓冲区的大小（以字节计）。
		注意：函数的第二个参数destLen是传址调用，当调用函数时，destLen表示dest缓冲区大小（初始值不能为0哦），
		( destLen > (sourceLen + 12) * 100.1% )；当函数退出后，destLen表示压缩后缓冲区的实际大小。
		此时，destLen/sourceLen正好是压缩率!!!
		返回值：
		-5 : 输出缓冲区不够大；
		-4 : 没有足够的内存；
		0 : 表示成功；
		*/
		int iRet = 0;
		do
		{
			iRet = uncompress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
			if (0 == iRet)
			{
				// 成功
				break;
			}
			else if (-5 == iRet)
			{
				// 输出缓冲区不够大, 以 100KB 大小递增
				delete[]pDestData;
				pDestData = NULL;
				dwDestDataSize = dwDestDataSize + (100 * 1024);
				pDestData = new BYTE[dwDestDataSize];
				if (NULL == pDestData)
				{
					delete[]pSrcData;
					pSrcData = NULL;
					::CloseHandle(hFile);
					return FALSE;
				}
			}
			else
			{
				// 没有足够的内存 或 其他情况
				delete[]pDestData;
				pDestData = NULL;
				delete[]pSrcData;
				pSrcData = NULL;
				::CloseHandle(hFile);
				return FALSE;
			}
		} while (TRUE);
		// 返回数据
		*ppUncompressData = pDestData;
		*pdwUncompressDataSize = dwDestDataSize;

		// 释放
		//	delete[]pDestData;
		//	pDestData = NULL;
		delete[]pSrcData;
		pSrcData = NULL;
		::CloseHandle(hFile);

		return TRUE;
	}


	// 数据解压
	// 输入：将要解压缩的数据内容、将要解压缩的数据内容长度
	// 输出：数据解压后的数据内容、数据解压后的内容长度
	BOOL Zlib_UncompressData(BYTE* pCompressData, DWORD dwCompressDataSize, BYTE** ppUncompressData, DWORD* pdwUncompressDataSize)
	{
		// 注意可能出现压缩后的文件比压缩前的文件大的现象!!!

		// 申请动态内存
		DWORD dwDestDataSize = MAX_SRC_FILE_SIZE;
		BYTE* pDestData = new BYTE[dwDestDataSize];
		if (NULL == pDestData)
		{
			return FALSE;
		}

		// 解压缩数据
		/*
		int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
		compress函数将source缓冲区中的内容压缩到dest缓冲区。sourceLen表示source缓冲区的大小（以字节计）。
		注意：函数的第二个参数destLen是传址调用，当调用函数时，destLen表示dest缓冲区大小（初始值不能为0哦），
		( destLen > (sourceLen + 12) * 100.1% )；当函数退出后，destLen表示压缩后缓冲区的实际大小。
		此时，destLen/sourceLen正好是压缩率!!!
		返回值：
		-5 : 输出缓冲区不够大；
		-4 : 没有足够的内存；
		0 : 表示成功；
		*/
		int iRet = 0;
		do
		{
			iRet = uncompress(pDestData, &dwDestDataSize, pCompressData, dwCompressDataSize);
			if (0 == iRet)
			{
				// 成功
				break;
			}
			else if (-5 == iRet)
			{
				// 输出缓冲区不够大, 以 100KB 大小递增
				delete[]pDestData;
				pDestData = NULL;
				dwDestDataSize = dwDestDataSize + (100 * 1024);
				pDestData = new BYTE[dwDestDataSize];
			}
			else
			{
				// 没有足够的内存 或 其他情况
				delete[]pDestData;
				pDestData = NULL;
				return FALSE;
			}
		} while (TRUE);
		// 返回数据
		*ppUncompressData = pDestData;
		*pdwUncompressDataSize = dwDestDataSize;

		// 释放
		//	delete[]pDestData;
		//	pDestData = NULL;

		return TRUE;
	}


	void Zlib_GetCurrentFile(char* pszCurrentFile, DWORD dwBufferSize)
	{
		char szFileName[MAX_PATH] = { 0 };
		::RtlZeroMemory(szFileName, MAX_PATH);

		::GetModuleFileName(NULL, szFileName, MAX_PATH);
		char* p = ::strrchr(szFileName, '\\');
		p[0] = '\0';

		::lstrcat(szFileName, "\\");
		::lstrcat(szFileName, pszCurrentFile);

		::RtlZeroMemory(pszCurrentFile, dwBufferSize);
		::lstrcpy(pszCurrentFile, szFileName);
	}


	// 将数据存储为文件
	// 输入：数据原文件路径、将要保存的数据内容、将要保存的数据内容长度
	BOOL SaveToFile(char* pszFileName, BYTE* pData, DWORD dwDataSize)
	{
		char szSaveName[MAX_PATH] = { 0 };
		::lstrcpy(szSaveName, pszFileName);
		::PathStripPath(szSaveName);
		Zlib_GetCurrentFile(szSaveName, MAX_PATH);

		HANDLE hFile = ::CreateFile(szSaveName, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return FALSE;
		}

		DWORD dwRet = 0;
		::WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);

		::CloseHandle(hFile);

		return TRUE;
	}


	// 将数据存储为文件
	// 输入：数据原文件路径、将要保存的数据内容、将要保存的数据内容长度
	BOOL SaveToOriginalFile(char* pszFileName, BYTE* pData, DWORD dwDataSize)
	{
		char szSaveName[MAX_PATH] = { 0 };
		::lstrcpy(szSaveName, pszFileName);
		::PathStripPath(szSaveName);
		// 创建文件
		HANDLE hFile = ::CreateFile(szSaveName, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return FALSE;
		}
		// 写入数据
		DWORD dwRet = 0;
		::WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);
		// 关闭句柄
		::CloseHandle(hFile);

		return TRUE;
	}


	// 将字符串压缩
	// 输入：原始字符串
	// 输出：经过压缩后的二进制
	std::string CompressData(const std::string data)
	{
		std::string result;
		DWORD dwDestDataSize = data.size();
		BYTE* pDestBuf = new BYTE[dwDestDataSize];
		if (pDestBuf != NULL)
		{
			int iRet = 0;
			do
			{
				iRet = compress(pDestBuf, &dwDestDataSize, (const BYTE*)data.c_str(), data.length());
				if (iRet == 0)//压缩成功
				{
					if (dwDestDataSize != 0)
					{
						result.resize(dwDestDataSize);
						memcpy((char*)result.c_str(), pDestBuf, dwDestDataSize);//将压缩后的数据放入STL堆内存中
						break;
					}
				}
				else if (iRet == -5)//输出缓冲区不够大,以4K大小递增
				{
					delete[] pDestBuf;
					pDestBuf = NULL;
					dwDestDataSize = dwDestDataSize + 4 * 1024;//4K大小递增
					pDestBuf = new BYTE[dwDestDataSize];
					if (pDestBuf == NULL)
					{
						return result;
					}
				}
				else//没有足够的内存或其它的情况
				{
					delete[] pDestBuf;
					pDestBuf = NULL;
					return result;
				}
			} while (true);
			delete[] pDestBuf;
		}
		return result;
	}


	// 将二进制解压缩
	// 输入：压缩后的二进制
	// 输出：原始字符串
	std::string UnCompressData(const std::string data)
	{
		std::string result;
		DWORD dwMaxSize = 1024 * 8;//默认大小为8K
		DWORD dwDestDataSize = dwMaxSize;
		BYTE* pDestBuf = new BYTE[dwDestDataSize];
		if (pDestBuf != NULL)
		{
			int iRet = 0;
			do
			{
				iRet = uncompress(pDestBuf, &dwDestDataSize, (BYTE*)data.c_str(), data.size());
				if (iRet == 0)//解压成功
				{
					if (dwDestDataSize != 0)
					{
						result.resize(dwDestDataSize);
						memcpy((char*)result.c_str(), pDestBuf, dwDestDataSize);//将解压后的数据放到STL堆内存中
						break;
					}
				}
				else if (iRet == -5)//输出缓冲区不够大,以4K大小递增
				{
					delete[] pDestBuf;
					pDestBuf = NULL;
					dwDestDataSize = dwDestDataSize + 4 * 1024;//4K大小递增
					pDestBuf = new BYTE[dwDestDataSize];
					if (pDestBuf == NULL)
					{
						return result;
					}
				}
				else//没有足够的内存或其它的情况
				{
					delete[] pDestBuf;
					pDestBuf = NULL;
					return result;
				}
			} while (true);
			delete[] pDestBuf;
		}
		return result;
	}
}