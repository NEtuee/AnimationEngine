#pragma once
#include <string>
#include <vector>
#include "DirectXDefine.h"

namespace StringEx
{
	XMFLOAT3			dataToFloat3(std::string data);
	XMFLOAT4			dataToFloat4(std::string data);
	void				split(std::vector<char*>& result, char* input, char delimiter);
	void				removeAll(char* str, char target);
	void				removeAll(char* str, size_t from, size_t to);
	void				remove(char* str, char target);
	void				removeTarget(char* str, size_t target);
	size_t				find(const char* str, char target);
	int					findAll(const char* str, char target);
	void				split(std::vector<std::string>& result, std::string input, char delimiter);
	std::string			replaceAll(const std::string& target, const std::string& pattern, const std::string& replace);
}