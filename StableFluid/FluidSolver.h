#ifndef FLUID_SOLVER
#define FLUID_SOLVER
#include <math.h>
#include <algorithm>

#include "FluidQuantity.h"

class FluidSolver
{
public:
	int width;
	int height;
	double cellSize;
	FluidQuantity *dense;
	FluidQuantity *speed;
	//For rendering
	float *dense_f;

	FluidSolver::FluidSolver(int width, int height)
		:width(width), height(height), cellSize(1.0/std::min(width, height))
	{
		dense = new FluidQuantity(width, height, cellSize);
		speed = new FluidQuantity(width, height, cellSize);
		dense_f = new float[width*height];
	}

	void Update(double timeStep)
	{
		velocityStep();
		densityStep();
		doubleToFloat();
	}

	void velocityStep()
	{

	}

	void densityStep()
	{
		dense->addSource(0.25, 0.75, 0.25, 0.75, 1);
	}

	void diffuse()
	{

	}

	void advect()
	{

	}

	void addDense(float x1, float y1, float x2, float y2, float dense)
	{

	}

	void doubleToFloat()
	{
		int count = 0;
		for (int i = 1; i <= width; i++)
			for (int j = 1; j <= height; j++)
				dense_f[(i - 1) + (j - 1)*width] = float(dense->currData[i + j*(width + 2)]);
	}

	~FluidSolver()
	{
		delete dense;
	}

private:

};
#endif