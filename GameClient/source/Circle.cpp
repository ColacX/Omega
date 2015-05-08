#include "GraphicLibrary.h"
#include "Resource.h"

#include "Circle.h"

GLuint Circle::shader;

void Circle::Construct()
{
	shader = (GLuint)Resource::Get("script/circle.glsl");
}

void Circle::BatchRender(std::vector<Circle*> items, glm::mat4 pvmMatrix)
{
	glUseProgram(shader);
	for (int ia = 0; ia < items.size(); ia++)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader, "pvmMatrix"), 1, false, glm::value_ptr(pvmMatrix));
		glBegin(GL_TRIANGLES);
		glVertexAttrib2f(1, -1, -1);  glVertexAttrib3f(0, -1, -1, 0);
		glVertexAttrib2f(1, +1, -1);  glVertexAttrib3f(0, +1, +1, 0);
		glVertexAttrib2f(1, -1, +1);  glVertexAttrib3f(0, -1, +1, 0);
		
		glVertexAttrib2f(1, +1, +1);  glVertexAttrib3f(0, +1, +1, 0);
		glVertexAttrib2f(1, +1, -1);  glVertexAttrib3f(0, +1, +1, 0);
		glVertexAttrib2f(1, -1, +1);  glVertexAttrib3f(0, -1, +1, 0);
		glEnd();
	}
}

float Circle::FactorFrom(Arrow arrow)
{
	float denominator = glm::dot(arrow.direction, normal);

	if (denominator >= 0)
	{
		//missed the plane
		return std::numeric_limits<float>::min();
	}

	float numerator = glm::dot((position - arrow.origin), normal);
	float factor = numerator / denominator;
	glm::vec3 intersectionPoint = arrow.origin + arrow.direction * factor;
	float magnitude = glm::length(intersectionPoint - position);

	if (magnitude > radius)
	{
		//outside of circle
		return std::numeric_limits<float>::min();
	}

	return factor;
}
