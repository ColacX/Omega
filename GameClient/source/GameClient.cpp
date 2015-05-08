//opengl coordinate system
//x right
//y up
//z outwards

#include "GraphicLibrary.h"

//standard libraries
#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <unordered_map>

//custom libraries
#include <XmlParser.h>
#include <SafeFile.h>
#pragma comment(lib, "PrimitiveLibrary.lib")

#include "Resource.h"

#include "ColladaParser.h"
#include "Camera.h"
#include "Piece.h"
#include "CubePiece.h"
#include "CircleConnector.h"
#include "Circle.h"


struct Triangle
{
	glm::vec3 position;
};


class IGround
{
	virtual float GetY(float  x, float z)
	{
		//height map interpolation?
		return 0;
	}
};

class HorisontalConnector
{
	glm::vec3 position;
};

class VerticalConnector
{
	glm::vec3 position;
};

class PointConnector
{
	glm::vec3 position;
};

struct BoundingBox
{
	glm::vec3 position;
	float width, height, depth;
};

struct BoundingCube
{
	glm::vec3 position;
	float size;
};

struct CrossAim
{
	static GLuint texture, shader;
	static ColladaParser p;

	static void Load()
	{
		{
			auto s = "script/textured.glsl";
			shader = CreateShaderProgram(s, s, 0);
			glUseProgram(shader);
			glUniform1i(glGetUniformLocation(shader, "sampler0"), 0);
			glUseProgram(0);
		}

		texture = LoadImageToTexture("binary/CrossAim1.tif");
		//texture = LoadImageToTexture("binary/cross-hairs.png");

		SafeFile f;
		std::string fileData;
		f.OpenReadAll("script/CrossAim0.DAE", fileData);
		p.Parse(fileData.c_str());
	}

	static void BatchRender(const std::vector<CrossAim*> crossAims, Camera* camera)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		glUseProgram(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		for (auto node = p.currentScene->nodes.begin(); node != p.currentScene->nodes.end(); node++)
		{
			auto& n = node->second;

			//auto& as = p.animations["Box001-anim"].sources["Box001-Matrix-animation-output-transform"];
			/*auto animationMatrix = glm::transpose(glm::make_mat4(&as.floats[0 + 16 * (frameCount % 21)]));
			auto modelMatrix = animationMatrix; */
			glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(0.1, 0.1, 0.1));
			glm::mat4 m = camera->ProjectionViewMatrix() * model;
			glUniformMatrix4fv(glGetUniformLocation(shader, "pvmMatrix"), 1, false, glm::value_ptr(m));

			auto& g = p.geometries[n.geometryId];
			auto& vertices = g.vertices[g.triangleVerticesId];
			auto& pos = g.sources[vertices.positionSourceId].floats;
			auto& uvs = g.sources[vertices.texcoordSourceId].floats;

			glBegin(GL_TRIANGLES);

			for (int ia = 0; ia < g.triangleIndices.size(); ia++)
			{
				auto vertexId = g.triangleIndices[ia];
				float x = pos[vertexId * 3 + 0];
				float y = pos[vertexId * 3 + 1];
				float z = pos[vertexId * 3 + 2];
				float u = uvs[vertexId * 2 + 0];
				float v = uvs[vertexId * 2 + 1];

				glVertexAttrib2f(1, u, -v);  glVertexAttrib3f(0, x, y, z);
				//printf("%f %f %f\n", x, y, z);
			}

			glEnd();
		}
	}
};

GLuint CrossAim::texture;
GLuint CrossAim::shader;
ColladaParser CrossAim::p;


#include "SplashScene.h"
#include "DesignScene.h"

class GameClient
{
public:
	TTF_Font* textFont;
	SDL_Window* sdlWindow;
	SDL_GLContext sdlContext;
	int windowWidth = 800;
	int windowHeight = 800;
	bool gameRunning = true;
	bool freeMouse = true;
	std::unordered_map<SDL_Keycode, bool> keyStates;
	GLuint texturedShader;
	GLuint defaultTexture;
	Camera* camera;
	ColladaParser p;
	int frameCount = 0;
	CrossAim crossAim;
	std::vector<CubePiece*> cubePieces;
	DesignScene designScene;

	void Construct()
	{
		SafeFile sf;
		std::string fd;
		//sf.OpenReadAll("script/plane0.DAE", fd);
		sf.OpenReadAll("script/minimal0.DAE", fd);
		p.Parse(fd.c_str());

		if (SDL_Init(SDL_INIT_EVENTS) != 0)
			throw "SDL_Init";

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		sdlWindow = SDL_CreateWindow("Omega", 10, 30, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		//SDL_SetWindowBordered(sdlWindow, SDL_FALSE);
		SDL_MaximizeWindow(sdlWindow);
		SDL_GetWindowSize(sdlWindow, &windowWidth, &windowHeight);
		sdlContext = SDL_GL_CreateContext(sdlWindow);
		SDL_GL_MakeCurrent(sdlWindow, sdlContext);

		if (glewInit() != 0)
			throw "glewInit";

		if (TTF_Init() == -1)
			throw "TTF_Init";

		textFont = TTF_OpenFont("binary/consola.ttf", 20);

		if (!textFont)
			throw "TTF_OpenFont";

		Resource::Construct();

		designScene.Construct(windowWidth, windowHeight);
		camera = &designScene.camera;
	}

	void Destruct()
	{

	}

	void ProcessInteraction()
	{
		//process sdl events
		SDL_Event e;
		float m = 0.1f;

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				exit(0); //forced quit
				gameRunning = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					gameRunning = false;
					break;
				case SDLK_TAB:
					if (!freeMouse)
					{
						SDL_SetRelativeMouseMode(SDL_FALSE);
						freeMouse = true;
					}
					break;
				case SDLK_F2:
					SDL_GL_SetSwapInterval(1); //enable vertical sync
					break;
				case SDLK_F3:
					SDL_GL_SetSwapInterval(0); //disable vertical sync
				default:
					keyStates[e.key.keysym.sym] = true;
					break;
				}
				break;
			case SDL_KEYUP:
				keyStates[e.key.keysym.sym] = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				//printf("button: %d %d\n", e.button.x, e.button.y);
				if (freeMouse)
				{
					SDL_SetRelativeMouseMode(SDL_TRUE);
					freeMouse = false;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				designScene.TryAddPiece();
				break;
			case SDL_MOUSEMOTION:
				if (freeMouse)
					break;

				camera->RotateLocalX(-e.motion.yrel * m);
				camera->RotateLocalY(-e.motion.xrel * m);
				break;
			default:
				break;
			}
		}

