
/* graphics.cc */
#include <cassert>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <SDL2/SDL.h>
#include "graphics.hh"
#include "interface.hh"
#include "process.hh"
#include "state.hh"

namespace graphics
{
	volatile int width  = 768;
	volatile int height = 768;

	static SDL_Window   *window   = NULL;
	static SDL_Renderer *renderer = NULL;
	static SDL_Texture  *texture  = NULL;
	static int          *vbuffer  = NULL;

	void initialize(void)
	{
		assert(SDL_Init(SDL_INIT_EVERYTHING) != -1);
		window = SDL_CreateWindow
		(
			STATE_PROG " v" STATE_VERSION,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
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
		
		SDL_GetWindowSize(window, (int *)&width, (int *)&height); 
		assert(width > 0);
		assert(height > 0);

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
			width, 	
			height	
		);
		assert(texture != NULL);
		vbuffer = new int[width * height];
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
		void (*fullscreen)(void) = +[](void) -> void
		{
			if(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)
				SDL_SetWindowFullscreen(window, 0);
			else
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			resize();
		};
		process::reload(fullscreen);
	}

	void screenshot(void)
	{
		char title[64];
		SDL_Surface *shot;
		int plength;
		
		shot = SDL_CreateRGBSurface
		(
			0,
			width,
			height,
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
		std::sprintf(title, "screenshots/%d.bmp", std::time(NULL));
		SDL_SaveBMP(shot, title);
		plength = std::strlen(title);
		interface::push_format(-1, (height / 2) + 24, plength, "Saved as %s", title);

		SDL_FreeSurface(shot);
	}
	
	void set_manual(int index, int color)
	{
		vbuffer[index] = color;
	}

	void set(int x, int y, int color)
	{
		set_manual(y * width + x, color);
	}
	
	int get(int x, int y)
	{
		return vbuffer[y * width + x];
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
			width * sizeof(int)
		);
		SDL_RenderCopy
		(
			renderer,
			texture,
			NULL, 
			NULL
		);
	}

	void shift(void)
	{
		static long double dx = 0;
		static long double dy = 0;
		dx = state::x - dx;
		dy = state::y - dy;
		int sx = dx * state::scale;
		int sy = dy * state::scale;

		printf("(dx, dy) = (%d, %d)\n", sx, sy);

		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				int ox = x + sx;
				int oy = y - sy;
				if((ox < 0 || ox >= width)
				|| (oy < 0 || oy >= height))
					set(x, y, -1);
				else
					set(x, y, get(ox, oy));
			}
		}

		dx = state::x;
		dy = state::y;
	}

	void post_process(void)
	{

	}

	void refresh(void)
	{
		SDL_RenderPresent(renderer);
	}
}

