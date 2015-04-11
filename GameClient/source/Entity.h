#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\vector_angle.hpp>

struct Entity
{
	//all computation are in world space
	glm::vec3 position;
	glm::quat rotation;

	void MoveGlobal(float x, float y, float z)
	{
		position.x += x;
		position.y += y;
		position.z += z;
	}

	void MoveLocal(float x, float y, float z)
	{
		glm::vec4 global_translation = glm::toMat4(rotation) * glm::vec4(x, y, z, 1.0f);
		position += glm::vec3(global_translation);
	}

	void RotateGlobalX(float degrees)
	{
		rotation = glm::angleAxis(degrees, glm::vec3{ 1, 0, 0 } *glm::inverse(rotation)) * rotation;
	}

	void RotateGlobalY(float degrees)
	{
		rotation = glm::angleAxis(degrees, glm::vec3{ 0, 1, 0 } *glm::inverse(rotation)) * rotation;
	}

	void RotateGlobalZ(float degrees)
	{
		rotation = glm::angleAxis(degrees, glm::vec3{ 0, 0, 1 } *glm::inverse(rotation)) * rotation;
	}

	void RotateLocalX(float degrees)
	{
		rotation = glm::angleAxis(degrees, glm::vec3{ 1, 0, 0 }) * rotation;
	}

	void RotateLocalY(float degrees)
	{
		rotation = glm::angleAxis(degrees, glm::vec3{ 0, 1, 0 }) * rotation;
	}

	void RotateLocalZ(float degrees)
	{
		rotation = glm::angleAxis(degrees, glm::vec3{ 0, 0, 1 }) * rotation;
	}

	glm::quat LookAt(float x, float y, float z)
	{
		glm::vec3 point(x, y, z);
		return glm::toQuat(glm::lookAt(position, point, glm::vec3{ 0.0f, 1.0f, 0.0f }));;
	}

	void MoveTo(float x, float y, float z, float delta_time)
	{
		float t = delta_time * 1e-9;
		position += (glm::vec3(x, y, z) - position) * t;
	}

	void RotateTo(glm::quat rotate_to, float delta_time)
	{
		float r = delta_time * 1e-9;
		rotation = glm::slerp(rotation, rotate_to, r);
	}

	glm::mat4 ModelMatrix()
	{
		return glm::translate(position) * glm::toMat4(rotation);
	}
};
