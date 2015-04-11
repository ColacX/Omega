#include <string>
#include <iostream>
#include <chrono>
#include <vector>

struct A
{
	std::vector<std::string> v;

	A() { printf("default constructor\n"); }
	A(A& o) { printf("copy constructor\n"); }
	A(A&& o) { printf("move constructor\n"); }
	A operator=(A& o) { printf("assignment operator\n"); return *this; }
};

int main()
{
	A s;

	for (int ia = 0; ia < 10000; ia++)
	{
		s.v.push_back("item");
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
		A a;
		a = std::move(s);
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



	getchar();
}