#pragma once

#include "HeaderTypes.h"

void ReadFile(const char* filePath, char*& bufferPointer, unsigned int& fileSize);

void CompileShader(const char* filePath, std::vector<char*>& sourceVector, GLuint shaderId);

GLuint CreateShaderProgram(const char* vertSource, const char* fragSource, const char* geomSource);

GLuint LoadImageToTexture(const char* imageFilePath);
