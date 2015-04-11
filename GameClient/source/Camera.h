#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\vector_angle.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Camera
{
	//position and rotation in camera view space is inverted from world space
	glm::vec3 position;
	glm::quat rotation;
	glm::mat4 projection;

	void Perspective(int windowWidth, int windowHeight)
	{
		projection = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	}

	void Flat(int windowWidth, int windowHeight)
	{
		auto s = (float)windowWidth / (float)windowHeight;
		projection = glm::ortho(-1.0f * s, +1.0f * s, -1.0f, +1.0f);
	}

	void MoveGlobal(float x, float y, float z)
	{
		position.x -= x;
		position.y -= y;
		position.z -= z;
	}

	void MoveLocal(float x, float y, float z)
	{
		glm::vec4 global_translation = glm::vec4(-x, -y, -z, 1.0f) * glm::toMat4(rotation);
		position += glm::vec3(global_translation);
	}

	void RotateGlobalX(float degrees)
	{
		rotation = glm::angleAxis(-degrees, glm::vec3{ 1, 0, 0 } * glm::inverse(rotation)) * rotation;
	}

	void RotateGlobalY(float degrees)
	{
		rotation = glm::angleAxis(-degrees, glm::vec3{ 0, 1, 0 } * glm::inverse(rotation)) * rotation;
	}

	void RotateGlobalZ(float degrees)
	{
		rotation = glm::angleAxis(-degrees, glm::vec3{ 0, 0, 1 } * glm::inverse(rotation)) * rotation;
	}

	void RotateLocalX(float degrees)
	{
		rotation = glm::angleAxis(-degrees, glm::vec3{ 1, 0, 0 }) * rotation;
	}

	void RotateLocalY(float degrees)
	{
		rotation = glm::angleAxis(-degrees, glm::vec3{ 0, 1, 0 }) * rotation;
	}
	
	void RotateLocalZ(float degrees)
	{
		rotation = glm::angleAxis(-degrees, glm::vec3{ 0, 0, 1 }) * rotation;
	}

	glm::quat LookAt(float x, float y, float z)
	{
		glm::vec3 point(x, y, z);
		return glm::toQuat(glm::lookAt(-position, point, glm::vec3{ 0.0f, 1.0f, 0.0f }));;
	}

	void MoveTo(float x, float y, float z, float scalar)
	{
		position += (glm::vec3(-x, -y, -z) - position) * scalar;
	}

	void RotateTo(glm::quat rotate_to, float scalar)
	{
		rotation = glm::slerp(rotation, glm::inverse(rotate_to), scalar);
	}

	glm::mat4 ProjectionViewMatrix()
	{
		//camera view space is inverted from world space
		glm::mat4 view = glm::toMat4(rotation) * glm::translate(position);
		return projection * view;
	}
};
