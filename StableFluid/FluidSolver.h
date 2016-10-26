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
	float *speed_f;

	FluidSolver::FluidSolver(int width, int height)
		:width(width), height(height), cellSize(1.0/std::min(width, height))
	{
		dense = new FluidQuantity(width, height, cellSize);
		speed = new FluidQuantity(width, height, cellSize);
		dense_f = new float[width*height];
		speed_f = new float[width*height];
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
		dense->addSource(0.25, 0.9, 0.25, 0.5, 1);
	}



	void doubleToFloat()
	{
		for (int i = 0; i < width*height; i++)
		{
			dense_f[i] = float(dense->currData[i]);
			speed_f[i] = float(speed->currData[i]);
		}
	}

	void addDense(float x1, float y1, float x2, float y2, float dense)
	{

	}

	inline const double getDense(int x, int y)
	{
		return dense->value(x, y);
	}

	~FluidSolver()
	{
		delete dense;
	}

private:

};
#endif