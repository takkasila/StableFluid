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
	double diffusion_rate;
	//For rendering
	float *dense_f;

	FluidSolver::FluidSolver(int width, int height, double diffusion_rate)
		:width(width), height(height), diffusion_rate(diffusion_rate), cellSize(1.0/std::min(width, height))
	{
		dense = new FluidQuantity(width, height, cellSize);
		speed = new FluidQuantity(width, height, cellSize);
		dense_f = new float[width*height];
	}

	void Update(double timeStep)
	{
		velocityStep(timeStep);
		densityStep(timeStep);
		doubleToFloat();
	}

	void velocityStep(double timeStep)
	{

	}

	void densityStep(double timeStep)
	{
		dense->addSource(0.25, 0.75, 0.25, 0.75, 1);
		diffuse(dense->nextData, dense->currData, timeStep);
		advect();
	}

	void setEqual(double* a, double* b)
	{
		for (int i = 0; i < (width + 2)*(height + 2); i++)
			a[i] = b[i];
	}

	void diffuse(double *x, double *x_prev, double timeStep)
	{
		printf("Calculating timestep: %f\n", timeStep);
		double coeff = timeStep*diffusion_rate*width*height;
		for (int k = 0; k < 20; k++)
		{
			for (int j = 1; j <= height; j++)
				for (int i = 1; i <= width; i++)
				{
					x[AT(i, j)] =
						(
							x_prev[AT(i, j)]
							+
							coeff *
							(
								x[AT(i-1, j)] + x[AT(i+1, j)] + x[AT(i, j-1)] + x[AT(i, j+1)]
							)
						)
						/
						(1+4*coeff);
				}
			set_boundary(x);
		}
		printf("Yea!\n");

	}

	void advect()
	{

	}

	void set_boundary(double *x)
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
				dense_f[(i - 1) + (j - 1)*width] = float(dense->nextData[AT(i,j)]);
	}
	
	inline int AT(int i, int j)
	{
		return i + j*(width + 2);
	}

	~FluidSolver()
	{
		delete dense;
	}

private:

};
#endif