
/* state.hh */
#pragma once

#define STATE_PROG    "Mandelfract"
#define STATE_VERSION "1.4.0"

namespace state
{
	extern volatile bool        running;
	extern volatile long double x;
	extern volatile long double y;
	extern volatile long long   scale;
	
	void move(int, int);
	void zoom(int);
}

