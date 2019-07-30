#pragma once
#include <string>
#include <windows.h>

namespace ubase
{
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
		std::string GetOrinalFunctionContent(const char* pFile, const char* findName, unsigned int count, bool bin2Hex = true);
	};
}
