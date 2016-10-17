#include <stdio.h>
#include <sstream>

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"

#include "FluidQuantity.h"
#include "FluidSolver.h"
GLFWwindow* window;
float window_width = 400;
float window_height = 400;

int initProgram();

int main()
{
	if (initProgram() != 0)
		return -1;

	FluidSolver fluid(window_width, window_height);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);






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

	window = glfwCreateWindow(window_width, window_height, "Stable FLuid @JosStam", NULL, NULL);
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
	glfwSetCursorPos(window, window_width / 2, window_height / 2);

	glClearColor(0.5, 0.5, 0.5, 1);

	//Z-Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	return 0;
}