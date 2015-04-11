#pragma once

struct CircleConnector
{
	static ColladaParser p2;
	static GLuint texture2, shader;

	static void Load()
	{
		//todo add dependency inject
		auto s = "script/textured.glsl";
		shader = CreateShaderProgram(s, s, 0);
		glUseProgram(shader);
		glUniform1i(glGetUniformLocation(shader, "sampler0"), 0);
		glUseProgram(0);

		SafeFile f;
		std::string fileData;
		f.OpenReadAll("script/CrossAim0.DAE", fileData);
		p2.Parse(fileData.c_str());

		texture2 = LoadImageToTexture("binary/Connector.tga");
	}

	static void BatchRender(const std::vector<CircleConnector*> items, Camera* camera)
	{
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glUseProgram(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);

		for (int ia = 0; ia < items.size(); ia++)
		for (auto node = p2.currentScene->nodes.begin(); node != p2.currentScene->nodes.end(); node++)
		{
			auto& n = node->second;
			auto model = glm::toMat4(items[ia]->rotation);
			model = glm::translate(items[ia]->position) * model;
			auto pvmMatrix = camera->ProjectionViewMatrix() * model;
			glUniformMatrix4fv(glGetUniformLocation(shader, "pvmMatrix"), 1, false, glm::value_ptr(pvmMatrix));

			auto& g = p2.geometries[n.geometryId];
			auto& vertices = g.vertices[g.triangleVerticesId];
			auto& pos = g.sources[vertices.positionSourceId].floats;
			auto& uvs = g.sources[vertices.texcoordSourceId].floats;

			glBegin(GL_TRIANGLES);

			for (int ia = 0; ia < g.triangleIndices.size(); ia++)
			{
				auto vertexId = g.triangleIndices[ia];
				float x = pos[vertexId * 3 + 0];
				float y = pos[vertexId * 3 + 1];
				float z = pos[vertexId * 3 + 2];
				float u = uvs[vertexId * 2 + 0];
				float v = uvs[vertexId * 2 + 1];

				glVertexAttrib2f(1, u, -v);  glVertexAttrib3f(0, x, y, z);
			}

			glEnd();
		}
	}

	glm::vec3 position, normal;
	glm::quat rotation;

	CircleConnector() {}
	CircleConnector(glm::vec3 position, glm::vec3 normal, glm::quat rotation) : position(position), normal(normal), rotation(rotation)
	{
		//todo calculate rotation based on normal
	}

	bool RayHit(glm::vec3 rayOrigin, glm::vec3 rayDirection, float& originDistance)
	{
		//auto normal = glm::rotate(circleConnectors[ia].rotation, glm::vec3(0,0,1));
		auto denominator = glm::dot(rayDirection, normal);

		if (denominator > -1e-6)
		{
			//hmm the normal is in weird direction...

			//false
			return false;
		}

		//true
		auto numerator = glm::dot((position - rayOrigin), normal);
		float rayT = numerator / denominator;

		if (rayT < 0)
		{
			//behind the camera
			return false;
		}

		auto planeIntersectionPoint = rayOrigin + rayDirection * rayT;
		auto vector = (planeIntersectionPoint - position);
		auto magnitude = glm::length(vector);

		if (magnitude > 0.5f)
		{
			//outside of circle
			return false;
		}

		originDistance = rayT;
		return true;
	}
};

ColladaParser CircleConnector::p2;
GLuint CircleConnector::texture2;
GLuint CircleConnector::shader;
