#ifndef FLUID_QUANTITY
#define FLUID_QUANTITY
class FluidQuantity
{
public:
	double *currData, *nextData;
	int width, height;
	double cellSize;
	int totalBlock;

	FluidQuantity::FluidQuantity(int width, int height, double cellSize)
		: width(width), height(height), cellSize(cellSize)
	{
		totalBlock = (width + 2) * (height + 2);
		currData = new double[totalBlock];
		nextData = new double[totalBlock];

		std::fill(currData, currData + (totalBlock), 0);
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
		
		for (int i = ix_begin+1; i <= ix_end; i++)
			for (int j = iy_begin+1; j <= iy_end; j++)
				currData[i + j*(width+2)] = value;
	}

	~FluidQuantity()
	{
		delete [] currData;
		delete [] nextData;
	}


private:

};

#endif