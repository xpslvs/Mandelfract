/* graphics.hh */
#ifndef GRAPHICS_HH
#define GRAPHICS_HH

namespace graphics 
{
	void initialize(void);
	void quit(void);
	void resize(void);
	void toggle_fullscreen(void);
	void screenshot(void);
	void set(int, int, int);
	void set_manual(int, int);
	int  color(int, int);
	void clear(void);
	void load_pixels(void);
	void load_interface(void);
	void post_process(void);
	void set_invalid(void);
	void shift(void);
	void refresh(void);
}

#endif /* GRAPHICS_HH */
