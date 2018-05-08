/* fractal.cc */
#include <cstddef>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "fractal.hh"
#include "algorithms.hh"
#include "state.hh"
#include "const.h"
#include "complex.hh"

#define SET_COLOR (0x7f0000) 

namespace fractal 
{
	static int color_fractionalize(int color, float fraction)
	{
		int const r = ((color >> 16) & 0xff) * fraction;
		int const g = ((color >> 8)  & 0xff) * fraction;
		int const b = ((color)       & 0xff) * fraction;

		return (r << 16) | (g << 8) | (b);

	}

	static int hexcolor(int index)
	{
		const int color[] = {
			0x00007f,
			0x00003f,
			0x00007f,
			0x0000ff,
			0x7f3f3f,
			0xff7f00,
			0xffff00,
			0x1fff00,
			0x00ff00,
			0xff0000,
			0xffffff,
			0x000000
		};

		return color[index % 12];
	}

	static int gradient(float fraction)
	{
		float indexfraction = fraction * (12 - 1);
		int   iindex = (int)indexfraction % 12;
		float findex = indexfraction - iindex;
		
		int colorlow  = color_fractionalize(hexcolor(iindex), 1.0f - findex);
		int colorhigh = color_fractionalize(hexcolor(iindex + 1), findex);

		return colorlow + colorhigh;
		

	}
	
	int render(long double x, long double y)
	{
		ComplexLf z;
		int iter;
		int iterations = state.iterations;
		
		std::srand(std::time(NULL));
		iter = mandelbrot(x, y, iterations, &z);
		
		double color = iter + 1 - std::log(std::log(z.modulus())) / LN_2;
		float fraction = color / iterations;
		int   rcol  = gradient(fraction);

		return iter >= iterations ? SET_COLOR : rcol;
	}
}
