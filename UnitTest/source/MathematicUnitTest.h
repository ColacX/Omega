#pragma once

#include "UnitTestBase.h"
#include <Mathematic.h>

struct MathematicUnitTest : UnitTestBase
{
public:
	void TestAll()
	{
		typedef MathematicUnitTest T;
		Test(&T::MultiplyQuaternion0);
		//Test(&T::NoRotation);
		//Test(&T::RotateZ90);
	}

	void MultiplyQuaternion0()
	{
		Quaternion4 a(3, 1, 6, 3);
		Quaternion4 b(6, 7, 2, 1);
		auto r = Multiply(a, b);
		Assert(r.x == 27, "Expected 27");
		Assert(r.y == 62, "Expected 62");
		Assert(r.z == -19, "Expected -19");
		Assert(r.w == -4, "Expected -4");
	}

	void MultiplyQuaternion1()
	{
		Quaternion4 a(7.2, 1.2, 6.5, 3.6);
		Quaternion4 b(31.3, 3.1, 2.2, 7.1);
		auto r = Multiply(a, b);
		Assert(r.x == 27, "Expected 27");
		Assert(r.y == 62, "Expected 62");
		Assert(r.z == -19, "Expected -19");
		Assert(r.w == -4, "Expected -4");
	}

	void NoRotation()
	{
		Vector3 v(1,0,0);
		Quaternion4 q(0,0,0,1);
		auto r = Rotate(v, q);
		Assert(r.x == 1, "Expected 1");
		Assert(r.y == 0, "Expected 0");
		Assert(r.z == 0, "Expected 0");
	}

	void RotateZ90()
	{
		Vector3 v(1, 0, 0);
		Quaternion4 q(0, 0, 0, 1);
		auto r = Rotate(v, q);
		Assert(r.x == 0, "Expected 0");
		Assert(r.y == 1, "Expected 1");
		Assert(r.z == 0, "Expected 0");
	}
};
