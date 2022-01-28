#include "XMLData.h"

void XMLElement::setParent(XMLElement* parent)
{
	_parent = parent;
	_parent->_childs.push_back(this);
}

void XMLElement::addData(XMLData* data)
{
	_data.push_back(data);
}

XMLData* XMLElement::findData(std::string target)
{
	for (auto iter = _data.begin(); iter != _data.end(); ++iter)
	{
		if ((*iter)->name.compare(target) == 0)
		{
			return (*iter);
		}
	}

	return nullptr;
}

XMLElement* XMLElement::findElement(std::string target)
{
	for (auto iter = _childs.begin(); iter != _childs.end(); ++iter)
	{
		if ((*iter)->name.compare(target) == 0)
		{
			return (*iter);
		}
	}

	return nullptr;
}

std::string XMLElement::getProperty(std::string target)
{
	auto item = _property.find(target);
	if (item == _property.end())
		return "";
	return item->second;
}

void XMLElement::destroyChildren()
{
	destroyStructure(this);
}

void XMLElement::destroyStructure(XMLElement* root)
{
	for (auto iter = root->_childs.begin(); iter != root->_childs.end(); ++iter)
	{
		destroyStructure(*iter);
		delete (*iter);
	}

	root->_childs.clear();

	for (auto iter = root->_data.begin(); iter != root->_data.end(); ++iter)
	{
		delete (*iter);
	}
	root->_data.clear();

}
