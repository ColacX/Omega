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

		for (int ia = 0; ia < 100; ia++)
		{
			auto c = new CubePiece();
			c->position = glm::vec3(ia, 0, 0);
			cubePieces.push_back(c);
			c->AddConnectors(sceneConnectors);
		}
	}

	void Render()
	{
		CubePiece::BatchRender(cubePieces, &camera);
		auto l = glm::lookAt(glm::vec3(), glm::vec3(0, +1, 0), glm::vec3(0, 0, 1));
		auto q = glm::toQuat(glm::lookAt(glm::vec3(), glm::vec3(0, +1, 0), glm::vec3(0, 0, 1)));
		auto r = q * glm::vec3(0, 0, 1);

		glm::vec3 rayOrigin = (-camera.position);
		glm::vec3 rayDirection = glm::rotate(glm::inverse(camera.rotation), glm::vec3(0, 0, -1));
		float closestDistance = std::numeric_limits<float>::max();
		float originDistance = std::numeric_limits<float>::max();
		CircleConnector* closestConnector = 0;

		for (int ia = 0; ia < sceneConnectors.size(); ia++)
		{
			auto c = sceneConnectors[ia];
			c->RayHit(rayOrigin, rayDirection, originDistance);

			if (originDistance < closestDistance)
			{
				closestDistance = originDistance;
				closestConnector = c;
			}
		}

		//CircleConnector::BatchRender(sceneConnectors, &camera);

		if (closestConnector != 0)
		{
			CircleConnector::BatchRender({ closestConnector }, &camera);
		}

		CrossAim::BatchRender({ &crossAim }, &flatCamera);
	}
};