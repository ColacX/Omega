#pragma once

#include "HeaderTypes.h"
#include "ColladaParser.h"
#include "Camera.h"

struct CircleConnector
{
	static ColladaParser p2;
	static GLuint texture2, shader;

	static void Load();

	static void BatchRender(const std::vector<CircleConnector*> items, Camera* camera);

	glm::vec3 position, normal;
	glm::quat rotation;

	CircleConnector();
	CircleConnector(glm::vec3 position, glm::vec3 normal, glm::quat rotation);

	bool RayHit(glm::vec3 rayOrigin, glm::vec3 rayDirection, float& originDistance);
};
