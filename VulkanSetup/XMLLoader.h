#pragma once
#include <string>
class XMLElement;
class XMLData;
namespace rapidxml {
	template<typename t>
	class xml_document;
}

class XMLLoader
{
public:
	rapidxml::xml_document<char>* loadXML_rapid(std::string path);

	XMLElement*			startElement_c(char* data);
	XMLData*			getData_c(char* data);
	XMLElement*			loadXML_c(std::string path);

	XMLElement*			startElement(std::string data);
	XMLData*			getData(std::string data);
	XMLElement*			loadXML(std::string path);
};
