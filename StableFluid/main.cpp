#include <stdio.h>
#include <vector>
#include <sstream>

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"

#include "LoadShader.h"
#include "FluidQuantity.h"
#include "FluidSolver.h"

using namespace glm;
using namespace std;

//typedef unsigned long DWORD;
//extern "C" {	// Force using Nvidia GPU. Turn 0 if don't want to.
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}

GLFWwindow* window;
float screeen_width = 500;
float screeen_height = 500;

int initProgram();

int main()
{
	if (initProgram() != 0)
		return -1;

	ShaderGenerator shaderProgram;
	shaderProgram.AddShader("pass_vert.glsl", GL_VERTEX_SHADER);
	shaderProgram.AddShader("fluid_frag.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgramID = shaderProgram.LinkProgram();

	FluidSolver fluid(screeen_width, screeen_height);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data [] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uniformScreenSizeLocation = glGetUniformLocation(shaderProgramID, "screenSize");
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgramID);

		glUniform2f(uniformScreenSizeLocation, screeen_width, screeen_height);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0
			, 3
			, GL_FLOAT
			, GL_FALSE
			, 0
			, (void*)0
		);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	return 0;
}


int initProgram()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to init glfw\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	window = glfwCreateWindow(screeen_width, screeen_height, "Stable FLuid @JosStam", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open glfw window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to init GLEW");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, screeen_width / 2, screeen_height / 2);

	glClearColor(0.1, 0.1, 0.1, 1);

	//Uniform Double
	glEnable(GL_ARB_gpu_shader_fp64);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	return 0;
}