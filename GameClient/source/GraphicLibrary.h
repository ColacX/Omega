//use this in .cpp implementation files only

#pragma once

#define _CRT_SECURE_NO_WARNINGS

//external libraries
#include <Windows.h>

#include <GL\glew.h>
#pragma comment(lib, "glew32.lib")

#include <gl\GL.h>
#pragma comment(lib, "opengl32.lib")

#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#undef main

#include <SDL_image.h>
#pragma comment(lib, "SDL2_image.lib")

#include <SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.lib")

#include <glm/gtc/type_ptr.hpp>

#include "GameLibrary.h"
