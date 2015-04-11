#pragma once

#include "UnitTestBase.h"
#include <SafeFile.h>

struct SafeFileUnitTest : UnitTestBase
{
public:
	void TestAll()
	{
		typedef SafeFileUnitTest T;
		Test(&T::Simple);
		Test(&T::Plane0);
	}

	void Simple()
	{
		SafeFile f;
		std::string actual;
		f.OpenReadAll("script/SafeFile.UnitTest/simple.txt", actual);
		std::string expected(R"(adsklfjsdklfjkljl
sdfjlk
sdfsdf
sjjd)");

		Assert(actual.size() == 36, "Expected fileSize to be 36 bytes");
		Assert(actual == expected, "Expected file data to match");
	}

	void Plane0()
	{
		SafeFile f;
		std::string actual;
		f.OpenReadAll("script/SafeFile.UnitTest/plane0.DAE", actual);
		Assert(actual.size() == 3471, "Expected fileSize to be 3471 bytes");
	}
};
