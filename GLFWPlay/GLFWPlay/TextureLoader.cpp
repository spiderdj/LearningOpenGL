#include "stdafx.h"
#include "TextureLoader.h"
#include <iostream>

TextureLoader::TextureLoader()
{
}


TextureLoader::~TextureLoader()
{
}

GLuint TextureLoader::loadBMP(const char* imagePath)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char* data;

	FILE* file = fopen(imagePath, "rb");
	if (!file)
	{
		std::cout << "Failed to open image: " << imagePath << std::endl;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << "Not a correct BMP file" << std::endl;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << "Not a correct BMP file" << std::endl;
	}


	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)
	{
		imageSize = width*height * 3;
	}

	if (dataPos = 0)
	{
		dataPos = 54;
	}

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}