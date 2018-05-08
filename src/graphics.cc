/* graphics.cc */
#include <cassert>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <SDL2/SDL.h>
#include "graphics.hh"
#include "interface.hh"
#include "process.hh"
#include "state.hh"

namespace graphics
{
	static SDL_Window   *window   = NULL;
	static SDL_Renderer *renderer = NULL;
	static SDL_Texture  *texture  = NULL;
	static int          *vbuffer  = NULL;

	void set_invalid(void)
	{
		std::memset(vbuffer, VALUE_INVALID, state.width * state.height * sizeof(int));
	}

	void initialize(void)
	{
		assert(SDL_Init(SDL_INIT_EVERYTHING) != -1);

		window = SDL_CreateWindow
		(
			PROGRAM " v" VERSION,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			state.width,
			state.height,
			SDL_WINDOW_RESIZABLE
		);
		assert(window != NULL);
		resize();
		interface::initialize();
	}

	void resize(void)
	{
		if(texture != NULL)
			SDL_DestroyTexture(texture);
		if(renderer != NULL)
			SDL_DestroyRenderer(renderer);
		if(vbuffer != NULL)
			delete[] vbuffer;
		
		SDL_GetWindowSize(window, &state.width, &state.height); 

		renderer = SDL_CreateRenderer
		(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
		);

		assert(renderer != NULL);
		texture = SDL_CreateTexture
		(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			state.width, 	
			state.height	
		);
		assert(texture != NULL);

		vbuffer = new int[state.width * state.height];
		assert(vbuffer != NULL);
	}

	void quit(void)
	{
		delete[] vbuffer;
		interface::quit();
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void toggle_fullscreen(void)
	{
		if(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)
			SDL_SetWindowFullscreen(window, 0);
		else
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}

	void screenshot(void)
	{
		SDL_Surface *shot;
		char title[64];
		int  length;
		
		shot = SDL_CreateRGBSurface
		(
			0,
			state.width,
			state.height,
			32,
			0x00FF0000,
			0x0000FF00,
			0x000000FF,
			0xFF000000
		);
		assert(shot != NULL);
		SDL_RenderReadPixels
		(
			renderer,
			NULL,
			SDL_PIXELFORMAT_ARGB8888,
			shot->pixels,
			shot->pitch
		);
		std::sprintf(title, "screenshots/%d.bmp%n", (int)std::time(NULL), &length);
		SDL_SaveBMP(shot, title);
		interface::push_format(-1, (state.height / 2) + 24, length, "Saved as %s", title);

		SDL_FreeSurface(shot);
	}
	
	void set_manual(int index, int color)
	{
		vbuffer[index] = color;
	}

	void set(int x, int y, int color)
	{
		set_manual(y * state.width + x, color);
	}
	
	int color(int x, int y)
	{
		return vbuffer[y * state.width + x];
	}

	void clear(void)
	{
		SDL_RenderClear(renderer);
	}

	void load_interface(void)
	{
		interface::render(renderer);
	}
	
	void load_pixels(void)
	{
		SDL_UpdateTexture
		(
			texture,
			NULL,
			vbuffer,
			state.width * sizeof(int)
		);
		SDL_RenderCopy
		(
			renderer,
			texture,
			NULL, 
			NULL
		);
	}

	/* Shifts the video buffer according to how the coordinates moved
	 * since last render
	 */
	void shift(void)
	{
		static long double px = 0;
		static long double py = 0;
		
		const int dx = (state.x - px) * state.scale;
		const int dy = (py - state.y) * state.scale;
		
		auto lambda = [=](int x, int y) -> void
		{
			const int rx = x + dx;
			const int ry = y + dy;
			if(rx < 0 || rx >= state.width || ry < 0 || ry >= state.height)
				set(x, y, VALUE_INVALID);
			else
				set(x, y, color(rx, ry));
		};
		
		for(int y  = (dy < 0 ? state.height-1 : 0)
		   ;    y != (dy < 0 ? -1             : state.height)
		   ;    y += (dy < 0 ? -1             : 1))
		{
			for(int x  = (dx < 0 ? state.width-1 : 0)
			   ;    x != (dx < 0 ? -1            : state.width)
			   ;    x += (dx < 0 ? -1            : 1))
			{
				lambda(x, y);
			}
		}

		px = state.x;
		py = state.y;
	}

	void post_process(void)
	{

	}

	void refresh(void)
	{
		SDL_RenderPresent(renderer);
	}
}
