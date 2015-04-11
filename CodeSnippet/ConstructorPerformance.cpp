#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_map>

struct A
{
	std::vector<std::string> v;
	A() { printf("default constructor\n"); }
	A(A& o) { printf("copy constructor\n"); }
	//A(A&& o) : v(std::move(o.v)) {}
	A(A&& o) { printf("move constructor\n"); }
	A operator=(A& o) { printf("assignment operator\n"); return *this; }
};

A fun0()
{
	A a;
	return a;
}

A fun1()
{
	A a;
	return std::move(a);
}

void fun2(A& a)
{
}

int main()
{
	A s;

	for (int ia = 0; ia < 10000; ia++)
	{
		s.v.push_back("item");
	}

	printf("tests starts now\n\n");

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		A a = fun0();
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		A& a = fun0();
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();

		std::unordered_map<std::string, A> m;
		m.emplace("a", fun0());

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();

		std::unordered_map<std::string, A> m;
		m.emplace("a", fun1());

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();

		A a;
		fun2(a);
		std::unordered_map<std::string, A> m;
		m.emplace("a", std::move(a));

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();

		A a;
		fun2(a);
		std::unordered_map<std::string, A> m;
		m.emplace("a", a);

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();

		std::unordered_map<std::string, A> m;
		m.emplace("s", s);

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		
		std::unordered_map<std::string, A> m;
		m.emplace("s", std::move(s));

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();

		std::unordered_map<std::string, A> m;
		m.emplace("s", s);

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();

		std::unordered_map<std::string, A> m;
		m["s"] = s;

		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		A d;
		d = s;
		
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		A a = s;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		A b(s);
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		A c(std::move(s));
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	{
		auto startTime = std::chrono::high_resolution_clock::now();
		A& z = s;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		printf("et(ns): %f\n", elapsedTime);
	}

	//conclusion you must explicity create a move constructor and apporiopriate std::move for each member variable
	//passing refrence to a function is always more efficient
	getchar();
}