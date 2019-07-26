#include "../stdafx.h"
#include "UserProcess.h"

namespace ubase
{
	bool EnableDebugPriv()
	{
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &seDebugNameValue))
		{
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool EnableShutdownPriv()
	{
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &seDebugNameValue))
		{
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool EnableBackupPriv()
	{
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &seDebugNameValue))
		{
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool EnableRestorePriv()
	{
		HANDLE hToken;
		LUID seDebugNameValue;
		TOKEN_PRIVILEGES tkp;

		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return false;

		if (!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &seDebugNameValue))
		{
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = seDebugNameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
			CloseHandle(hToken);

		return true;
	}

	bool PsGetPidsByProcessName(std::vector<DWORD> &vDwPids, const std::string &processName)
	{
		vDwPids.clear();
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return false;
		}

		PROCESSENTRY32 pe = { sizeof(pe) };
		for (BOOL fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
		{
			if (!_strcmpi(pe.szExeFile, processName.c_str()))
			{
				DWORD pid = pe.th32ProcessID;
				vDwPids.push_back(pid);
			}
		}
		CloseHandle(hSnapshot);

		if (vDwPids.size() != 0)
			return true;

		return false;
	}

	bool PsGetAllProcessName(std::vector<std::string> &vsProcessName)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return false;
		}

		PROCESSENTRY32 pe = { sizeof(pe) };
		for (BOOL bOk = Process32First(hSnapshot, &pe); bOk; bOk = Process32Next(hSnapshot, &pe))
		{
			vsProcessName.push_back(pe.szExeFile);
		}
		CloseHandle(hSnapshot);
		if (vsProcessName.size() != 0)
			return true;
		return false;
	}

	bool RemoteThreadInjectDll(DWORD dwPid, char* szPath)
	{
		bool bResult = false;
		//1 ��Ŀ�����������һ���ռ�
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		if (hProcess != NULL)
		{
			LPVOID pRemoteAddress = VirtualAllocEx(
				hProcess,
				NULL,
				1,
				MEM_COMMIT,
				PAGE_READWRITE
			);
			if (pRemoteAddress != NULL)
			{
				//2 ��dll��·��д�뵽�Է��Ľ��̿ռ���
				DWORD dwWriteSize = 0;
				//дһ�����ݵ�ָ�����������ٵ��ڴ�ռ�
				WriteProcessMemory(hProcess, pRemoteAddress, szPath, strlen(szPath) + 1, &dwWriteSize);

				//3 ����һ��Զ���̣߳���Ŀ����̵���LoadLibrary
				HANDLE hThread = CreateRemoteThread(
					hProcess,
					NULL,
					0,
					(LPTHREAD_START_ROUTINE)LoadLibraryA,
					pRemoteAddress,
					NULL,
					NULL
				);
				if (hThread != NULL)
				{
					bResult = true;
				}
			}
		}
		return bResult;
	}

	HMODULE FindRemoteMod(HANDLE hProcess, const wchar_t * pszModShortName)
	{
		HMODULE hModules[0x100] = { 0 };
		int cbNeeded = 0;
		if (!EnumProcessModules(hProcess
			, hModules
			, sizeof(hModules)
			, (unsigned long *)&cbNeeded))
			return (HMODULE)0;

		for (int i = 0; i < (cbNeeded >> 2); i++)
		{
			wchar_t szModuleName[0x200] = { 0 };
			GetModuleBaseNameW(hProcess
				, hModules[i]
				, szModuleName
				, sizeof(szModuleName) / sizeof(szModuleName[0]));

			if (!_wcsicmp(szModuleName, pszModShortName))
				return hModules[i];
		}
		return (HMODULE)0;
	}

	bool RemoteThreadUnloadDll(DWORD dwPid, wchar_t *dllName)
	{
		bool bResult = false;
		// �򿪽��̣��߱�д��Ȩ�޺ʹ����߳�Ȩ�� 
		HANDLE hProcess = OpenProcess(PROCESS_VM_READ
			| PROCESS_VM_WRITE
			| PROCESS_VM_OPERATION
			| PROCESS_SUSPEND_RESUME
			| PROCESS_CREATE_THREAD
			| PROCESS_QUERY_INFORMATION
			, FALSE
			, dwPid);
		if (NULL == hProcess)
			return bResult;

		// ����Ƿ���ͬһ���û��Ự�еĽ���
		HANDLE hToken = INVALID_HANDLE_VALUE;
		if (!OpenProcessToken(hProcess, TOKEN_READ, &hToken))
		{
			CloseHandle(hProcess);
			return bResult;
		}
		CloseHandle(hToken);

		HMODULE hDllModule = FindRemoteMod(hProcess, dllName);

		if (hDllModule != 0)
		{
			// ��FreeLibraryΪ�߳��崴���̣߳�ж��Ŀ������е��ض�DLL. 
			unsigned long tid = 0;
			CreateRemoteThread(hProcess
				, 0, 0
				, (LPTHREAD_START_ROUTINE)FreeLibrary
				, hDllModule
				, 0
				, &tid);
			bResult = true;
		}
		CloseHandle(hProcess);
		hProcess = 0;
		return bResult;
	}

	bool ZwCreateThreadExInjectDll(DWORD dwProcessId, char *pszDllFileName)
	{
		HANDLE hProcess = NULL;
		SIZE_T dwSize = 0;
		LPVOID pDllAddr = NULL;
		FARPROC pFuncProcAddr = NULL;
		HANDLE hRemoteThread = NULL;
		DWORD dwStatus = 0;

		// ��ע����̣���ȡ���̾��
		hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
		if (NULL == hProcess)
		{
			return false;
		}
		// ��ע������������ڴ�
		dwSize = 1 + ::lstrlen(pszDllFileName);
		pDllAddr = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
		if (NULL == pDllAddr)
		{
			return false;
		}
		// ��������ڴ���д������
		if (FALSE == ::WriteProcessMemory(hProcess, pDllAddr, pszDllFileName, dwSize, NULL))
		{
			return false;
		}
		// ���� ntdll.dll
		HMODULE hNtdllDll = ::LoadLibrary("ntdll.dll");
		if (NULL == hNtdllDll)
		{
			return false;
		}
		// ��ȡLoadLibraryA������ַ
		pFuncProcAddr = ::GetProcAddress(::GetModuleHandle("Kernel32.dll"), "LoadLibraryA");
		if (NULL == pFuncProcAddr)
		{
			return false;
		}
		// ��ȡZwCreateThread������ַ
#ifdef _WIN64
		typedef DWORD(WINAPI *typedef_ZwCreateThreadEx)(
			PHANDLE ThreadHandle,
			ACCESS_MASK DesiredAccess,
			LPVOID ObjectAttributes,
			HANDLE ProcessHandle,
			LPTHREAD_START_ROUTINE lpStartAddress,
			LPVOID lpParameter,
			ULONG CreateThreadFlags,
			SIZE_T ZeroBits,
			SIZE_T StackSize,
			SIZE_T MaximumStackSize,
			LPVOID pUnkown);
#else
		typedef DWORD(WINAPI *typedef_ZwCreateThreadEx)(
			PHANDLE ThreadHandle,
			ACCESS_MASK DesiredAccess,
			LPVOID ObjectAttributes,
			HANDLE ProcessHandle,
			LPTHREAD_START_ROUTINE lpStartAddress,
			LPVOID lpParameter,
			BOOL CreateSuspended,
			DWORD dwStackSize,
			DWORD dw1,
			DWORD dw2,
			LPVOID pUnkown);
#endif
		typedef_ZwCreateThreadEx ZwCreateThreadEx = (typedef_ZwCreateThreadEx)::GetProcAddress(hNtdllDll, "ZwCreateThreadEx");
		if (NULL == ZwCreateThreadEx)
		{
			return false;
		}
		// ʹ�� ZwCreateThreadEx ����Զ�߳�, ʵ�� DLL ע��
		dwStatus = ZwCreateThreadEx(&hRemoteThread, PROCESS_ALL_ACCESS, NULL, hProcess, (LPTHREAD_START_ROUTINE)pFuncProcAddr, pDllAddr, 0, 0, 0, 0, NULL);
		if (NULL == hRemoteThread)
		{
			return false;
		}
		// �رվ��
		::CloseHandle(hProcess);
		::FreeLibrary(hNtdllDll);

		return true;
	}
}


