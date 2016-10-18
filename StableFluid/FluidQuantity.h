#ifndef FLUID_QUANTITY
#define FLUID_QUANTITY
class FluidQuantity
{
public:
	double *currData, *nextData;
	int width, height;
	double cellSize;

	FluidQuantity::FluidQuantity(int width, int height, double cellSize)
		: width(width), height(height), cellSize(cellSize)
	{
		currData = new double[width*height];
		nextData = new double[width*height];

		//std::fill(currData, currData + (width*height), 1.0);
		for (int i = 1; i <= width*height; i++)
		{
			currData[i-1] = double(i) / double(width*height);
		}
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