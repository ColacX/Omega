#pragma once

struct UnitTestBase
{
	bool catchMode = false;
	int totalTests = 0;
	int passedTests = 0;
	int failedTests = 0;

	template <typename T>
	void Test(void (T::*methodPointer)())
	{
		totalTests++;
		auto c = dynamic_cast<T*>(this);

		if (catchMode)
		{
			try
			{
				(c->*methodPointer)();
			}
			catch (...)
			{
				failedTests++;
				return;
			}
		}
		else
		{
			(c->*methodPointer)();
		}

		passedTests++;
	}

	void Assert(bool passed, const char* message)
	{
		if (!passed)
			throw message;
	}

	virtual void TestAll() = 0;
};
