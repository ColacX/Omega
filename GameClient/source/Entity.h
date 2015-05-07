#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\vector_angle.hpp>

#include "CubePiece.h"
#include "HoverPiece.h"

struct Entity
{
	//all computation are in world space
	glm::vec3 position;
	glm::quat rotation;
	std::vector<CubePiece*> cubePieces;
	std::vector<HoverPiece*> hoverPieces;

	void MoveGlobal(float x, float y, float z);

	void MoveLocal(float x, float y, float z);

	void RotateGlobalX(float degrees);

	void RotateGlobalY(float degrees);

	void RotateGlobalZ(float degrees);

	void RotateLocalX(float degrees);

	void RotateLocalY(float degrees);

	void RotateLocalZ(float degrees);

	glm::quat LookAt(float x, float y, float z);

	void MoveTo(float x, float y, float z, float delta_time);

	void RotateTo(glm::quat rotate_to, float delta_time);

	glm::mat4 ModelMatrix();
};
