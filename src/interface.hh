
/* interface.hh */
#pragma once

#include <SDL2/SDL.h>

namespace interface 
{
	void initialize(void);
	void quit(void);
	void render(SDL_Renderer *);
	void push_format(int, int, int, char const *, ...);
	void toggle_interface(void);
	void toggle_debug(void);
	void toggle_help(void);
}

