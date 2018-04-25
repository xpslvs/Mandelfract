
/* interface.cc */
#include <cstdarg>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "interface.hh"
#include "state.hh"
#include "process.hh"
#include "fractal.hh"
#include "graphics.hh"

#define FONT_PATH        "fonts/cour.ttf"
#define FONT_SIZE         14
#define FORMAT_STACK_SIZE 24
#define FORMAT_DATA_SIZE  48
#define COLOR_FG          0xFFFFFFFF
#define COLOR_BG          0xFF000000
#define CROSSHAIR_RADIUS  7

namespace interface 
{
	enum DISPLAYSTATE : int
	{
		NONE  = 0x00,
		SHOW  = 0x01,
		DEBUG = 0x02,
		HELP  = 0x04,
	};

	struct Format
	{
		SDL_Rect prop;
		char data[FORMAT_DATA_SIZE];
	};

	static void load_interface(void);
	static void render_format(SDL_Renderer *, Format *);
	static void render_interface(SDL_Renderer *);
	static void render_crosshair(SDL_Renderer *);

	static std::vector<Format> stack(FORMAT_STACK_SIZE);
	static TTF_Font           *font;
	static int                 state;

	void initialize(void)
	{
		assert(TTF_Init() != -1);
		font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
		assert(font != NULL);
		toggle_interface();
	}

	void quit(void)
	{
		TTF_CloseFont(font);
		TTF_Quit();
	}
	
	void render(SDL_Renderer *renderer)
	{
		if(! (state & DISPLAYSTATE::SHOW))
			return;
		load_interface();
		render_interface(renderer);
		render_crosshair(renderer);
	}
	
	void push_format(int x, int y, int sz, char const *fmt, ...)
	{
		Format format;
		va_list argv;

		if(sz > FORMAT_DATA_SIZE)
			sz = FORMAT_DATA_SIZE;
		if(sz < 0)
			sz = 0;

		va_start(argv, fmt);
		std::vsnprintf(format.data, sz, fmt, argv);
		TTF_SizeText(font, format.data, &format.prop.w, &format.prop.h);

		if(x == -1) /* Center */
			format.prop.x = (graphics::width- format.prop.w) / 2;
		else
			format.prop.x = x;
		if(y == -1) /* Center */
			format.prop.y = (graphics::height - format.prop.h) / 2;
		else
			format.prop.y = y;
		
		stack.push_back(format);
	}

	void toggle_interface(void)
	{
		state ^= DISPLAYSTATE::SHOW; 
	}

	void toggle_debug(void)
	{
		state ^= DISPLAYSTATE::DEBUG;
	}

	void toggle_help(void)
	{
		state ^= DISPLAYSTATE::HELP;
	}

