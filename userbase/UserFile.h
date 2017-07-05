#pragma once
#include <string>
#include <vector>
#include <Shlwapi.h>
#include <shlobj.h>
#include <io.h>

namespace ubase
{
	//ϵͳ����·���Ļ�ȡ(·������б��)
	std::string FsGetDesktopPath();//��ȡ�ļ��еľ���·��
	std::string FsGetDocumentPath();//��ȡ�ĵ��ļ��еľ���·��
	std::string FsGetProgramPath();//��ȡӦ�ó���װ·��
	std::string FsGetTempPath();//��ȡ��ʱ�ļ���
	std::string FsGetWindowsPath();//��ȡwindows�ļ���
	std::string FsGetSystem32Path();//��ȡSymtem32�ļ���
	std::string FsGetCSIDLPath(IN int nCSIDL);//��ȡ����·��--SHGetSpecialFolderPathʵ��

	//���ļ�·���йصĲ���
	std::string FsGetProcessPath();//��ȡ���̵�·������б��
	std::string FsGetProcessFullPath();//��ȡ���̵�ȫ·���������̵�PE�ļ�
	std::string FsGetFileName(IN const std::string& strFilePath);//�����ļ���ȫ·����ȡ�ļ�������
	std::string FsGetFileExt(IN const std::string& strFilePath);//��ȡ�ļ�����չ��
	std::string FsGetFilePath(IN const std::string& strFilePath);//ͨ���ļ�ȫ·����ȡ�ļ���·��,��ȡ��·������б��
	void FsAddBackslash(IN OUT std::string& strDir);//·��������Ϸ�б��
	void FsDelFillBackslash(IN OUT std::string& strDir);//ȥ��·������ķ�б��
	bool FsIsDir(IN const std::string& strPath);//�ж�·���Ƿ����ļ���
	bool FsIsFile(IN const std::string& strFilePath);//�ж�·���Ƿ����ļ�

	//���ļ��������
	__int64 FsGetFileSize(IN const std::string& strFileName);//��ȡ�ļ��Ĵ�С,��λ��longlong����__int64����,�ܹ���ʾ����4G���ļ���С
	LONGLONG FsGetModiftyTime(IN const std::string& strFile);//��ȡ�ļ��޸ĵ�ʱ��(ʱ����UNIXʱ���)
	LONGLONG FsGetCreateTime(IN const std::string& strFile);//��ȡ�ļ�������ʱ��(ʱ����UNIXʱ���)
	LONGLONG FsGetAccessTime(IN const std::string& strFile);//��ȡ�ļ������ʵ�ʱ��(ʱ����UNIXʱ���)
	

	//�ļ�����
	bool FsDeleteFileEx(IN const std::string& strFileName);//ɾ���ļ�(���ɾ�������᳢�������ļ�����,Ȼ���ٴ�ɾ��)
	bool FsMoveFileEx(IN const std::string& strSrc, IN OUT std::string& strDst);//�ƶ��ļ�
	bool FsForceCreateDir(IN const std::string& strDir);//ѭ�������ļ���
	void FsForceDeleteDir(IN const std::string& strDir);//ѭ��ɾ���ļ���
};
