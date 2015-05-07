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

	void Perspective(int windowWidth, int windowHeight);

	void Flat(int windowWidth, int windowHeight);

	void MoveGlobal(float x, float y, float z);

	void MoveLocal(float x, float y, float z);

	void RotateGlobalX(float degrees);

	void RotateGlobalY(float degrees);

	void RotateGlobalZ(float degrees);

	void RotateLocalX(float degrees);

	void RotateLocalY(float degrees);
	
	void RotateLocalZ(float degrees);

	glm::quat LookAt(float x, float y, float z);

	void MoveTo(float x, float y, float z, float scalar);

	void RotateTo(glm::quat rotate_to, float scalar);

	glm::mat4 ProjectionViewMatrix();
};
