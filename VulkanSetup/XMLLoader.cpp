#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include "XMLLoader.h"
#include "XMLData.h"
#include "StringEx.h"
#include <windows.h>

//rapidxml
#include <rapidxml.hpp>


rapidxml::xml_document<char>* XMLLoader::loadXML_rapid(std::string path)
{
	std::basic_ifstream<char> xmlFile(path);
	xmlFile.seekg(0, std::ios::end);
	unsigned int size = xmlFile.tellg();
	xmlFile.seekg(0);

	std::vector<char> xmlData(size + 1);
	xmlData[size] = 0;
	xmlFile.read(&xmlData.front(), (std::streamsize)size);

	rapidxml::xml_document<char>* xmlDoc = new rapidxml::xml_document<char>;
	xmlDoc->parse<0>(&xmlData.front());

	xmlFile.close();
	return xmlDoc;
}

XMLElement* XMLLoader::startElement_c(char* data)
{
	XMLElement* elem = new XMLElement;

	StringEx::removeAll(data, '<');
	StringEx::removeAll(data, '>');

	size_t namePos = StringEx::find(data, ' ');
	size_t realSize = namePos == -1 ? strlen(data) + 1 : namePos;
	char elemName[128];// = new char[realSize + 1];
	memcpy(elemName, data, realSize);
	elemName[realSize] = '\0';
	elem->name = std::string(elemName);

	if (namePos == -1)
		return elem;

	data = &data[namePos + 1];

	std::vector<std::string> _quatVec;
	while (true)
	{
		size_t quatStart = StringEx::find(data, '\"');

		if (quatStart == -1)
			break;

		StringEx::removeTarget(data, quatStart);

		size_t quatEnd = StringEx::find(data, '\"');
		StringEx::removeTarget(data, quatEnd);

		char quatString[256];// = new char[quatEnd - quatStart + 1];
		
		strncpy(quatString, data + quatStart, quatEnd - quatStart);
		quatString[quatEnd - quatStart] = '\0';
		_quatVec.push_back(std::string(quatString));
		StringEx::removeAll(data, quatStart, quatEnd - quatStart);
	}

	std::vector<std::string> split;
	//StringEx::split()
	StringEx::split(split, data, ' ');
	for (unsigned int i = 0; i < split.size(); ++i)
	{
		//std::cout << split[i] << std::endl;
		//StringEx::remove(split[i], '=');
		if (split[i] == "" || split[i] == " ")
		{
			split.erase(split.begin() + i);
			i--;
			continue;
		}
		split[i] = StringEx::replaceAll(split[i], "=", "");

		elem->_property[split[i]] = _quatVec[i] == "" ? "" : std::string(_quatVec[i]);

		
	}


	_quatVec.clear();
	split.clear();

	return elem;
}

XMLData* XMLLoader::getData_c(char* data)
{
	XMLData* xmlData = new XMLData;

	StringEx::removeAll(data, '<');

	size_t dataStart = StringEx::find(data, '>');
	size_t dataEnd = StringEx::find(data, '/');

	char nameString[256] = { 0 };
	strncpy(nameString, data, dataStart);

	xmlData->name = std::string(nameString);

	char dataString[256] = { 0 };;
	strncpy(dataString, data + dataStart + 1, dataEnd - dataStart - 1);
	xmlData->data = std::string(dataString);


	return xmlData;
}

XMLElement* XMLLoader::loadXML_c(std::string path)
{
	clock_t start, end;
	double result;
	start = clock();

	std::ifstream readFile;
	readFile.open(path);
	if (readFile.is_open() == false)
	{
		assert("file not found");
		return nullptr;
	}

	char loadedChar[256] = { 0 };

	readFile.getline(loadedChar, 255);

	char* substr = &loadedChar[StringEx::find(loadedChar, '<')];
	XMLElement* root = startElement_c(substr);
	XMLElement* current = root;

	while (readFile.eof() == false)
	{
		readFile.getline(loadedChar, 256);
		if (loadedChar == "" || loadedChar[0] == '\0')
			continue;

		size_t findTarget = StringEx::find(loadedChar, '<');
		if (findTarget == -1)
		{
			assert("xml error");
			return nullptr;
		}

		char* substr = &loadedChar[findTarget];

		size_t slash = StringEx::find(substr, '/');
		int right = StringEx::findAll(substr, '>');

		if (substr[1] == '/')
		{
			current = current->_parent;
		}
		else if (slash != -1 && right >= 2)
		{
			current->addData(getData_c(substr));
		}
		else if (slash != -1)
		{
			StringEx::remove(substr, '/');
			XMLElement* element = startElement_c(substr);
			element->setParent(current);
		}
		else
		{
			XMLElement* element = startElement_c(substr);
			element->setParent(current);
			current = element;
		}
	}

	readFile.close();

	end = clock() - start;
	result = static_cast<double>(end);

	std::cout << "xml loaded : \"" << path << "\" " << result << "ms" << std::endl;
	return root;
}

XMLElement* XMLLoader::startElement(std::string data)
{
	XMLElement* elem = new XMLElement;

	data = StringEx::replaceAll(data, "<", "");
	data = StringEx::replaceAll(data, ">", "");

	std::vector<std::string> split;

	size_t namePos = data.find(' ');

	elem->name = data.substr(0, namePos);// split[0];
	if (namePos == std::string::npos)
		return elem;
	data = data.substr(namePos + 1);

	std::vector<std::string> _quatVec;
	while (true)
	{
		size_t quatStart = data.find('\"');

		if (quatStart == std::string::npos)
			break;

		data = data.replace(quatStart,1, "");
		size_t quatEnd = data.find('\"');
		data = data.replace(quatEnd, 1, "");

		_quatVec.push_back(data.substr(quatStart, quatEnd - quatStart));
		data = data.replace(quatStart, quatEnd - quatStart,"");
	}

	StringEx::split(split, data, ' ');

	for (unsigned int i = 0; i < split.size(); ++i)
	{
		split[i] = split[i].replace(split[i].find('='), 1, "");
		elem->_property[split[i]] = _quatVec[i];
	}

	return elem;
}

XMLData* XMLLoader::getData(std::string data)
{
	XMLData* xmlData = new XMLData;

	data = StringEx::replaceAll(data, "<", "");
	size_t dataStart = data.find('>');
	size_t dataEnd = data.find('/');

	xmlData->name = data.substr(0, dataStart);
	xmlData->data = data.substr(dataStart + 1, dataEnd - dataStart - 1);

	return xmlData;
}

XMLElement* XMLLoader::loadXML(std::string path)
{
	clock_t start, end;
	double result;
	start = clock();


	std::ifstream readFile;
	readFile.open(path);
	if (readFile.is_open() == false)
	{
		assert("file not found");
		return nullptr;
	}

	std::string line;
	getline(readFile, line);

	XMLElement* root = startElement(line);
	XMLElement* current = root;

	int elemDepth = 0;

	while (readFile.eof() == false)
	{
		std::string line;
		getline(readFile, line);

		if (line == "" || line[0] == '\0')
			break;

		line = line.substr(line.find('<'));
		if (line[1] == '/')
		{
			//line end
			current = current->_parent;
			
		}
		else if (line.find('/') != std::string::npos)
		{
			//data
			current->addData(getData(line));
			
		}
		else
		{
			//line start
			XMLElement* element = startElement(line);
			element->setParent(current);
			current = element;
		}

	}

	end = clock() - start;
	result = static_cast<double>(end);

	std::cout << "xml loaded : \"" << path << "\" " << result << "ms" << std::endl;
	return root;
}