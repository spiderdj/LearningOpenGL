// GLFWPlay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "ShaderLoader.h"
using namespace glm;

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f, -1.0f, // triangle 1 : begin
	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f, -1.0f, // triangle 2 : begin
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f, // triangle 2 : end
	1.0f, -1.0f, 1.0f, 
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
static GLfloat g_color_buffer_data[12 * 3 * 3];

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		std::cout << "Failed to initalise GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 32); //4x Antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Major version 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//Minor version 3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //To make mac happy for some reason
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Use the newer version of OPENGL

	GLFWwindow* window; //The window
	window = glfwCreateWindow(800, 600, "Hello World!", NULL, NULL);
	 
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //Make the window the current OpenGL Context
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initalise GLEW" << std::endl;
		return -1;
	}

	for (int i = 0; i < 12*3*3; i++)
	{
		g_color_buffer_data[i] = 0.5f+g_vertex_buffer_data[i] / 2;
		g_color_buffer_data[i + 1] = 0.5f + g_vertex_buffer_data[i + 1] / 4;
		g_color_buffer_data[i + 2] = 0.5f - g_vertex_buffer_data[i+2] / 2;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLuint shaderProgram = ShaderLoader::LoadShaders("C:/Users/Kieran/Documents/GitHub/LearningOpenGL/GLFWPlay/Debug/SimpleVertexShader.vertexshader", "C:/Users/Kieran/Documents/GitHub/LearningOpenGL/GLFWPlay/Debug/SimpleFragmentShader.fragmentshader");

	glm::mat4  projectionMatrix = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 model = glm::mat4(1.0f);


	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

	double lastTime = glfwGetTime();
	do
	{
		double deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glUseProgram(shaderProgram);
		glm::mat4 view = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 MVP = projectionMatrix * view * model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		//Vertex positions
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,(void*)0);
		//Vertex colours
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 12*3);
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);	
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

}    