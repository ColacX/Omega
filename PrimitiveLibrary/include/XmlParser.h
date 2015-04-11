#pragma once

#include <vector>
#include <unordered_map>

struct XmlElement
{
	std::string tagName;
	std::unordered_map<std::string, std::string> attributes;
	std::string content;
	std::vector<XmlElement> childs;

	std::vector<XmlElement*> Childs(std::string tagName);
	std::vector<XmlElement*> Childs(std::string tagName, std::string attributeName, std::string attributeValue);
	XmlElement* Child(std::string tagName);
	XmlElement* Child(std::string tagName, std::string attributeName, std::string attributeValue);
	bool HasChild(std::string tagName);
	bool HasAttribute(std::string attributeName);
	bool HasAttribute(std::string attributeName, std::string attributeValue);
	std::string Attribute(std::string attributeName);
};

class XmlParser
{
private:
	const char* xml;
	int scanCount = 0;
	int peekOffset = 0;
	char buffer[4096];

	int Peek(const char* format);
	void XMLHeaderInfo();
	void SkipWhitespace();
	bool ReadElement(XmlElement& e);

public:
	XmlElement root;
	void Parse(const char* xmlData);
};
