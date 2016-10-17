#ifndef FLUID_QUANTITY
#define FLUID_QUANTITY
class FluidQuantity
{
	double *currData, *nextData;
public:
	int width, height;
	double cellSize;

	FluidQuantity::FluidQuantity(int width, int height, double cellSize)
		: width(width), height(height), cellSize(cellSize)
	{
		currData = new double[width*height];
		nextData = new double[width*height];
		memset(currData, 0, sizeof(double) * width*height);
	}

	~FluidQuantity()
	{
		delete [] currData;
		delete [] nextData;
	}

	inline const double value(int x, int y)
	{
		return currData [x+y*width];
	}

private:

};

#endif