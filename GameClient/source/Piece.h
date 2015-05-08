#pragma once

#include "HeaderTypes.h"

struct Piece
{
	glm::vec3 position;
	glm::quat rotation;
};

struct BlockPiece : Piece
{
	static void BatchRender(const std::vector<BlockPiece*> pieces)
	{
	}
};

struct WheelPiece : Piece
{
	glm::quat rotation;

	static void BatchRender(const std::vector<WheelPiece*> pieces)
	{
	}
};

struct HumanLegPiece : Piece
{
	glm::quat rotation;

	static void BatchRender(const std::vector<HumanLegPiece*> pieces)
	{
	}
};

struct SpiderLegPiece : Piece
{
	glm::quat rotation;

	static void BatchRender(const std::vector<SpiderLegPiece*> pieces)
	{

	}
};
