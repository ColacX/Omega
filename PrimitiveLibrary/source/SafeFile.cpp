#include "../include/SafeFile.h"

void SafeFile::Open(const std::string filePath, Mode mode)
{
	try
	{
		switch (mode)
		{
		case ReadText:
			fopen_s(&file, filePath.c_str(), "r");
			break;
		case WriteText:
			fopen_s(&file, filePath.c_str(), "w");
		}
	}
	catch (...)
	{
		throw "could not open file";
	}

	if (file == 0)
		throw "could not open file";
}

void SafeFile::Close()
{
	fclose(file);
}

void SafeFile::ReadAll(std::string& fileData)
{
	try
	{
		fseek(file, 0, SEEK_END);
		auto fileSize = ftell(file) + 1;
		auto bufferPointer = new char[fileSize];
		fseek(file, 0, SEEK_SET);
		auto bytesRead = fread(bufferPointer, 1, (size_t)fileSize, file);
		bufferPointer[bytesRead] = 0;
		fileData = std::string(bufferPointer);
		delete[] bufferPointer;
	}
	catch (...)
	{
		throw "could not read file";
	}
}

void SafeFile::OpenReadAll(const std::string filePath, std::string& fileData)
{
	Open(filePath, Mode::ReadText);
	ReadAll(fileData);
}
