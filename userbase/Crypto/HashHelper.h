#pragma once
#include <string>
#include <openssl/sha.h>
#include <openssl/md5.h>

std::string md5(const std::string &srcStr);//md5����
std::string sha1(const std::string &srcStr);//sha1����
std::string sha256(const std::string &srcStr);//sha256����