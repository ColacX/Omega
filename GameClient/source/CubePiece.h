#pragma once

#include <vector>

#include "Piece.h"
#include "CircleConnector.h"
#include "ColladaParser.h"
#include "Camera.h"

struct CubePiece : Piece
{
	static std::vector<CircleConnector> circleConnectors;
	static GLuint texture, shader;
	static ColladaParser p;

	static void Load();

	static void BatchRender(const std::vector<CubePiece*> pieces, Camera* camera);

	void AddConnectors(std::vector<CircleConnector*>& sceneConnectors);
};
