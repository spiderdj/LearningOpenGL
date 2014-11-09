#pragma once
#include "GL\glew.h"
#include <algorithm>
#include <iostream>
#include <ios>
#include <fstream>
#include <string>
#include <vector>
class ShaderLoader
{
public:
	ShaderLoader();
	~ShaderLoader();

	static GLuint LoadShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
};

