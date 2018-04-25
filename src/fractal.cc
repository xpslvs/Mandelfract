#include <cstdio>
/* fractal.cc */
#include <cmath>
#include <tuple>
#include "fractal.hh"
#include "functions.hh"
#include "state.hh"
#include "const.h"

#define NAN_BITPATTERN        (0x7fa00000)
#define NNAN_BITPATTERN       (0xffa00000)
#define SET_COLOR             (-1)
#define FRACTAL_TYPES         (1)
#define ITERATIONS_MULTIPLIER (2)

namespace fractal 
{
	volatile int        iterations = 128;
	static volatile int type       = 0;

	static int fibonacci(int n)
	{
		return (std::pow(PHI, n) - std::pow(-PSI, n)) / SQRT_5;
	}

	static int color_fractionalize(int color, float fraction)
	{
		int const r = ((color >> 16) & 0xff) * fraction;
		int const g = ((color >> 8)  & 0xff) * fraction;
		int const b = ((color)       & 0xff) * fraction;

		return (r << 16) | (g << 8) | (b);

	}

	static int gcd(int a, int b)
	{
		if(b == 0)
		{
			if(a == 0)
				return 0;
			else
				return 1;
		}
		int mod = a % b;
		if(mod == 0)
			return b;
		else
			return gcd(b, mod);
	}
	
	static int hexcolor(int index)
	{
		const int color[] = {
			0x0000ff,
			0xff0000,
			0x00ff00,
			0xffff00,
			0x000000,
			0xffffff
		};

		return color[index % 6];
	}

	static int gradient(float fraction)
	{
		/*const int color[] = {
			0xffffff,
			0xff0000,
			0x00ff00,
			0x0000ff,
			0xffff00,
			0x000000
		};

		int idx = (int)(fraction * 6) % 6;
		return color[idx];*/ 
		int const gradients = 256;	

		float indexfraction = fraction * (6 - 1);
		int   iindex = (int)indexfraction % 6;
		float findex = indexfraction - iindex;
		
		int colorlow  = color_fractionalize(hexcolor(iindex), 1.0f - findex);
		int colorhigh = color_fractionalize(hexcolor(iindex + 1), findex);

		return colorlow + colorhigh;
		

	}
	
	int render(long double x, long double y)
	{
		std::tuple<int, std::complex<long double>> value = mandelbrot(x, y, iterations);
		int const iter = std::get<0>(value);
		std::complex<long double> const z = std::get<1>(value);
		
		double color = iter + 1 - std::log(std::log(std::abs(z))) / LN_2;
		float fraction = color / iterations;
		int   rcol  = gradient(fraction);

		return iter >= iterations ? SET_COLOR : rcol;

	}

	void toggle_type(int signum)
	{
		if(signum > 0)
			++type;
		else if(signum < 0)
			--type;
		type %= FRACTAL_TYPES;
	}

	void toggle_iterations(int signum)
	{
		if(signum > 0)
			iterations *= ITERATIONS_MULTIPLIER;
		else if(signum < 0)
			iterations /= ITERATIONS_MULTIPLIER;
		iterations = iterations ? iterations : 1;
	}
}

