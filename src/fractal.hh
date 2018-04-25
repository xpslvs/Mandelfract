
/* fractal.hh */
#pragma once

namespace fractal 
{
	extern volatile int iterations;

	int render(long double, long double);
	void toggle_type(int);
	void toggle_iterations(int);
}