		if (keyStates[SDLK_LSHIFT])
			m *= 10.0f;

		if (keyStates[SDLK_LALT])
			m *= 0.1f;

		if (keyStates[SDLK_a])
			camera->MoveLocal(-m, 0, 0);

		if (keyStates[SDLK_d])
			camera->MoveLocal(+m, 0, 0);

		if (keyStates[SDLK_w])
			camera->MoveLocal(0, 0, -m);

		if (keyStates[SDLK_s])
			camera->MoveLocal(0, 0, +m);

		if (keyStates[SDLK_SPACE])
			camera->MoveLocal(0, +m, 0);

		if (keyStates[SDLK_LCTRL])
			camera->MoveLocal(0, -m, 0);

		if (keyStates[SDLK_q])
			camera->RotateLocalZ(+10.0f * m);

		if (keyStates[SDLK_e])
			camera->RotateLocalZ(-10.0f * m);
	}

	void ProcessSimulation()
	{
		//for (auto ia = p.animations.begin(); ia != p.animations.end(); ia++)
		//{
		//	auto& a = ia->second;
		//	auto& n = p.currentScene->nodes[a.targetNodeId];
		//	a.sources[]
		//	n.matrix;
		//}
	}

	void Test()
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glUseProgram(texturedShader);
		glUniformMatrix4fv(glGetUniformLocation(texturedShader, "pvmMatrix"), 1, false, glm::value_ptr(camera->ProjectionViewMatrix()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, defaultTexture);

		glBegin(GL_TRIANGLE_STRIP);
		//texcoord; position;
		glVertexAttrib2f(1, 0, 1); glVertexAttrib2f(0, -1, -1); //bottom left
		glVertexAttrib2f(1, 1, 1); glVertexAttrib2f(0, +1, -1); //bottom right
		glVertexAttrib2f(1, 0, 0); glVertexAttrib2f(0, -1, +1); //top left
		glVertexAttrib2f(1, 1, 0); glVertexAttrib2f(0, +1, +1); //top right
		glEnd();

		for (auto node = p.currentScene->nodes.begin(); node != p.currentScene->nodes.end(); node++)
		{
			auto& n = node->second;
			//printf("%s\n", n.id.c_str());

			//auto& as = p.animations["Box001-anim"].sources["Box001-Matrix-animation-output-transform"];
			/*auto animationMatrix = glm::transpose(glm::make_mat4(&as.floats[0 + 16 * (frameCount % 21)]));
			auto modelMatrix = animationMatrix; */
			auto modelMatrix = glm::transpose(glm::make_mat4(&n.matrix[0]));
			glUniformMatrix4fv(glGetUniformLocation(texturedShader, "pvmMatrix"), 1, false, glm::value_ptr(camera->ProjectionViewMatrix() * modelMatrix));

			auto& g = p.geometries[n.geometryId];
			auto& vertices = g.vertices[g.triangleVerticesId];
			auto& pos = g.sources[vertices.positionSourceId].floats;
			auto& uvs = g.sources[vertices.texcoordSourceId].floats;

			glBegin(GL_TRIANGLES);

			for (int ia = 0; ia < g.triangleIndices.size(); ia++)
			{
				auto vertexId = g.triangleIndices[ia];
				float x = pos[vertexId * 3 + 0];
				float y = pos[vertexId * 3 + 1];
				float z = pos[vertexId * 3 + 2];
				float u = uvs[vertexId * 2 + 0];
				float v = uvs[vertexId * 2 + 1];

				glVertexAttrib2f(1, u, -v);  glVertexAttrib3f(0, x, y, z);
				//printf("%f %f %f\n", x, y, z);
			}

			glEnd();
		}
	}

	void RenderFrame()
	{
		glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		designScene.Render();
		
		SDL_GL_SwapWindow(sdlWindow);
	}

	void Run()
	{
		glm::vec4 v(1, 0, 0, 0);
		glm::quat q;

		glm::quat a(3, 1, 6, 3);
		glm::quat b(6, 7, 2, 1);
		auto c = a * b;

		auto rotatedV = glm::rotate(q, v);

		while (gameRunning)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			ProcessInteraction();
			ProcessSimulation();
			RenderFrame();

			auto endTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
			auto fps = 1.0f / elapsedTime;
			//printf("et(ns): %f\n", elapsedTime);
			printf("fps: %f\n", fps);
			frameCount++;
		}
	}
};

void main()
{
	printf("main: start\n");

	GameClient gc;
	gc.Construct();
	gc.Run();

	printf("main: end\n");
}
