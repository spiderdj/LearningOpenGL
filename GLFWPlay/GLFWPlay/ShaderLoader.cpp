#include "stdafx.h"
#include "ShaderLoader.h"

ShaderLoader::ShaderLoader()
{
}


ShaderLoader::~ShaderLoader()
{
}

std::string ReadShaderFile(const char* shaderPath)
{
	std::string ShaderSource = "";
	std::ifstream shaderStream(shaderPath,std::ios::in);
	
	if (shaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(shaderStream,line))
		{
			ShaderSource += "\n" + line;
		}
		shaderStream.close();
	}
	else
	{
		std::cout << "Failed to open file: " << shaderPath << std::endl;
	}
	return ShaderSource;
}

bool CompileShader(GLuint shaderID, const char* ShaderPath, std::string shaderSource)
{
	GLint result = GL_FALSE;
	int InfoLogLength = 0;
	//Compile Shader
	std::cout << "Compiling shader: " << ShaderPath << std::endl;
	char const* shaderPointer = shaderSource.c_str();
	glShaderSource(shaderID, 1, &shaderPointer, NULL);
	glCompileShader(shaderID);

	//Check it compiled
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> shaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
	if (InfoLogLength > 1)
	{
		std::cout << "Shader Info Log: ";
		for each (char c in shaderErrorMessage)
		{
			std::cout << c;
		}
		std::cout << std::endl;
	}

	return result == GL_TRUE;
}

GLuint ShaderLoader::LoadShaders(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	//Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	//Read the shaders
	std::string VertexShaderSource = ReadShaderFile(vertexShaderPath);
	std::string FragmentShaderSource = ReadShaderFile(fragmentShaderPath);
	//Compile the shaders
	CompileShader(VertexShaderID, vertexShaderPath, VertexShaderSource);
	CompileShader(FragmentShaderID, fragmentShaderPath, FragmentShaderSource);
	//Link the program
	std::cout << "Linking the program" << std::endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	GLint result = GL_FALSE;
	int InfoLogLength = 0;
	//Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> programErrorMessage(std::max(InfoLogLength, int(0)));
	glGetProgramInfoLog(programID, InfoLogLength, NULL, &programErrorMessage[0]);
	if (InfoLogLength > 1)
	{
		std::cout << "Linking Info Log: ";
		for each (char c in programErrorMessage)
		{
			std::cout << c;
		}
		std::cout << std::endl;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return programID;
}
