/* fractal.hh */
#ifndef FRACTAL_HH
#define FRACTAL_HH

#define FRACTALS 1

enum Fractal : int
{
	MANDELBROT = 0
};

namespace fractal 
{
	int render(long double, long double);
}

#endif /* FRACTAL_HH */
