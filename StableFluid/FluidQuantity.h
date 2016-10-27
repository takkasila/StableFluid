#ifndef FLUID_QUANTITY
#define FLUID_QUANTITY

class FluidQuantity
{
public:
	double *data_prev, *data;
	int width, height;
	double cellSize;
	int totalBlock;

	FluidQuantity::FluidQuantity(int width, int height, double cellSize)
		: width(width), height(height), cellSize(cellSize)
	{
		totalBlock = (width + 2) * (height + 2);
		data_prev = new double[totalBlock];
		data = new double[totalBlock];

		std::fill(data_prev, data_prev + (totalBlock), 0);
		std::fill(data, data + (totalBlock), 0);
	}

	inline const double value(int x, int y)
	{
		return data_prev [x+y*width];
	}

	void addSource(double x_begin, double x_end, double y_begin, double y_end, double value)
	{
		int ix_begin = x_begin*width;
		int ix_end = x_end*width;
		int iy_begin = y_begin*height;
		int iy_end = y_end*height;
		
		for (int i = ix_begin+1; i <= ix_end; i++)
			for (int j = iy_begin+1; j <= iy_end; j++)
				data_prev[i + j*(width+2)] = value;
	}

	~FluidQuantity()
	{
		delete [] data_prev;
		delete [] data;
	}


private:

};

#endif