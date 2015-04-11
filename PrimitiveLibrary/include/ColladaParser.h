struct ColladaGeometrySource
{
	std::string id;
	std::vector<float> floats;
};

struct ColladaGeometry
{
	std::string id, name, triangleSourceId;
	std::unordered_map<std::string, ColladaGeometrySource> sources;
	std::vector<int> triangles;
};

class ColladaParser
{
private:
	int ReadInt(const char* s)
	{
		int i;
		int scanCount;
		sscanf(s, "%d%n", &i, &scanCount);

		if (!scanCount)
			throw "unable to read value";

		return i;
	}

	std::vector<int> ReadInts(const char* s, int n)
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

	std::vector<float> ReadFloats(const char* s, int n)
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

	std::string ReadId(const std::string& hashId)
	{
		return std::string(hashId.c_str() + 1);
	}

	void ReadGeometry(XmlElement* geometryElement, ColladaGeometry& g)
	{
		g.id = geometryElement->Attribute("id");
		g.name = geometryElement->Attribute("name");
		auto mesh = geometryElement->Child("mesh");
		auto sources = mesh->Childs("source");

		if (sources.size() != 3)
			throw "unexpected number of sources";

		for (int ib = 0; ib < sources.size(); ib++)
		{
			auto& source = sources[ib];
			ColladaGeometrySource s;
			s.id = source->Attribute("id");
			auto floats = source->Childs("float_array")[0];
			s.floats = ReadFloats(floats->content.c_str(), ReadInt(floats->Attribute("count").c_str()));
			g.sources.emplace(s.id, s);
		}

		auto triangle = mesh->Child("triangles");

		auto p = triangle->Child("p");
		int triangleCount = ReadInt(triangle->Attribute("count").c_str());
		g.triangles = ReadInts(p->content.c_str(), triangleCount * 3);

		auto input = triangle->Child("input");
		std::string verticesId = ReadId(input->Attribute("source"));
		auto vertices = mesh->Child("vertices", "id", verticesId);
		auto verticesInput = vertices->Child("input", "semantic", "POSITION");
		g.triangleSourceId = ReadId(verticesInput->Attribute("source"));
	}

public:
	std::unordered_map<std::string, ColladaGeometry> geometries;
	//std::vector<ColladaMaterials> materials;
	//std::vector<ColladaImages> images;
	//std::vector<ColladaAnimations> animations;

	void Parse(const char* colladaFileData)
	{
		XmlParser p;
		p.Parse(colladaFileData);

		if (p.root.tagName != "COLLADA")
			throw "expected COLLADA as root element";

		if (!p.root.HasAttribute("xmlns", "http://www.collada.org/2005/11/COLLADASchema") || !p.root.HasAttribute("version", "1.4.1"))
			throw "expected COLLADA as root element";

		auto libraryGeometries = p.root.Childs("library_geometries");

		for (int ia = 0; ia < libraryGeometries.size(); ia++)
		{
			auto geometryElements = libraryGeometries[ia]->Childs("geometry");

			for (int ib = 0; ib < geometryElements.size(); ib++)
			{
				ColladaGeometry g;
				ReadGeometry(geometryElements[ib], g);
				geometries.emplace(g.id, g);
			}
		}
	}
};