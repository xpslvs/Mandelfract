/* state.cc */
#include "state.hh"
#include "graphics.hh"

#define MOVEMENT_MULTIPLIER (16)   
#define SCALE_MULTIPLIER    (2)

namespace state
{
	volatile bool        running    = true;
	volatile long double x          = 0.0L;
	volatile long double y          = 0.0L;
	volatile long long   scale      = 128;

	void move(int dx, int dy)
	{
		x += (long double)(dx * MOVEMENT_MULTIPLIER) / scale;
		y += (long double)(dy * MOVEMENT_MULTIPLIER) / scale;
		graphics::shift();
	}

	void zoom(int signum)
	{
		if(signum > 0)
			scale *= SCALE_MULTIPLIER;
		else if(signum < 0)
			scale /= SCALE_MULTIPLIER;
		scale = scale ? scale : 1;
	}
}

