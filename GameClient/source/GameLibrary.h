#pragma once

void ReadFile(const char* filePath, char*& bufferPointer, unsigned int& fileSize)
{
	FILE* file;

	if (fopen_s(&file, filePath, "r"))
	{
		__debugbreak();
		throw "fopen_s";
	}

	fseek(file, 0, SEEK_END);
	fileSize = (unsigned int)ftell(file);
	bufferPointer = new char[fileSize];
	fseek(file, 0, SEEK_SET);
	auto bytesRead = fread(bufferPointer, 1, (size_t)fileSize, file);
	bufferPointer[bytesRead] = 0;
	fclose(file);
}

void CompileShader(const char* filePath, std::vector<char*>& sourceVector, GLuint shaderId)
{
	char* textData;
	unsigned int text_dataSize;
	ReadFile(filePath, textData, text_dataSize);
	sourceVector.push_back(textData);
	glShaderSource(shaderId, (GLsizei)sourceVector.size(), (const GLchar**)&sourceVector[0], 0);
	glCompileShader(shaderId);
	int status = 0;
	glGetObjectParameterivARB(shaderId, GL_OBJECT_COMPILE_STATUS_ARB, &status);

	if (!status)
	{
		GLbyte infoLog[1000];
		glGetInfoLogARB(shaderId, sizeof(infoLog), 0, (GLcharARB*)infoLog);
		fprintf(stderr, "infoLog:\n%s\n", infoLog);
		__debugbreak();
		throw "glcompile_shader";
	}

	delete[] textData;
}

GLuint CreateShaderProgram(const char* vertSource, const char* fragSource, const char* geomSource)
{
	GLuint programObject = glCreateProgram();

	if (vertSource != 0)
	{
		GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
		std::vector<char*> sourceVector;
		sourceVector.push_back("#define VERT\n");
		fprintf(stderr, "vertex shader %s compiling...\n", vertSource);
		CompileShader(vertSource, sourceVector, shaderId);
		glAttachShader(programObject, shaderId);
	}

	if (geomSource != 0)
	{
		GLuint shaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		std::vector<char*> sourceVector;
		sourceVector.push_back("#define GEOM\n");
		fprintf(stderr, "geometry shader %s compiling...\n", geomSource);
		CompileShader(geomSource, sourceVector, shaderId);
		glAttachShader(programObject, shaderId);
	}

	if (fragSource != 0)
	{
		GLuint shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		std::vector<char*> sourceVector;
		sourceVector.push_back("#define FRAG\n");
		fprintf(stderr, "fragment shader %s compiling...\n", fragSource);
		CompileShader(fragSource, sourceVector, shaderId);
		glAttachShader(programObject, shaderId);
	}

	glLinkProgram(programObject);
	return programObject;
}

GLuint LoadImageToTexture(const char* imageFilePath)
{
	SDL_Surface* sdlSurface = IMG_Load(imageFilePath); //can throw an odd error if something is wrong with the image

	if (sdlSurface == nullptr)
	{
		__debugbreak();
		throw "IMG_Load";
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (sdlSurface->format->BitsPerPixel == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sdlSurface->w, sdlSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, sdlSurface->pixels);
	}
	else if (sdlSurface->format->BitsPerPixel == 32)
	{
		if (sdlSurface->format->Rmask == 0x000000ff)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
		}
		else if (sdlSurface->format->Bmask == 0x000000ff)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
		}
		else
		{
			throw "unsupported channel order";
		}
	}
	else
	{
		throw "unsupported BitsPerPixel";
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(sdlSurface);
	return texture_id;
}
