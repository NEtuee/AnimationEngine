#define _CRT_SECURE_NO_WARNINGS

#include "StringEx.h"
#include <sstream>

namespace StringEx
{
	XMFLOAT3 dataToFloat3(std::string data)
	{
		std::vector<std::string> split;
		StringEx::split(split, data, ' ');

		XMFLOAT3 result{ 0.f,0.f,0.f };

		result.x = std::stof(split[0]);
		result.y = std::stof(split[1]);
		result.z = std::stof(split[2]);

		return result;
	}
	XMFLOAT4 dataToFloat4(std::string data)
	{
		std::vector<std::string> split;
		StringEx::split(split, data, ' ');

		XMFLOAT4 result{ 0.f,0.f,0.f,0.f };

		result.x = std::stof(split[0]);
		result.y = std::stof(split[1]);
		result.z = std::stof(split[2]);
		result.w = std::stof(split[3]);

		return result;
	}
	void split(std::vector<char*>& result, char* input, char delimiter)
	{
		char* temp = NULL;
		char* tok = strtok(input, &delimiter);
		while (tok != NULL)
		{
			result.push_back(tok);
			tok = strtok(NULL, &delimiter);
		}
	}
	void removeAll(char* str, char target)
	{
		size_t size = strlen(str);
		size_t offset = 0;

		for (size_t i = 0; i < size - offset; ++i)
		{
			while (str[i + offset] == target) 
			{ 
				++offset; 
				if (i + offset >= size)
					break;
			}

			if(offset == 0)
				continue;

			str[i] = str[i + offset];
		}

		str[size - offset] = '\0';
	}

	void removeAll(char* str, size_t from, size_t to)
	{
		size_t size = strlen(str);
		size_t offset = to;

		for (size_t i = from; i < size - offset; ++i)
		{
			str[i] = str[i + offset];
		}

		str[size - offset] = '\0';
	}

	void remove(char* str, char target)
	{
		size_t findTarget = find(str, target);
		if (findTarget == -1)
			return;

		size_t size = strlen(str);

		for (size_t i = findTarget; i < size; ++i)
		{
			str[i] = str[i + 1];
		}
	}

	void removeTarget(char* str, size_t target)
	{
		if (target == -1)
			return;

		size_t size = strlen(str);

		for (size_t i = target; i < size; ++i)
		{
			str[i] = str[i + 1];
		}
	}

	size_t find(const char* str, char target)
	{
		size_t result = 0;
		while (str[result] != '\0')
		{
			if (str[result] == target)
				return result;
			else
				++result;
		}
		result = -1;

		return result;
	}
	int findAll(const char* str, char target)
	{
		int i = 0;
		int result = 0;

		while (str[i] != '\0')
		{
			if (str[i++] == target)
				result++;

		}

		return result;
	}
	void split(std::vector<std::string>& result, std::string input, char delimiter)
	{
		std::stringstream ss(input);
		std::string temp;

		while (getline(ss, temp, delimiter)) result.push_back(temp);
	}

	std::string replaceAll(const std::string& target, const std::string& pattern, const std::string& replace)
	{
		std::string result = target;
		std::string::size_type pos = 0;

		while ((pos = result.find(pattern)) != std::string::npos)
		{
			result.replace(pos, pattern.size(), replace);
		}

		return result;
	}
}