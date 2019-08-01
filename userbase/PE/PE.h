#pragma once
#include <string>
#include <map>
#include <vector>
#include <windows.h>

namespace ubase
{
	typedef struct _MAP_FILE_STRUCT
	{
		HANDLE hFile;//�ļ����
		HANDLE hMapping;//ӳ���ļ����
		LPVOID imageBase;//ӳ���ַ
	}MAP_FILE_STRUCT;

	typedef struct _PE_SECTION_INFO
	{
		CHAR sectionName[9];
		DWORD virtualSize;
		DWORD virtualAddress;
		DWORD pointerToRawData;
		DWORD sizeOfRawData;
		DWORD characteristics;
	}PE_SECTION_INFO;

	typedef struct _PE_IMPORT_DESC
	{
		UINT dllIndex;
		CHAR dllName[MAX_PATH];
		DWORD originalFirstThunk;
		DWORD timeDateStamp;
		DWORD forwarderChain;
		DWORD firstThunk;
	}PE_IMPORT_DESC;

	typedef struct _PE_IMPORT_FUNC
	{
		DWORD thunkRVA;
		DWORD thunkValue;
		WORD hint;
		CHAR funcName[MAX_PATH];
	}PE_IMPORT_FUNC;

	class PE
	{
	public:
		PE();
		~PE();

		// �ڴ�ƫ��ת�ļ�ƫ��(����ɹ����ط�0)
		DWORD RvaToOffset(PIMAGE_NT_HEADERS pNTHeader, DWORD dwExportRVA);

		// ��ȡDLL����������ԭʼ����
		// pFile:dll�ļ��Ķ���������
		// count:��Ҫ��ȡ�������ֽ���(�Ӻ���ͷ��ʼ)
		// bin2Hex:��ȡ����Ƿ���16���Ƶķ�ʽչʾ
		std::string GetOrinalFunctionContent(PVOID imageBase, const char* findName, unsigned int count, bool bin2Hex = true);

		// ��ȡdll�ļ�ָ�����Ƶĺ������ļ�ƫ��
		// �����ȡ�ɹ�,�򷵻ط�0
		DWORD PE::GetExportFunOffsetByName(PVOID imageBase, const char* findName);

		// ��ȡָ��dll�ļ�������������ʽ�ĵ��������ĺ��������ļ�ƫ��
		void PE::GetAllExportNamedFunctionsOffset(PVOID imageBase, std::map<std::string, DWORD>& mpFunctions);

		// �ж��Ƿ���PE��ʽ
		bool IsPEFile(LPVOID imageBase);

		// ��ȡDOSͷ��
		PIMAGE_DOS_HEADER GetDosHeader(LPVOID imageBase);

		// ��ȡNTͷ��
		PIMAGE_NT_HEADERS GetNtHeaders(LPVOID imageBase);

		// ��ȡ�ļ�ͷ
		PIMAGE_FILE_HEADER GetFileHeader(LPVOID imageBase);

		// ��ȡ��ѡͷ
		PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID imageBase);

		// ��ȡ��һ���ڵ�λ��
		PIMAGE_SECTION_HEADER GetFirstSectionHeader(PIMAGE_NT_HEADERS pNtHeader);

		// ��ȡ�ڿ�Ļ�����Ϣ
		bool GetSectionHeaderInfo(LPVOID imageBase, std::vector<PE_SECTION_INFO>& vtSectionInfo);

		// ��ȡ����Ŀ¼�����ڵ�ַ
		LPVOID GetDirectoryEntryToData(LPVOID imageBase, USHORT directoryEntry);

		// ��ȡ��һ�������ṹ
		PIMAGE_IMPORT_DESCRIPTOR GetFirstImportDesc(LPVOID imageBase);

		// ��ȡ�����Ļ�����Ϣ
		bool GetImportDescInfo(LPVOID imageBase, std::vector<PE_IMPORT_DESC>& vtImportDesc);

		// ͨ��DllIndex��ȡ���뺯���б�
		bool GetImportFuncByDllIndex(LPVOID imageBase, UINT dllIndex, std::vector<PE_IMPORT_FUNC>& vtImportFunc);
	};
}
