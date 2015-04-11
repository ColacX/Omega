#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <crtdbg.h>

#include <XmlParser.h>
#include <SafeFile.h>
#pragma comment(lib, "PrimitiveLibrary.lib")

#include "XmlParserUnitTest.h"
#include "SafeFileUnitTest.h"
#include "MathematicUnitTest.h"

void main()
{
	printf("UnitTest: start\n");
	//_CrtSetBreakAlloc(166);

	{
		std::vector<UnitTestBase*> testClasses;
		testClasses.push_back(new XmlParserUnitTest());
		testClasses.push_back(new SafeFileUnitTest());
		testClasses.push_back(new MathematicUnitTest());

		for (int ia = 0; ia < testClasses.size(); ia++)
		{
			UnitTestBase& t = *testClasses[ia];
			t.TestAll();

			if (t.passedTests != t.totalTests)
			{
				printf("failed: %d passed: %d total: %d\n", t.failedTests, t.passedTests, t.totalTests);
				getchar();
			}
		}

		for (int ia = 0; ia < testClasses.size(); ia++)
		{
			delete testClasses[ia];
		}
		testClasses.clear();
	}

	//remember that destructors are called only when they go out of scope
	if (_CrtDumpMemoryLeaks())
	{
		throw "memory leaks detected";
	}

	printf("UnitTest: end\n");
}
