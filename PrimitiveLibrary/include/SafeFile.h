#pragma once

#include <string>
#include <stdio.h>

class SafeFile
{
public:
	FILE* file = 0;

	enum Mode
	{
		ReadText,
		WriteText
	};

	void Open(const std::string filePath, Mode mode);
	void Close();
	void ReadAll(std::string& fileData);
	void OpenReadAll(const std::string filePath, std::string& fileData);
};
