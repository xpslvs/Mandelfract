
/* input.cc */
#include <cstddef>
#include <SDL2/SDL.h>
#include "input.hh"
#include "state.hh"
#include "process.hh"
#include "graphics.hh"
#include "interface.hh"
#include "fractal.hh"

namespace input 
{
	static void event_window(int);
	static void event_keyboard(int);
	static void event_mouse_click(SDL_Event const *);
	static void event_mouse_scroll(SDL_MouseWheelEvent const *);

	/* Polls for and simultanously handles input events/data
	 */
	void poll(void)
	{
		volatile SDL_Event event;
		while(SDL_PollEvent((SDL_Event *)&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				state::running = false;
				break;
			case SDL_WINDOWEVENT:
				event_window(event.window.event);
				break;
			case SDL_KEYDOWN:
				event_keyboard(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				event_mouse_click((SDL_Event *)&event);
				break;
			case SDL_MOUSEWHEEL:
				event_mouse_scroll((SDL_MouseWheelEvent *)&event.wheel);
				break;
			default:
				/* Undefined event */
				break;
			}
		}
	}

	void event_window(int event)
	{
		void (*resize)(void) = +[](void) -> void
		{
			graphics::resize();
			process::toggle_refresh();
		};

		if(event == SDL_WINDOWEVENT_RESIZED)
			process::reload(resize);
	}

	void event_keyboard(int key)
	{
		switch(key)
		{
		case SDLK_PLUS: /* Zoom in */	
			state::zoom(1);
			break;
		case SDLK_MINUS: /* Zoom out */
			state::zoom(-1);
			break;
		case SDLK_UP: /* Move up */ 
		case SDLK_w:
			state::move(0, 1);
			break;
		case SDLK_RIGHT: /* Move right */ 
		case SDLK_d:
			state::move(1, 0);
			break;
		case SDLK_DOWN: /* Move down */	
		case SDLK_s:
			state::move(0, -1);
			break;
		case SDLK_LEFT: /* Move left */	
		case SDLK_a:
			state::move(-1, 0);
			break;
		case SDLK_z: /* Toggle fractal type (next) */ 
			fractal::toggle_type(1);
			break;
		case SDLK_x: /* Toggle fractal type (previous) */ 
			fractal::toggle_type(-1);
			break;
		case SDLK_h: /* Toggle help display */ 
			interface::toggle_help();
			break;
		case SDLK_g: /* Toggle debug display */ 
			interface::toggle_debug();
			break;
		case SDLK_LCTRL: /* Toggle interface display */ 
			interface::toggle_interface();
			break;
		case SDLK_o: /* Decrement iterations */ 
			fractal::toggle_iterations(-1);
			break;
		case SDLK_i: /* Increment iterations */ 
			fractal::toggle_iterations(1);
			break;
		case SDLK_e: /* Decrement threads */ 
			process::toggle_threads(-1);
			break;
		case SDLK_q: /* Increment threads */ 
			process::toggle_threads(1);
			break;
		case SDLK_SPACE: /* Take screenshot */ 
			graphics::screenshot();
			break;
		case SDLK_F11: /* Toggle fullscreen */ 
			graphics::toggle_fullscreen();
			break;
		}
		process::toggle_refresh();
	}

	void event_mouse_click(SDL_Event const *event)
	{
		if(event->button.button == SDL_BUTTON_LEFT)
		{
			state::x += (long double)(event->motion.x - (graphics::width / 2))  / state::scale;
			state::y += (long double)((graphics::height / 2) - event->motion.y) / state::scale;
			process::toggle_refresh();
		}
		
	}

	void event_mouse_scroll(SDL_MouseWheelEvent const *event)
	{
		state::zoom(event->y);
		process::toggle_refresh();
	}
}

