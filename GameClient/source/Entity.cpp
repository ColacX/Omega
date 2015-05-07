#include "Entity.h"

#include <vector>

void Entity::MoveGlobal(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Entity::MoveLocal(float x, float y, float z)
{
	glm::vec4 global_translation = glm::toMat4(rotation) * glm::vec4(x, y, z, 1.0f);
	position += glm::vec3(global_translation);
}

void Entity::RotateGlobalX(float degrees)
{
	rotation = glm::angleAxis(degrees, glm::vec3{ 1, 0, 0 } *glm::inverse(rotation)) * rotation;
}

void Entity::RotateGlobalY(float degrees)
{
	rotation = glm::angleAxis(degrees, glm::vec3{ 0, 1, 0 } *glm::inverse(rotation)) * rotation;
}

void Entity::RotateGlobalZ(float degrees)
{
	rotation = glm::angleAxis(degrees, glm::vec3{ 0, 0, 1 } *glm::inverse(rotation)) * rotation;
}

void Entity::RotateLocalX(float degrees)
{
	rotation = glm::angleAxis(degrees, glm::vec3{ 1, 0, 0 }) * rotation;
}

void Entity::RotateLocalY(float degrees)
{
	rotation = glm::angleAxis(degrees, glm::vec3{ 0, 1, 0 }) * rotation;
}

void Entity::RotateLocalZ(float degrees)
{
	rotation = glm::angleAxis(degrees, glm::vec3{ 0, 0, 1 }) * rotation;
}

glm::quat Entity::LookAt(float x, float y, float z)
{
	glm::vec3 point(x, y, z);
	return glm::toQuat(glm::lookAt(position, point, glm::vec3{ 0.0f, 1.0f, 0.0f }));;
}

void Entity::MoveTo(float x, float y, float z, float delta_time)
{
	float t = delta_time * 1e-9;
	position += (glm::vec3(x, y, z) - position) * t;
}

void Entity::RotateTo(glm::quat rotate_to, float delta_time)
{
	float r = delta_time * 1e-9;
	rotation = glm::slerp(rotation, rotate_to, r);
}

glm::mat4 Entity::ModelMatrix()
{
	return glm::translate(position) * glm::toMat4(rotation);
}
