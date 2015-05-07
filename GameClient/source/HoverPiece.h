#pragma once

#include "Piece.h"

struct HoverPiece : Piece
{
	glm::quat rotation;

	static void BatchRender(const std::vector<HoverPiece*> pieces);
};
