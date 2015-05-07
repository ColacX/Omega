#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "XmlParser.h"

class ColladaParser
{
public:
#pragma region Region_1
	struct Image
	{
		std::string id, name;
	};

	struct Material
	{
		std::string id, name;
	};

	struct Effect
	{
		std::string id, name;
	};

	struct Geometry
	{
		struct Source
		{
			std::string id;
			std::vector<float> floats;
		};

		struct Vertices
		{
			std::string id;
			std::string positionSourceId;
			std::string normalSourceId;
			std::string texcoordSourceId;
		};

		std::string id, name, triangleVerticesId;
		std::unordered_map<std::string, Source> sources;
		std::vector<int> triangleIndices;
		std::unordered_map<std::string, Vertices> vertices;

		void ReadSource(XmlElement* e);
	};

	struct Animation
	{
		struct Source
		{
			std::string id;
			std::vector<float> floats;
		};

		std::string id, name, targetNodeId;
		std::unordered_map<std::string, Source> sources;

		void ReadSource(XmlElement* e);
	};

	struct Scene
	{
		struct Node
		{
			std::string id, name, geometryId;
			std::vector<float> matrix;
		};

		std::string id, name;
		std::unordered_map<std::string, Node> nodes;

		void ReadNode(XmlElement* e);
	};
#pragma endregion

	std::unordered_map<std::string, Animation> animations;
	std::unordered_map<std::string, Geometry> geometries;
	std::unordered_map<std::string, Scene> scenes;
	Scene* currentScene = 0;

	void Parse(const char* colladaFileData);

private:
#pragma region StaticMembers

	static int ReadInt(const char* s);

	static int ReadInt(const std::string&s)
	{
		return ReadInt(s.c_str());
	}

	static std::vector<int> ReadInts(const char* s, int n);

	static std::vector<int> ReadInts(const std::string& s, int n);

	static std::vector<float> ReadFloats(const char* s, int n);

	static std::vector<float> ReadFloats(const std::string& s, int n);

	static std::string ReadId(const std::string& hashId);

	static std::string ReadTargetNode(const std::string& target);

#pragma endregion

	void ReadScene(XmlElement* e);

	void ReadGeometry(XmlElement* e);

	void ReadAnimations(XmlElement* e);
};
