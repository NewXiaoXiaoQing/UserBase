#pragma once
#include <iostream>
#include <windows.h>
#include <Wincrypt.h>

namespace ubase
{
	bool IntGetRandomUInt(unsigned int &randomNum);//��ȡһ���������
	void ConvertUIntToBytes(unsigned int number, unsigned char numBytes[], bool bigEndian);//��unsigned intתΪbytes
	unsigned int ConvertBytesToUInt(unsigned char numBytes[], bool bigEndian);//��bytesתΪunsigned int
}
