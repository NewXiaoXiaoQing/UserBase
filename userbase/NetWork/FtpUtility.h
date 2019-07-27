#pragma once

/*******************************************************************************
* Copyright (c) 2017, �ɶ���ʤ�Ƽ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�FtpUtility.h
* �ļ���ʶ��
* ժ����Ҫ��
* �������ߣ���С��
* ������ڣ�2017/04/20
*
* ˵����ʹ��Libcurlʵ��Ftp����ز���
******************************************************************************/
#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#if _DEBUG
#pragma comment(lib, "debug/libcurld.lib")
#else
#pragma comment(lib, "release/libcurl.lib")
#endif
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")


namespace ubase
{
	class FtpUtility
	{
	public:
		FtpUtility();
		~FtpUtility();
		bool FtpUpload(std::string ftpUrl, std::string filepath, std::string userkey, unsigned int timeout);
		bool FtpDownload(std::string ftpUrl, std::string filepath, std::string userkey, unsigned int timeout);
		bool FtpDownloadEx(std::string ftpUrl, std::string filepath, std::string userkey, unsigned int timeout);
		bool FtpDeleteFile(std::string ftpname, std::vector<std::string> folderlist, std::string remotepath, std::string userKey);
		bool FtpGetList(std::string ftpUrl, std::vector<std::string> subfolderlist, std::string userKey, std::vector<std::string> &filelist);
		bool FtpCreateDir(std::string ftpUrl, std::string userkey, std::string foldername, std::vector<std::string> folderlist);
	};
};


