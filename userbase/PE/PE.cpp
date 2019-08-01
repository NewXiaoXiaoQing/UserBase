#include "PE.h"
#include "../String/UserString.h"
#include <imagehlp.h>

#pragma comment(lib, "Imagehlp.lib")

namespace ubase
{
	PE::PE()
	{

	}

	PE::~PE()
	{

	}

	DWORD PE::RvaToOffset(PIMAGE_NT_HEADERS pNTHeader, DWORD dwExportRVA)
	{
		PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)((DWORD)pNTHeader + sizeof(IMAGE_NT_HEADERS));
		for (int i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++)
		{
			if (dwExportRVA >= pSection[i].VirtualAddress && dwExportRVA < (pSection[i].VirtualAddress + pSection[i].SizeOfRawData))
			{
				return pSection[i].PointerToRawData + (dwExportRVA - pSection[i].VirtualAddress);
			}
		}
		return 0;
	}

	std::string PE::GetOrinalFunctionContent(PVOID imageBase, const char* findName, unsigned int count, bool bin2Hex)
	{
		std::string result;
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)imageBase;//��ȡDOSͷ
		PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)((DWORD)imageBase + pDosHeader->e_lfanew);//��ȡNTͷ
		DWORD dwExportOffset = RvaToOffset(pNTHeader, pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		if (dwExportOffset != 0)
		{
			PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)((DWORD)imageBase + dwExportOffset);
			DWORD dwFunNameOffset = (DWORD)imageBase + RvaToOffset(pNTHeader, pExport->Name);
			DWORD* pDwNamesAddress = (DWORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfNames));
			DWORD* pdwFunctionAddress = (DWORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfFunctions));
			WORD* pwOrdinals = (WORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfNameOrdinals));

			for (int i = 0; i < pExport->NumberOfNames; i++)
			{
				DWORD dwFunctionAddress = pdwFunctionAddress[pwOrdinals[i]];
				DWORD pdwFunNameOffset = RvaToOffset(pNTHeader, pDwNamesAddress[i]);
				char* funName = (char*)((DWORD)imageBase + pdwFunNameOffset);//��������
				//�ԱȺ��������Ƿ����������ҵĺ�����
				if (_stricmp(funName, findName) == 0)//����������������ҵĺ���
				{
					DWORD dwFunctionOffset = RvaToOffset(pNTHeader, dwFunctionAddress);//��ȡ�����������ļ�ƫ��
					result.resize(count);
					memcpy((char*)result.c_str(), (char*)((DWORD)imageBase + dwFunctionOffset), count);
					if (bin2Hex == true)
					{
						result = ubase::BinToHex(result, true);
					}
					break;
				}
			}
		}
		return result;
	}

	DWORD PE::GetExportFunOffsetByName(PVOID imageBase, const char* findName)
	{
		DWORD offset = 0;
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)imageBase;//��ȡDOSͷ
		PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)((DWORD)imageBase + pDosHeader->e_lfanew);//��ȡNTͷ
		DWORD dwExportOffset = RvaToOffset(pNTHeader, pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		if (dwExportOffset != 0)
		{
			PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)((DWORD)imageBase + dwExportOffset);
			DWORD dwFunNameOffset = (DWORD)imageBase + RvaToOffset(pNTHeader, pExport->Name);
			DWORD* pDwNamesAddress = (DWORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfNames));
			DWORD* pdwFunctionAddress = (DWORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfFunctions));
			WORD* pwOrdinals = (WORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfNameOrdinals));

			for (int i = 0; i < pExport->NumberOfNames; i++)
			{
				DWORD dwFunctionAddress = pdwFunctionAddress[pwOrdinals[i]];
				DWORD pdwFunNameOffset = RvaToOffset(pNTHeader, pDwNamesAddress[i]);
				char* funName = (char*)((DWORD)imageBase + pdwFunNameOffset);//��������
				//�ԱȺ��������Ƿ����������ҵĺ�����
				if (_stricmp(funName, findName) == 0)//����������������ҵĺ���
				{
					offset = RvaToOffset(pNTHeader, dwFunctionAddress);//��ȡ���ҵĵ����������ļ�ƫ��
					break;
				}
			}
		}
		return offset;
	}

	void PE::GetAllExportNamedFunctionsOffset(PVOID imageBase, std::map<std::string, DWORD>& mpFunctions)
	{
		bool result = false;
		PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)imageBase;//��ȡDOSͷ
		PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)((DWORD)imageBase + pDosHeader->e_lfanew);//��ȡNTͷ
		DWORD dwExportOffset = RvaToOffset(pNTHeader, pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		if (dwExportOffset != 0)
		{
			PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)((DWORD)imageBase + dwExportOffset);
			DWORD dwFunNameOffset = (DWORD)imageBase + RvaToOffset(pNTHeader, pExport->Name);
			DWORD* pDwNamesAddress = (DWORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfNames));
			DWORD* pdwFunctionAddress = (DWORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfFunctions));
			WORD* pwOrdinals = (WORD*)((DWORD)imageBase + RvaToOffset(pNTHeader, pExport->AddressOfNameOrdinals));

			for (int i = 0; i < pExport->NumberOfNames; i++)
			{
				DWORD dwFunctionAddress = pdwFunctionAddress[pwOrdinals[i]];//��ȡ��Ӧ���ƺ�����RVA
				DWORD pdwFunNameOffset = RvaToOffset(pNTHeader, pDwNamesAddress[i]);//��ȡ�洢�������Ƶ�RVA
				char* funName = (char*)((DWORD)imageBase + pdwFunNameOffset);//��ȡ����������
				DWORD fileOffset= RvaToOffset(pNTHeader, dwFunctionAddress);//��ȡ�������ļ�ƫ��
				mpFunctions[funName] = fileOffset;//�洢����������������Ϣ
			}
		}
	}

	bool PE::IsPEFile(LPVOID imageBase)
	{
		PIMAGE_DOS_HEADER pDosHeader = NULL;
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		if (imageBase == NULL)
			return false;

		pDosHeader = (PIMAGE_DOS_HEADER)imageBase;//��ȡDOSͷ��
		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)//�ж��Ƿ���MZ
			return false;

		pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);//��ȡNTͷ��
		if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)//�ж��Ƿ�ΪPE��ʽ
			return false;

		return true;
	}

	PIMAGE_DOS_HEADER PE::GetDosHeader(LPVOID imageBase)
	{
		PIMAGE_DOS_HEADER pDosHeader = NULL;

		if (!IsPEFile(imageBase))
			return NULL;

		pDosHeader = (PIMAGE_DOS_HEADER)imageBase;
		return pDosHeader;
	}

	PIMAGE_NT_HEADERS PE::GetNtHeaders(LPVOID imageBase)
	{
		PIMAGE_DOS_HEADER pDosHeader = NULL;
		PIMAGE_NT_HEADERS pNtHeader = NULL;

		if (!IsPEFile(imageBase))
			return NULL;

		pDosHeader = (PIMAGE_DOS_HEADER)imageBase;
		pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
		return pNtHeader;
	}

	PIMAGE_FILE_HEADER PE::GetFileHeader(LPVOID imageBase)
	{
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PIMAGE_FILE_HEADER pFileHeader = NULL;
		pNtHeader = GetNtHeaders(imageBase);
		if (!pNtHeader)
			return NULL;
		pFileHeader = &pNtHeader->FileHeader;
		return pFileHeader;
	}

	PIMAGE_OPTIONAL_HEADER PE::GetOptionalHeader(LPVOID imageBase)
	{
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
		pNtHeader = GetNtHeaders(imageBase);
		if (!pNtHeader)
			return NULL;
		pOptionalHeader = &pNtHeader->OptionalHeader;
		return pOptionalHeader;
	}

	PIMAGE_SECTION_HEADER PE::GetFirstSectionHeader(PIMAGE_NT_HEADERS pNtHeader)
	{
		PIMAGE_SECTION_HEADER pSectionHeader = NULL;
		pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);
		return pSectionHeader;
	}

	bool PE::GetSectionHeaderInfo(LPVOID imageBase, std::vector<PE_SECTION_INFO>& vtSectionInfo)
	{
		PIMAGE_NT_HEADERS pNtHeader = GetNtHeaders(imageBase);
		if (!pNtHeader)
			return false;

		PIMAGE_FILE_HEADER pFileHeader = &pNtHeader->FileHeader;
		if (!pFileHeader)
			return false;

		PIMAGE_SECTION_HEADER pSectionHeader = GetFirstSectionHeader(pNtHeader);
		if (!pSectionHeader)
			return false;

		for (int i = 0; i < pFileHeader->NumberOfSections; i++)
		{
			PE_SECTION_INFO item = { 0 };

			//������
			char cName[9];
			memset(cName, 0, sizeof(cName));
			memcpy(cName, pSectionHeader->Name, 8);

			//��ʵ����
			DWORD virtualSize = pSectionHeader->Misc.VirtualSize;
			//������ƫ��(RVA)
			DWORD virtualAddress = pSectionHeader->VirtualAddress;
			//���ļ�ƫ��(Offset)
			DWORD pointerToRawData = pSectionHeader->PointerToRawData;
			//�ڴ�С
			DWORD sizeOfRawData = pSectionHeader->SizeOfRawData;
			//������,�ɶ�����д����ִ��
			DWORD characteristics = pSectionHeader->Characteristics;

			memcpy(item.sectionName, cName, 8);
			item.virtualSize = virtualSize;
			item.virtualAddress = virtualAddress;
			item.pointerToRawData = pointerToRawData;
			item.sizeOfRawData = sizeOfRawData;
			item.characteristics = characteristics;

			vtSectionInfo.push_back(item);

			++pSectionHeader;
		}

		if (vtSectionInfo.size() != 0)
			return true;

		return false;
	}

	LPVOID PE::GetDirectoryEntryToData(LPVOID imageBase, USHORT directoryEntry)
	{
		DWORD dwDataStartRVA;
		LPVOID pDirData = NULL;
		PIMAGE_NT_HEADERS     pNtHeader = NULL;
		PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;

		pNtHeader = GetNtHeaders(imageBase);
		if (!pNtHeader)
			return NULL;
		pOptionalHeader = GetOptionalHeader(imageBase);
		if (!pOptionalHeader)
			return NULL;
		dwDataStartRVA = pOptionalHeader->DataDirectory[directoryEntry].VirtualAddress;
		if (!dwDataStartRVA)
			return NULL;

		pDirData = ImageRvaToVa(pNtHeader, imageBase, dwDataStartRVA, NULL);
		if (!pDirData)
			return NULL;
		return  pDirData;
	}

	PIMAGE_IMPORT_DESCRIPTOR PE::GetFirstImportDesc(LPVOID imageBase)
	{
		PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
		pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)GetDirectoryEntryToData(imageBase, IMAGE_DIRECTORY_ENTRY_IMPORT);
		if (!pImportDesc)
			return NULL;
		return pImportDesc;
	}

	bool PE::GetImportDescInfo(LPVOID imageBase, std::vector<PE_IMPORT_DESC>& vtImportDesc)
	{
		bool result = false;
		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;

		pNtHeader = GetNtHeaders(imageBase);
		if (!pNtHeader)
			return result;

		pImportDesc = GetFirstImportDesc(imageBase);
		if (!pImportDesc)
			return result;

		unsigned int index = 0;
		while (pImportDesc->FirstThunk)
		{
			PE_IMPORT_DESC item = { 0 };

			char* szDllName = (char*)ImageRvaToVa(pNtHeader, imageBase, pImportDesc->Name, NULL);//��ȡ����DLL����
			DWORD originalFirstThunk = pImportDesc->OriginalFirstThunk;
			DWORD timeDateStamp = pImportDesc->TimeDateStamp;
			DWORD forwarderChain = pImportDesc->ForwarderChain;
			DWORD firstThunk = pImportDesc->FirstThunk;
			
			item.dllIndex = index;
			strcpy(item.dllName, szDllName);
			item.originalFirstThunk = originalFirstThunk;
			item.timeDateStamp = timeDateStamp;
			item.forwarderChain = forwarderChain;
			item.firstThunk = firstThunk;

			vtImportDesc.push_back(item);

			index++;
			pImportDesc++;
		}

		if (vtImportDesc.size() != 0)
			result = true;

		return result;
	}

	DWORD SelectThunk(PIMAGE_IMPORT_DESCRIPTOR pImportDesc)
	{
		return pImportDesc->OriginalFirstThunk ? pImportDesc->OriginalFirstThunk : pImportDesc->FirstThunk;
	}

	bool PE::GetImportFuncByDllIndex(LPVOID imageBase, UINT dllIndex, std::vector<PE_IMPORT_FUNC>& vtImportFunc)
	{
		bool result = false;

		PIMAGE_NT_HEADERS pNtHeader = NULL;
		PIMAGE_IMPORT_DESCRIPTOR pFirstImportDesc = NULL, pCurrentImportDesc = NULL;
		PIMAGE_IMPORT_BY_NAME pByName = NULL;
		DWORD dwThunk, *pdwThunk = NULL, *pdwRVA = NULL;
		CHAR cOrd[30], cMemAddr[30];

		pNtHeader = GetNtHeaders(imageBase);
		if (!pNtHeader)
			return result;

		pFirstImportDesc = GetFirstImportDesc(imageBase);
		if (!pFirstImportDesc)
			return result;

		pCurrentImportDesc = &pFirstImportDesc[dllIndex];
		dwThunk = SelectThunk(pCurrentImportDesc);
		pdwRVA = (DWORD*)dwThunk;//OriginalFirstThunk��FirstThunkָ���IMAGE_THUNK_DATA�ṹ�����RVA
		pdwThunk = (DWORD*)ImageRvaToVa(pNtHeader, imageBase, dwThunk, NULL);//OriginalFirstThunk��FirstThunkָ���IMAGE_THUNK_DATA�ṹ������ļ�ƫ��

		if (!pdwThunk)
			return result;

		while (*pdwThunk)
		{
			PE_IMPORT_FUNC item = { 0 };
			item.thunkRVA = (DWORD)pdwRVA;//IAT��ַ��RVA
			item.thunkValue = (DWORD)(*pdwThunk);//IAT��ַ������
			
			if (HIWORD(*pdwThunk) == 0x8000)
			{
				wsprintf(item.funcName, "Ord:%08lX", IMAGE_ORDINAL32(*pdwThunk));
			}
			else 
			{
				PIMAGE_IMPORT_BY_NAME pByName=(PIMAGE_IMPORT_BY_NAME)ImageRvaToVa(pNtHeader, imageBase, (DWORD)(*pdwThunk), NULL);
				if (pByName)
				{
					item.hint = pByName->Hint;//������
					wsprintf(item.funcName, (char*)pByName->Name);//��������
				}
				else
				{
					wsprintf(item.funcName, "MemAddr:%08lX", (DWORD)(*pdwThunk));
				}
			}
			vtImportFunc.push_back(item);
			pdwRVA++;
			pdwThunk++;
		}

		if (vtImportFunc.size() != 0)
			result = true;

		return result;
	}
}