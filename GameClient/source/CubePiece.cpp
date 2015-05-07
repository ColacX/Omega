#define _CRT_SECURE_NO_WARNINGS

#include "GraphicLibrary.h"

#include "CubePiece.h"
#include "SafeFile.h"

void CubePiece::Load()
{
	//todo add dependency inject
	auto s = "script/textured.glsl";
	shader = CreateShaderProgram(s, s, 0);
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "sampler0"), 0);
	glUseProgram(0);

	texture = LoadImageToTexture("binary/Colorful-Sky-Desktop-Background.jpg");

	SafeFile f;
	std::string fileData;
	f.OpenReadAll("script/Cube0.DAE", fileData);
	p.Parse(fileData.c_str());
}

void CubePiece::BatchRender(const std::vector<CubePiece*> pieces, Camera* camera)
{
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	for (int ia = 0; ia < pieces.size(); ia++)
	for (auto node = p.currentScene->nodes.begin(); node != p.currentScene->nodes.end(); node++)
	{
		auto& n = node->second;

		glm::mat4 model = glm::translate(pieces[ia]->position) * glm::toMat4(pieces[ia]->rotation);
		glm::mat4 pvmMatrix = camera->ProjectionViewMatrix() * model;
		glUniformMatrix4fv(glGetUniformLocation(shader, "pvmMatrix"), 1, false, glm::value_ptr(pvmMatrix));

		auto& g = p.geometries[n.geometryId];
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
			//printf("%f %f %f\n", x, y, z);
		}

		glEnd();
	}
}

void CubePiece::AddConnectors(std::vector<CircleConnector*>& sceneConnectors)
{
	for (int ia = 0; ia < circleConnectors.size(); ia++)
	{
		CircleConnector* item = new CircleConnector();
		auto& c = circleConnectors[ia];
		item->position = this->rotation * c.position + this->position;
		item->normal = this->rotation * c.normal;
		item->rotation = this->rotation * c.rotation;
		sceneConnectors.push_back(item);
	}
}

GLuint CubePiece::texture;
GLuint CubePiece::shader;
ColladaParser CubePiece::p;
std::vector<CircleConnector> CubePiece::circleConnectors = {
	CircleConnector(glm::vec3(0, 0, +1), glm::vec3(0, 0, 1), glm::toQuat(glm::lookAt(glm::vec3(), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0))))
	, CircleConnector(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::toQuat(glm::lookAt(glm::vec3(), glm::vec3(0, 0, +1), glm::vec3(0, 1, 0))))
	, CircleConnector(glm::vec3(0.5f, 0, +0.5f), glm::vec3(+1, 0, 0), glm::toQuat(glm::lookAt(glm::vec3(), glm::vec3(+1, 0, 0), glm::vec3(0, 1, 0))))
	, CircleConnector(glm::vec3(-0.5f, 0, +0.5f), glm::vec3(-1, 0, 0), glm::toQuat(glm::lookAt(glm::vec3(), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0))))
	, CircleConnector(glm::vec3(0, +0.5f, +0.5f), glm::vec3(0, +1, 0), glm::toQuat(glm::lookAt(glm::vec3(), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1))))
	, CircleConnector(glm::vec3(0, -0.5f, +0.5f), glm::vec3(0, -1, 0), glm::toQuat(glm::lookAt(glm::vec3(), glm::vec3(0, +1, 0), glm::vec3(0, 0, -1))))
};