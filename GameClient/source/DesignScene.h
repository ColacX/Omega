#pragma once

struct DesignScene
{
	Camera camera, flatCamera;
	CrossAim crossAim;
	std::vector<CubePiece*> cubePieces;
	std::vector<CircleConnector*> sceneConnectors;

	void Construct(int windowWidth, int windowHeight)
	{
		camera.Perspective(windowWidth, windowHeight);
		flatCamera.Flat(windowWidth, windowHeight);
		CrossAim::Load();
		CubePiece::Load();
		CircleConnector::Load();

		for (int ia = 0; ia < 1; ia++)
		{
			auto c = new CubePiece();
			c->position = glm::vec3(ia, 0, 0);
			cubePieces.push_back(c);
			c->AddConnectors(sceneConnectors);
		}
	}

	CircleConnector* FindConnector()
	{
		glm::vec3 rayOrigin = (-camera.position);
		glm::vec3 rayDirection = glm::rotate(glm::inverse(camera.rotation), glm::vec3(0, 0, -1));
		float closestDistance = std::numeric_limits<float>::max();
		float originDistance = std::numeric_limits<float>::max();
		CircleConnector* closestConnector = 0;

		for (int ia = 0; ia < sceneConnectors.size(); ia++)
		{
			auto c = sceneConnectors[ia];

			if (c->RayHit(rayOrigin, rayDirection, originDistance) && originDistance < closestDistance)
			{
				closestDistance = originDistance;
				closestConnector = c;
			}
		}

		return closestConnector;
	}

	void Render()
	{
		CubePiece::BatchRender(cubePieces, &camera);
		
		auto closestConnector = FindConnector();
		//CircleConnector::BatchRender(sceneConnectors, &camera);
		
		if (closestConnector != 0)
		{
			CircleConnector::BatchRender({ closestConnector }, &camera);
		}

		CrossAim::BatchRender({ &crossAim }, &flatCamera);
	}

	void TryAddPiece()
	{
		auto closestConnector = FindConnector();

		if (closestConnector != 0)
		{
			auto c = new CubePiece();
			c->position = closestConnector->position;
			c->rotation = closestConnector->rotation;
			cubePieces.push_back(c);
			c->AddConnectors(sceneConnectors);
		}
	}
};