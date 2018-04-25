
/* process.hh*/
#pragma once

namespace process 
{
	extern volatile int threads;

	void execute(void);
	void interrupt(void);
	void reload(void (*)(void));
	void pause(void);
	void toggle_threads(int);	
	void toggle_refresh(int = 3);
	float framerate(void);
}

