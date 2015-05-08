#pragma once

#include <string>
#include <unordered_map>

class Resource
{
	private:
	static std::unordered_map<std::string, void*> resources;

	public:
	static void* Get(std::string name);
	static void Set(std::string name, void* resource);
	static void Construct();
	static void Destruct();
};
