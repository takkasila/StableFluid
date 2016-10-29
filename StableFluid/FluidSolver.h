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
	FluidQuantity *speed_x;
	FluidQuantity *speed_y;
	double diffusion_rate;
	//For rendering
	float *dense_f;

	FluidSolver::FluidSolver(int width, int height, double diffusion_rate)
		:width(width), height(height), diffusion_rate(diffusion_rate), cellSize(1.0/std::min(width, height))
	{
		dense = new FluidQuantity(width, height, cellSize);
		speed_x = new FluidQuantity(width, height, cellSize);
		speed_y = new FluidQuantity(width, height, cellSize);
		dense_f = new float[(width+2)*(height+2)];
	}

	void Update(double timeStep)
	{
		velocityStep(timeStep);
		densityStep(timeStep);
		doubleToFloat();
	}

	void velocityStep(double timeStep)
	{
		diffuse(speed_x->data, speed_x->data_prev, timeStep, 1, 0);
		diffuse(speed_y->data, speed_y->data_prev, timeStep, 0, 1);
		project(speed_x->data, speed_y->data);
		advect(speed_x->data_prev, speed_x->data, speed_x->data, speed_y->data, timeStep, 1, 0);
		advect(speed_y->data_prev, speed_y->data, speed_x->data, speed_y->data, timeStep, 0, 1);
		project(speed_x->data_prev, speed_y->data_prev);
		SWAP(speed_x->data, speed_x->data_prev);
		SWAP(speed_y->data, speed_y->data_prev);
	}

	void densityStep(double timeStep)
	{
		diffuse(dense->data, dense->data_prev, timeStep, false, false);
		advect(dense->data_prev, dense->data, speed_x->data, speed_y->data, timeStep, false, false);
		SWAP(dense->data, dense->data_prev);
	}

	void diffuse(double *data, double *data_prev, double timeStep, bool u_cond, bool v_cond)
	{
		double coeff1 = timeStep*diffusion_rate*width*height;
		double coeff2 = 1 + 4 * coeff1;
		linear_solver(data, data_prev, coeff1, coeff2, u_cond, v_cond);
	}

	void advect(double *data, double *data_prev, double *u, double *v, double timeStep, bool u_cond, bool v_cond)
	{
		double x, y;
		int i0, i1, j0, j1;
		for (int j = 1; j <= height; j++)
			for (int i = 1; i <= width; i++)
			{
				//Traceback particle
				x = i - u[AT(i, j)]*timeStep*width;
				y = j - v[AT(i, j)]*timeStep*height;
				//Screen boundary condition
				if (x < 0.5) x = 0.5;
				if (x > width + 0.5) x = width + 0.5;
				if (y < 0.5) y = 0.5;
				if (y > height + 0.5) y = height + 0.5;
				//Find four closest neighbor
				i0 = (int) x;
				i1 = i0 + 1;
				j0 = (int) y;
				j1 = j0 + 1;
				//Interpolate four closest neighbor
				data[AT(i, j)] =
					lerp( 
						lerp(data_prev[AT(i0, j0)], data_prev[AT(i1, j0)], x - i0) // bottom
						, lerp(data_prev[AT(i0, j1)], data_prev[AT(i1, j1)], x - i0) // top
						, y-j0);
			}
		set_boundary(data, u_cond, v_cond);
	}

	void project(double *u, double *v)
	{
		double *p = new double[(width + 2)*(height + 2)];
		double *div = new double[(width + 2)*(height + 2)];

		for (int j = 1; j <= height; j++)
			for (int i = 1; i <= width; i++)
			{
				div[AT(i, j)] = -0.5*cellSize*(u[AT(i + 1, j)] - u[AT(i - 1, j)] 
					+ v[AT(i, j + 1)] - v[AT(i, j - 1)]);
				p[AT(i, j)] = 0;
			}
		set_boundary(div, false, false);
		set_boundary(p, false, false);
		
		linear_solver(p, div, 1, 4, 0, 0);

		for (int j = 1; j <= height; j++)
			for (int i = 1; i <= width; i++)
			{
				u[AT(i, j)] -= 0.5*(p[AT(i + 1, j)] - p[AT(i - 1, j)])/cellSize;
				v[AT(i, j)] -= 0.5*(p[AT(i, j + 1)] - p[AT(i, j - 1)])/cellSize;
			}
		set_boundary(u, true, false);
		set_boundary(v, false, true);
	}

	void linear_solver(double* data, double *data_prev, double coeff1, double coeff2, bool u_cond, bool v_cond)
	{
		for (int k = 0; k < 20; k++)
		{
			for (int j = 1; j <= height; j++)
				for (int i = 1; i <= width; i++)
				{
					data[AT(i, j)] =
						(
						data_prev[AT(i, j)]
						+
						coeff1 *
						(
						data[AT(i - 1, j)] + data[AT(i + 1, j)] + data[AT(i, j - 1)] + data[AT(i, j + 1)]
						)
						)
						/
						(coeff2);
				}
			set_boundary(data, u_cond, v_cond);
		}
	}

	void set_boundary(double *data, bool u_cond, bool v_cond)
	{
		//Edge
		int coef_u = u_cond == true ? -1 : 1;
		int coef_v = v_cond == true ? -1 : 1;
		for (int i = 1; i <= width; i++)
		{
			data[AT(i, 0)] = data[AT(i, 1)] * coef_u;
			data[AT(i, height + 1)] = data[AT(i, height)] * coef_u;
		}
		for (int j = 1; j <= height; j++)
		{
			data[AT(0, j)] = data[AT(1, j)] * coef_v;
			data[AT(width + 1, j)] = data[AT(width, j)] * coef_v;
		}
		//Corner
		data[AT(0, 0)] = (data[AT(0, 1)] + data[AT(1, 0)]) / 2;
		data[AT(width + 1, 0)] = (data[AT(width, 0)] + data[AT(width + 1, 1)]) / 2;
		data[AT(width + 1, height + 1)] = (data[AT(width, height + 1)] + data[AT(width + 1, height)]) / 2;
		data[AT(0, height+1)] = (data[AT(1, height+1)] + data[AT(0, height)]) / 2;
	}

	void addFlow(double x_begin, double x_end, double y_begin, double y_end, double dense, double speed_x, double speed_y)
	{
		this->dense->addSource(x_begin, x_end, y_begin, y_end, dense);
		this->speed_x->addSource(x_begin, x_end, y_begin, y_end, speed_x);
		this->speed_y->addSource(x_begin, x_end, y_begin, y_end, speed_y);
	}

	void doubleToFloat()
	{
		for (int i = 0; i < (width + 2)*(height + 2); i++)
			dense_f[i] = dense->data[i];
	}
	
	inline int AT(int i, int j)
	{
		return i + j*(width + 2);
	}

	inline void SWAP(double *a, double *b)
	{
		double* temp = a;
		a = b;
		b = temp;
	}

	~FluidSolver()
	{
		delete dense;
		delete speed_x;
		delete speed_y;
		delete [] dense_f;
	}

	inline double lerp(double a, double b, double amount)
	{
		return a + (b - a)*glm::clamp<double>(amount, 0, 1);
	}

private:

};
#endif