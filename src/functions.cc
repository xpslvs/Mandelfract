
/* functions.cc */
#include <cmath>
#include <tuple>
#include <complex>
#include "functions.hh"

#define DEVIATION_LIMIT    2.0L
#define DEVIATION_LIMIT_SQ 4.0L

namespace fractal 
{
	using val = std::tuple<int, std::complex<long double>>;

	val mandelbrot(long double x, long double y, int iterations)
	{
		std::complex<long double> z{0.0L, 0.0L};
		std::complex<long double> c{x,    y};
		
		int iter = 1;
		do
		{
			z = z*z + c;
			++iter;
		}while(std::norm(z) <= DEVIATION_LIMIT_SQ && iter < iterations);

		return {iter, z};
	}
/*
	i32 multibrot(f80 x, f80 y, i32 exponent, i32 iterations)
	{
		const Complex<f80> c(x, y);
		Complex<f80> z;

		for(i32 i = 1; i < iterations; ++i)
		{
			//z = z.pow(exponent) + c;

			if(z.norm() >= SQUARE(MANDELBROT_CONVERGENCE))
				return i;
		}

		return iterations;
	}

	i32 negabrot(f80 x, f80 y, i32 exponent, i32 iterations)
	{	
		const Complex<f80> c(x, y);
		Complex<f80> z;

		for(u32 i = 1; i < iterations; ++i)
		{
			//z = z.exponentiate(exponent) + c;
		
				return i;
		}

		return iterations;
	}

	i32 julia(f80 x, f80 y, f80 r, f80 i, i32 iterations)
	{
		const Complex<f80> c(r, i);
		Complex<f80> z(x, y);

		for(i32 i = 1; i < iterations; ++i)
		{
			z = z.square() + c;

			if(z.norm() >= SQUARE(MANDELBROT_CONVERGENCE))
				return i;
		}

		return iterations;
	}

	i32 burning_ship(f80 x, f80 y, i32 iterations)
	{
		const Complex<f80> c(x, y);
		Complex<f80> z;

		for(u32 i = 1; i < iterations; ++i)
		{
			z.set(ABSOLUTE(z.real()), ABSOLUTE(z.imag()));
			z = z.square() + c;

			if(z.norm() >= SQUARE(MANDELBROT_CONVERGENCE))
				return i;
		}

		return iterations;
	}

	i32 newton(f80 x, f80 y, i32 iterations, i32 type)
	{
		return 0;
	}

	i32 nova(f80 x, f80 y, i32 iterations)
	{
		return 0;
	}*/
}

