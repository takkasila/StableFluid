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

		std::fill(currData, currData + (width*height), 0);
	}

	inline const double value(int x, int y)
	{
		return currData [x+y*width];
	}

	void addSource(double x_begin, double x_end, double y_begin, double y_end, double value)
	{
		int ix_begin = x_begin*width;
		int ix_end = x_end*width;
		int iy_begin = y_begin*height;
		int iy_end = y_end*height;
		
		for (int i = ix_begin; i < ix_end; i++)
			for (int j = iy_begin; j < iy_end; j++)
				currData[i + j*width] = value;
	}

	~FluidQuantity()
	{
		delete [] currData;
		delete [] nextData;
	}


private:

};

#endif