#define _CRT_SECURE_NO_WARNINGS

#include "../include/XmlParser.h"

int XmlParser::Peek(const char* format)
{
	scanCount = 0;
	sscanf(xml + peekOffset, format, buffer, &scanCount);
	return scanCount;
}

void XmlParser::XMLHeaderInfo()
{
	Peek("%[^\n]\n%n");

	if (scanCount <= 0)
		throw "unexpected xml header info";

	//fprintf(log, "%s\n", buffer);
	peekOffset += scanCount;
}

void XmlParser::SkipWhitespace()
{
	//if(Peek("%[ \t\r\n]%n"))
	//	//fprintf(log, "%s\n", buffer);
	Peek("%[ \t\r\n]%n");
	peekOffset += scanCount;
}

bool XmlParser::ReadElement(XmlElement& e)
{
	SkipWhitespace();

	//read tag start
	if (!Peek("<%[a-zA-Z0-9-_#]%n"))
		return false;

	//fprintf(log, "<%s\n", buffer);
	e.tagName = buffer;
	peekOffset += scanCount;

	//read tag start attributes
	while (true)
	{
		SkipWhitespace();

		if (!Peek("%[a-zA-Z0-9-_#]%n"))
			break;
		//fprintf(log, "%s\n", buffer);
		std::string attributeName = buffer;
		peekOffset += scanCount;
		if (e.attributes.find(attributeName) != e.attributes.end())
			throw "attribute redefined";

		SkipWhitespace();

		if (!Peek("%[=]%n"))
			throw "expected attribute equal symbol =";
		peekOffset += scanCount;

		SkipWhitespace();

		if (!Peek("'%[^\']'%n") && !Peek("\"%[^\"]\"%n"))
			throw "expected attribute value";

		//fprintf(log, "%s\n", buffer);
		e.attributes[attributeName] = buffer;
		peekOffset += scanCount;

		//fprintf(log, "%s=%s\n", attributeName.c_str(), e.attributes[attributeName].c_str());
	}

	SkipWhitespace();

	//read tag start quick close
	if (Peek("%*[/]%[>]%n"))
	{
		peekOffset += 2;
		return true;
	}

	//read tag start close
	if (!Peek("%[>]%n"))
		throw "expected tag start close";
	peekOffset += scanCount;

	//read content
	if (Peek("%[^<]%n"))
	{
		//fprintf(log, "%s\n", buffer);
		e.content = buffer;
		peekOffset += scanCount;
	}

	//read children
	while (true)
	{
		XmlElement c;

		if (!ReadElement(c))
			break;

		e.childs.push_back(c);
	}

	//read tag end
	if (!Peek("</%[a-zA-Z0-9-_#]%n"))
		throw "expected tag end";

	if (e.tagName != buffer)
		throw "tagName open and close mismatch";

	//fprintf(log, "</%s\n", buffer);
	peekOffset += scanCount;

	SkipWhitespace();

	if (!Peek("%[>]%n"))
		throw "expected tag end close";
	peekOffset += scanCount;
	return true;
}

void XmlParser::Parse(const char* xmlData)
{
	xml = xmlData;

	//fprintf(log, "XmlParser: start\n");

	XMLHeaderInfo();

	if (!ReadElement(root))
		throw "missing root element";

	SkipWhitespace();

	if (peekOffset != strlen(xmlData))
		throw "xml may only have one root element. extra characters detected.";

	//fprintf(log, "XmlParser: end\n");
}

std::vector<XmlElement*> XmlElement::Childs(std::string tagName)
{
	std::vector<XmlElement*> v;
	for (int ia = 0; ia < childs.size(); ia++)
	{
		if (childs[ia].tagName == tagName)
			v.push_back(&childs[ia]);
	}
	return v;
}

std::vector<XmlElement*> XmlElement::Childs(std::string tagName, std::string attributeName, std::string attributeValue)
{
	std::vector<XmlElement*> v;
	for (int ia = 0; ia < childs.size(); ia++)
	{
		auto& c = childs[ia];
		
		if (c.tagName == tagName && c.HasAttribute(attributeName, attributeValue))
			v.push_back(&c);
	}
	return v;
}

XmlElement* XmlElement::Child(std::string tagName)
{
	for (int ia = 0; ia < childs.size(); ia++)
	{
		if (childs[ia].tagName == tagName)
			return &childs[ia];
	}

	throw "no element with specified tagName found";
}

XmlElement* XmlElement::Child(std::string tagName, std::string attributeName, std::string attributeValue)
{
	for (int ia = 0; ia < childs.size(); ia++)
	{
		auto& c = childs[ia];
		if (c.tagName == tagName && c.HasAttribute(attributeName, attributeValue))
			return &c;
	}

	throw "no element with specified tagName found";
}

bool XmlElement::HasAttribute(std::string attributeName)
{
	return this->attributes.find(attributeName) != this->attributes.end();
}

bool XmlElement::HasAttribute(std::string attributeName, std::string attributeValue)
{
	auto i = this->attributes.find(attributeName);
	return i != this->attributes.end() && i->second == attributeValue;
}

std::string XmlElement::Attribute(std::string attributeName)
{
	auto i = this->attributes.find(attributeName);

	if (i == this->attributes.end())
		throw "attributeName not found";

	return i->second;
}

bool XmlElement::HasChild(std::string tagName)
{
	for (int ia = 0; ia < childs.size(); ia++)
	{
		if (childs[ia].tagName == tagName)
			return true;
	}

	return false;
}