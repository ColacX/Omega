#pragma once

#include "UnitTestBase.h"

struct XmlParserUnitTest : UnitTestBase
{
	void TestAll()
	{
		typedef XmlParserUnitTest T;
		Test(&T::Simple);
		Test(&T::Plane0);
		Test(&T::Minimal0);
	}

	void Simple()
	{
		XmlParser p;
		p.Parse(R"(<?xml version="1.0" encoding="utf-8"?>
<root>
	<tag1></tag1>
	<tag2/>
	<tag3 tag3a0="v0" tag3a1 = "v1"></tag3>
	<tag4 tag4a0="v2"/>
	<tag5 tag5a0="v3">
		<tag6>
			<tag7 tag7a0  =  'v4'>
			</tag7>
		</tag6>
	</tag5>
</root>
)");

		Assert(p.root.childs[4].childs[0].childs[0].Attribute("tag7a0") == "v4", "expected tag7 to have an attribute tag7a0 with value v4");
	}

	void Plane0()
	{
		XmlParser p;
		SafeFile f;
		std::string fileData;
		f.OpenReadAll("script/XmlParser.UnitTest/plane0.DAE", fileData);
		p.Parse(fileData.c_str());
		auto e = p.root.childs[1].childs[0].childs[0].childs[0].childs[0];

		Assert(p.root.tagName == "COLLADA", "Expected COLLADA element");
		Assert(e.tagName == "float_array", "Expected float_array element");
		Assert(e.Attribute("id") == "Plane001-POSITION-array", "Expected id attribute value to be Plane001-POSITION-array");
		Assert(e.Attribute("count") == "18", "Expected id attribute count to be 18");
	}

	void Minimal0()
	{
		XmlParser p;
		SafeFile f;
		std::string fileData;
		f.OpenReadAll("script/XmlParser.UnitTest/minimal0.DAE", fileData);
		p.Parse(fileData.c_str());
		
		Assert(p.root.tagName == "COLLADA", "Expected COLLADA element");
		Assert(p.root.childs[4].tagName == "library_geometries", "Expected library_geometries element");
		auto fa = p.root.childs[4].childs[0].childs[0].childs[0].childs[0];
		Assert(fa.tagName == "float_array", "Expected float_array element");
		Assert(fa.Attribute("count") == "108", "Expected count element");
	}
};
