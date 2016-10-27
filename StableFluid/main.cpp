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

typedef unsigned long DWORD;
extern "C" {	// Force to use Nvidia GPU. Turn 0 if don't want to.
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLFWwindow* window;
static int screeen_width = 500;
static int screeen_height = 500;
static int cell_x = 100;
static int cell_y = 100;

int initProgram();

int main()
{
	if (initProgram() != 0)
		return -1;

	ShaderGenerator shaderProgram;
	shaderProgram.AddShader("pass_vert.glsl", GL_VERTEX_SHADER);
	shaderProgram.AddShader("fluid_frag.glsl", GL_FRAGMENT_SHADER);
	GLuint shaderProgramID = shaderProgram.LinkProgram();

	FluidSolver fluid(cell_x, cell_y, 0.01);

	//VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//VBO
	static const GLfloat g_vertex_buffer_data [] = {
		-1.0f, -1.0f, 0.0f, //bottom-right
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, //top-left
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//TBO
	GLuint u_densityFloat_tbo_tex = glGetUniformLocation(shaderProgramID, "u_densityFloat_tbo_tex");

	GLuint tbo;
	glGenBuffers(1, &tbo);

	GLuint tbo_tex;
	glGenTextures(1, &tbo_tex);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	//Uniform Screen
	GLuint u_screenSize_location = glGetUniformLocation(shaderProgramID, "screenSize");

	//Uniform Cell Number
	GLuint u_cellCount_location = glGetUniformLocation(shaderProgramID, "cellCount");

	//Timer
	double lastTime = 0, currTime;
	glfwSetTime(0);
	do
	{
		//Update fluid
		currTime = glfwGetTime();
		//fluid.addFlow(0.25, 0.75, 0.25, 0.5, 1, 1, 1);
		fluid.dense->addSource(0.25, 0.75, 0.25, 0.5, 1);
		fluid.speed_x->addSource(0, 1, 0, 1, 0.2);
		fluid.speed_y->addSource(0, 1, 0, 1, 0.2);
		fluid.Update(currTime-lastTime);
		lastTime = currTime;

		//Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgramID);
		glUniform2i(u_screenSize_location, screeen_width, screeen_height);
		glUniform2i(u_cellCount_location, cell_x, cell_y);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//TBO
		glBindBuffer(GL_TEXTURE_BUFFER, tbo);
		glBufferData(GL_TEXTURE_BUFFER, sizeof(float)*cell_x*cell_y, fluid.dense_f, GL_STATIC_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, tbo_tex);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, tbo);
		glUniform1i(u_densityFloat_tbo_tex, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
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

	glClearColor(0, 0.0, 0.0, 1);

	//Uniform Double
	glEnable(GL_ARB_gpu_shader_fp64);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	return 0;
}