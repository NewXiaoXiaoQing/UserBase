#pragma once
#include "../stdafx.h"
#include <windows.h>

namespace ubase
{
	class ShareMemory
	{
	public:
		// ���캯������������
		ShareMemory(const char* pszMapName, int nFileSize = 0, BOOL bServer = FALSE);
		~ShareMemory();
		// ����
		LPVOID GetBuffer() const { return  m_pBuffer; }
		HANDLE	m_hFileMap;
		// ʵ��
	private:
		LPVOID	m_pBuffer;
	};

	ShareMemory::ShareMemory(const char* pszMapName,
		int nFileSize, BOOL bServer) : m_hFileMap(NULL), m_pBuffer(NULL)
	{
		if (bServer)
		{
			// ����һ���ڴ�ӳ���ļ�����
			m_hFileMap = CreateFileMappingA(INVALID_HANDLE_VALUE,
				NULL, PAGE_READWRITE, 0, nFileSize, pszMapName);
		}
		else
		{
			// ��һ���ڴ�ӳ���ļ�����
			m_hFileMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, pszMapName);
		}

		// ӳ�������ڴ棬ȡ�ù����ڴ���׵�ַ
		m_pBuffer = (LPBYTE)MapViewOfFile(
			m_hFileMap,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			0
		);
	}

	ShareMemory::~ShareMemory()
	{
		// ȡ���ļ���ӳ�䣬�ر��ļ�ӳ�������
		UnmapViewOfFile(m_pBuffer);
		CloseHandle(m_hFileMap);
	}
}