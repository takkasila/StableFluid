#ifndef FLUID_SOLVER
#define FLUID_SOLVER
#include <math.h>
#include <algorithm>

#include "FluidQuantity.h"

class FluidSolver
{
	FluidQuantity *density;
	int width, height;
	double cellSize;

public:
	FluidSolver::FluidSolver(int width, int height)
		:width(width), height(height), cellSize(1.0/std::min(width, height))
	{
		density = new FluidQuantity(width, height, cellSize);
	}

	void Update()
	{

	}

	inline const double getDense(int x, int y)
	{
		return density->value(x, y);
	}

	~FluidSolver()
	{
		delete density;
	}

private:

};
#endif