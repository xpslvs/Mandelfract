
/* graphics.hh */
#pragma once

namespace graphics 
{	
	extern volatile int width;
	extern volatile int height;

	void initialize(void);
	void quit(void);
	void resize(void);
	void toggle_fullscreen(void);
	void screenshot(void);
	void set(int, int, int);
	void set_manual(int, int);
	int  get(int, int);
	void clear(void);
	void load_pixels(void);
	void load_interface(void);
	void post_process(void);
	void shift(void);
	void refresh(void);
}

