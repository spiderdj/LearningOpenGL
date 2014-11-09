#pragma once
#include "GL\glew.h"
class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	static GLuint loadBMP(const char*imagePath)
};

