#include "Resource.h"
#include "GameLibrary.h"

std::unordered_map<std::string, void*> Resource::resources;

void* Resource::Get(std::string name)
{
	auto it = resources.find(name);
	if (it == resources.end())
		throw "resource not found";
	return it->second;
}

void Resource::Set(std::string name, void* resource)
{
	resources[name] = resource;
}

void Resource::Construct()
{
	{
		auto s = "script/circle.glsl";
		auto shader = CreateShaderProgram(s, s, 0);
		Resource::Set(s, (void*)shader);
	}
	
	/*{
	auto s = "script/textured.glsl";
	auto shader = CreateShaderProgram(s, s, 0);
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "sampler0"), 0);
	glUseProgram(0);
	Resource::Set("textured.glsl", shader);
	}*/
}

void Resource::Destruct()
{

}