	static void load_interface(void)
	{
		int const x = FONT_SIZE;
		int const y = graphics::height - 10 * FONT_SIZE;
		int offset;

		if(state & DISPLAYSTATE::HELP)
		{
			push_format(x, FONT_SIZE*1 , 46, "<H>          : Toggle this message            "); 
			push_format(x, FONT_SIZE*2 , 46, "<G>          : Toggle debug information       ");
			push_format(x, FONT_SIZE*3 , 46, "<LCTRL>      : Toggle the interface           ");
			push_format(x, FONT_SIZE*4 , 46, "<ARROWS/WASD>: Move                           ");
			push_format(x, FONT_SIZE*5 , 46, "<+/-/MWHEEL> : Zoom                           ");
			push_format(x, FONT_SIZE*6 , 46, "<Z/X>        : Toggle fractal type            ");
			push_format(x, FONT_SIZE*7 , 46, "<I/O>        : Inc-/decrement max iterations  ");
			push_format(x, FONT_SIZE*8 , 46, "<Q/E>        : Inc-/decrement thread amount   ");
			push_format(x, FONT_SIZE*9 , 46, "<SPACE>      : Take a screenshot              ");
			push_format(x, FONT_SIZE*10, 46, "<F11>        : Toggle fullscreen              ");
			offset = 0;
		}
		else
		{
			push_format(x, y + FONT_SIZE*8, 40, "<H> to display help information         ");
			offset = -FONT_SIZE*2;
		}

		if(state & DISPLAYSTATE::DEBUG)
		{
			push_format(x, y + FONT_SIZE*2 + offset, 44, "Render size:  %dx%d pixels                     ", graphics::width, graphics::height);
			push_format(x, y + FONT_SIZE*3 + offset, 44, "Iterations :  %d                               ", fractal::iterations);
			push_format(x, y + FONT_SIZE*4 + offset, 44, "Threads    :  %d                               ", process::threads);
			push_format(x, y + FONT_SIZE*5 + offset, 44, "Rendered in:  %.3f seconds                     ", process::framerate());
			push_format(x, y + FONT_SIZE*6 + offset, 44, "Scale      :  %llu:1                           ", state::scale);
			push_format(x, y + FONT_SIZE*7 + offset, 44, "X          : %s%.18Lf                          ", state::x < 0.0L ? "" : " ", state::x);
			push_format(x, y + FONT_SIZE*8 + offset, 44, "Y          : %s%.18Lf                          ", state::y < 0.0L ? "" : " ", state::y);
		}
		else
		{
			push_format(x, y + FONT_SIZE*7 + (offset == 0 ? FONT_SIZE : 0), 40, "<G> to display debug information        ");
		}
	}

	static void render_interface(SDL_Renderer *renderer)
	{
		while(! stack.empty())
		{
			render_format(renderer, &stack.back());
			stack.pop_back();
		}
	}

	void render_format(SDL_Renderer *renderer, Format *format)
	{
		SDL_Surface *surface;
		SDL_Texture *texture;
		SDL_Color    foreground, background;
		*(int *)&foreground = COLOR_FG;
		*(int *)&background = COLOR_BG;

		surface = TTF_RenderText_Shaded(font, format->data, foreground, background);
		SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderCopy(renderer, texture, NULL, &format->prop);
		
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
	
	void render_crosshair(SDL_Renderer *renderer)
	{	
		bool (*function)(int, int);

		auto loop = [&](void) -> void
		{
			for(int x = -CROSSHAIR_RADIUS; x <= CROSSHAIR_RADIUS; ++x)
			{	
				for(int y = -CROSSHAIR_RADIUS; y <= CROSSHAIR_RADIUS; ++y)
				{
					if(function(x, y)) 
						SDL_RenderDrawPoint(renderer, (graphics::width / 2) + x, (graphics::height / 2) + y);
				}
			}
		};
	
		/* Draw outline */
		SDL_SetRenderDrawColor
		(
			renderer, 
			(COLOR_FG >> 16) & 0xff, 
			(COLOR_FG >> 8)  & 0xff, 
			(COLOR_FG)       & 0xff,	
			(COLOR_FG >> 24) & 0xff
		);
		function = +[](int x, int y) -> bool
		{ 
			return std::abs(x) == std::abs(y)
				|| std::abs(x) <= 1
				|| std::abs(y) <= 1
				|| std::abs(x-y) <= 1
				|| std::abs(x+y) <= 1
			;
		};
		loop();
		
		/* Draw inline */
		SDL_SetRenderDrawColor
		(
			renderer,
			(COLOR_BG >> 16) & 0xff, 
			(COLOR_BG >> 8)  & 0xff, 
			(COLOR_BG)       & 0xff,	
			(COLOR_BG >> 24) & 0xff
		);
		function = +[](int x, int y) -> bool 
		{ 
			return (std::abs(x) == std::abs(y) 
				|| x == 0
				|| y == 0)
				&& std::abs(x) < CROSSHAIR_RADIUS
				&& std::abs(y) < CROSSHAIR_RADIUS
			; 
		};
		loop();
	}
}

