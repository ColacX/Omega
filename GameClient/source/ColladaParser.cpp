#define _CRT_SECURE_NO_WARNINGS

#include "ColladaParser.h"

void ColladaParser::Geometry::ReadSource(XmlElement* e)
{
	Geometry::Source s;
	s.id = e->Attribute("id");
	auto v = e->Childs("float_array");

	for (int ia = 0; ia < v.size(); ia++)
	{
		auto& f = v[ia];
		auto count = ReadInt(f->Attribute("count"));
		s.floats = ReadFloats(f->content, count);
	}

	this->sources.emplace(s.id, s);
}

void ColladaParser::Animation::ReadSource(XmlElement* e)
{
	Source s;
	s.id = e->Attribute("id");

	if (e->HasChild("float_array"))
	{
		auto fa = e->Child("float_array");
		auto count = ReadInt(fa->Attribute("count"));
		s.floats = ReadFloats(fa->content, count);
	}

	this->sources.emplace(s.id, s);
}

void ColladaParser::Scene::ReadNode(XmlElement* e)
{
	if (e->HasAttribute("type", "JOINT"))
		return;

	Scene::Node n;
	n.id = e->Attribute("id");
	n.name = e->Attribute("name");
	n.matrix = ReadFloats(e->Child("matrix")->content, 16);
	n.geometryId = ReadId(e->Child("instance_geometry")->Attribute("url"));
	this->nodes.emplace(n.id, n);
}

void ColladaParser::Parse(const char* colladaFileData)
{
	XmlParser p;
	p.Parse(colladaFileData);

	if (p.root.tagName != "COLLADA")
		throw "expected COLLADA as root element";

	if (!p.root.HasAttribute("xmlns", "http://www.collada.org/2005/11/COLLADASchema") || !p.root.HasAttribute("version", "1.4.1"))
		throw "expected COLLADA as root element";

	XmlElement* root = &p.root;

	{
		auto v = root->Child("library_geometries")->Childs("geometry");

		for (int ia = 0; ia < v.size(); ia++)
			ReadGeometry(v[ia]);
	}

	if (root->HasChild("library_animations"))
	{
		auto v = root->Child("library_animations")->Childs("animation");

		for (int ia = 0; ia < v.size(); ia++)
			ReadAnimations(v[ia]);
	}

	{
		auto v = root->Child("library_visual_scenes")->Childs("visual_scene");

		for (int ia = 0; ia < v.size(); ia++)
		{
			ReadScene(v[ia]);
		}
	}

	auto sceneId = ReadId(root->Child("scene")->Child("instance_visual_scene")->Attribute("url"));
	currentScene = &scenes[sceneId];
	int x = 2;
}

int ColladaParser::ReadInt(const char* s)
{
	int i;
	int scanCount;
	sscanf(s, "%d%n", &i, &scanCount);

	if (!scanCount)
		throw "unable to read value";

	return i;
}

std::vector<int> ColladaParser::ReadInts(const std::string& s, int n)
{
	return ReadInts(s.c_str(), n);
}

std::vector<int> ColladaParser::ReadInts(const char* s, int n)
{
	std::vector<int> v;
	int scanOffset = 0;

	for (int ia = 0; ia < n; ia++)
	{
		int scanCount = 0;
		int i;
		sscanf(s + scanOffset, "%d%n", &i, &scanCount);
		scanOffset += scanCount;

		if (scanCount)
			v.push_back(i);
	}

	if (v.size() != n)
		throw "unable to read all";

	return v;
}

std::vector<float> ColladaParser::ReadFloats(const char* s, int n)
{
	std::vector<float> v;
	int scanOffset = 0;

	for (int ia = 0; ia < n; ia++)
	{
		int scanCount = 0;
		float f;
		sscanf(s + scanOffset, "%f%n", &f, &scanCount);
		scanOffset += scanCount;

		if (scanCount)
			v.push_back(f);
	}

	if (v.size() != n)
		throw "unable to read all";

	return v;
}

std::vector<float> ColladaParser::ReadFloats(const std::string& s, int n)
{
	return ReadFloats(s.c_str(), n);
}

std::string ColladaParser::ReadId(const std::string& hashId)
{
	return std::string(hashId.c_str() + 1);
}

std::string ColladaParser::ReadTargetNode(const std::string& target)
{
	char buffer[256];
	int scanCount = 0;
	sscanf(target.c_str(), "%[^/]%n", buffer, &scanCount);

	if (!scanCount)
		throw "expected target node";

	return buffer;
}

void ColladaParser::ReadScene(XmlElement* e)
{
	Scene s;
	s.id = e->Attribute("id");
	s.name = e->Attribute("name");
	auto v = e->Childs("node");

	for (int ia = 0; ia < v.size(); ia++)
		s.ReadNode(v[ia]);

	scenes.emplace(s.id, s);
}

void ColladaParser::ReadGeometry(XmlElement* e)
{
	Geometry g;
	g.id = e->Attribute("id");
	g.name = e->Attribute("name");

	{
		auto v = e->Child("mesh")->Childs("source");
		for (int ia = 0; ia < v.size(); ia++)
			g.ReadSource(v[ia]);
	}

	{
		auto v = e->Child("mesh")->Childs("vertices");
		for (int ia = 0; ia < v.size(); ia++)
		{
			Geometry::Vertices vert;
			vert.id = v[ia]->Attribute("id");
			vert.positionSourceId = ReadId(v[ia]->Child("input", "semantic", "POSITION")->Attribute("source"));
			vert.normalSourceId = ReadId(v[ia]->Child("input", "semantic", "NORMAL")->Attribute("source"));
			vert.texcoordSourceId = ReadId(v[ia]->Child("input", "semantic", "TEXCOORD")->Attribute("source"));
			g.vertices.emplace(vert.id, vert);
		}
	}

	auto t = e->Child("mesh")->Child("triangles");
	auto count = ReadInt(t->Attribute("count"));
	g.triangleVerticesId = ReadId(t->Child("input")->Attribute("source"));
	g.triangleIndices = ReadInts(t->Child("p")->content, count * 3);
	geometries.emplace(g.id, g);
}

void ColladaParser::ReadAnimations(XmlElement* e)
{
	Animation a;
	a.id = e->Attribute("id");
	a.name = e->Attribute("name");

	auto v = e->Child("animation")->Childs("source");
	for (int ia = 0; ia < v.size(); ia++)
		a.ReadSource(v[ia]);

	auto target = e->Child("animation")->Child("channel")->Attribute("target");
	a.targetNodeId = ReadTargetNode(target);
	//todo get sid subId
	animations.emplace(a.id, a);
}
