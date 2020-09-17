#pragma once
#include <string>
#include <stdint.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

void generateRSAKey(std::string strKey[2]);//���ɹ�˽��Կ��
std::string PubEncrypt(const std::string data, const std::string pubKey);//ʹ�ù�Կ����
std::string PriDecrypt(const std::string cipherText, const std::string priKey);//ʹ��˽Կ����
std::string PriEncrypt(const std::string data, const std::string priKey);//ʹ��˽Կ����
std::string PubDecrypt(const std::string cipherText, const std::string pubKey);//ʹ�ù�Կ����

