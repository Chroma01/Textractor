#pragma once

#include <string>

std::string Md5(const std::string& text);
std::string Md5Raw(const std::string& text);
std::string Base64Encode(const std::string& data);
std::string HmacSha1(const std::string& key, const std::string& data);
std::string Base64Encode(const BYTE* data, DWORD size);

