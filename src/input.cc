/* input.cc */
#include <cstddef>
#include <pthread.h>
#include <SDL2/SDL.h>
#include "input.hh"
#include "state.hh"
#include "graphics.hh"
#include "interface.hh"

namespace input 
{
	static void event_window(int);
	static void event_keyboard(int);
	static void event_mouse_click(SDL_Event const *);
	static void event_mouse_scroll(SDL_MouseWheelEvent const *);

	/* Continously poll whilst flag evaluates to true
	 */
	void *freeze(void *flag)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event) && *(bool *)flag)
		{
			if(event.type == SDL_QUIT)
				std::exit(0);
		}
		pthread_exit(NULL);
	}

	/* Polls for and simultanously handles input events/data
	 */
	void poll(void)
	{
		SDL_Event event;
		while(SDL_PollEvent((SDL_Event *)&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				state.running = false;
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
			}
		}
	}

	void event_window(int event)
	{
		if(event == SDL_WINDOWEVENT_RESIZED)
			state.set_status(Status::DISPATCH_AWAIT 
					       | Status::RESIZE
						   | Status::SETUP_THREADS
						   | Status::CLEAR
						   | Status::DISPATCH
		     );
	}

	void event_keyboard(int key)
	{
		switch(key)
		{
		case SDLK_PLUS: /* Zoom in */	
			state.zoom(1);
			state.set_status(Status::CLEAR);
			break;
		case SDLK_MINUS: /* Zoom out */
			state.zoom(-1);
			state.set_status(Status::CLEAR);
			break;
		case SDLK_UP: /* Move up */ 
		case SDLK_w:
			state.move(0, 1);
			break;
		case SDLK_RIGHT: /* Move right */ 
		case SDLK_d:
			state.move(1, 0);
			break;
		case SDLK_DOWN: /* Move down */	
		case SDLK_s:
			state.move(0, -1);
			break;
		case SDLK_LEFT: /* Move left */	
		case SDLK_a:
			state.move(-1, 0);
			break;
		case SDLK_z: /* Toggle fractal type (next) */ 
			state.switch_fractal(1);
			state.set_status(Status::CLEAR);
			break;
		case SDLK_x: /* Toggle fractal type (previous) */ 
			state.switch_fractal(-1);
			state.set_status(Status::CLEAR);
			break;
		case SDLK_r:
			state.set_status(Status::CLEAR);
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
			state.switch_iterations(-1);
			state.set_status(Status::CLEAR);
			break;
		case SDLK_i: /* Increment iterations */ 
			state.switch_iterations(1);
			state.set_status(Status::CLEAR);
			break;
		case SDLK_e: /* Decrement threads */ 
			state.switch_threads(-1);
			state.set_status(Status::DISPATCH_AWAIT | Status::SETUP_THREADS | Status::CLEAR);
			break;
		case SDLK_q: /* Increment threads */ 
			state.switch_threads(1);
			state.set_status(Status::DISPATCH_AWAIT | Status::SETUP_THREADS | Status::CLEAR);
			break;
		case SDLK_SPACE: /* Take screenshot */ 
			graphics::screenshot();
			break;
		case SDLK_F11: /* Toggle fullscreen */ 
			state.set_status(Status::DISPATCH_AWAIT | Status::TOGGLE_FULLSCREEN | Status::RESIZE | Status::SETUP_THREADS | Status::CLEAR);
			break;
		}
		state.set_status(Status::SHIFT | Status::DISPATCH);
	}

	void event_mouse_click(SDL_Event const *event)
	{
		if(event->button.button == SDL_BUTTON_LEFT)
		{
			state.x += (long double)(event->motion.x - (state.width / 2))  / state.scale;
			state.y += (long double)((state.height / 2) - event->motion.y) / state.scale;
			state.set_status(Status::SHIFT);
		}
	}

	void event_mouse_scroll(SDL_MouseWheelEvent const *event)
	{
		state.zoom(event->y);
		state.set_status(Status::CLEAR);
	}
}
