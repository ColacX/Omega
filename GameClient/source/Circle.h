#pragma once
#include "HeaderTypes.h"
#include "Arrow.h"

struct Circle
{
	static GLuint shader;
	static void Construct();
	static void BatchRender(std::vector<Circle*> items, glm::mat4 pvmMatrix);

	glm::vec3 position, normal;
	float radius;
	float FactorFrom(Arrow arrow);
};
