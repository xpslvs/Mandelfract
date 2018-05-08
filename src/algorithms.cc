/* algorithms.cc */
#include "algorithms.hh"
#include "fractal.hh"
#include "complex.hh"
#include "const.h"

int mandelbrot(long double x, long double y, int iterations, ComplexLf *r)
{
	ComplexLf z{0, 0};
	ComplexLf c{x, y};

	int iter = 0;
	while(iter < iterations && z.norm() < 4.0L)
	{
		z = z.square() + c;
		iter++;
	}

	if(r != nullptr)
		*r = z;
	return iter;
}

