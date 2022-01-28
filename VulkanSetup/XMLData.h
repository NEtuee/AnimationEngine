#pragma once
#include <unordered_map>
#include <vector>
#include <string>

class XMLData
{
public:
	std::string name;
	std::string data;
};

class XMLElement
{
public:
	void			setParent(XMLElement* parent);
	void			addData(XMLData* data);
	XMLData*		findData(std::string target);
	XMLElement*		findElement(std::string target);
	std::string		getProperty(std::string target);
	void			destroyChildren();
public:
	std::string name;

	std::unordered_map<std::string, std::string> _property;
	std::vector<XMLElement*>	_childs;
	std::vector<XMLData*>		_data;

	XMLElement* _parent;
private:
	void destroyStructure(XMLElement* root);
};
