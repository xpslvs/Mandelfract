/* main.cc */
#include "state.hh"
#include "input.hh"
#include "process.hh"
#include "graphics.hh"

State state;

void handle_status(int);

int main(int argc, char **argv)
{
	graphics::initialize();
	
	while(state.running)
	{
		input::poll();
		
		handle_status(state.status);
		
		graphics::clear();
		graphics::load_pixels();
		graphics::post_process();
		graphics::load_interface();
		graphics::refresh();					

		state.status = Status::NONE;
	}

	process::await();
	graphics::quit();
	return 0;
}

void handle_status(int status)
{
	if(status & Status::DISPATCH_AWAIT)
		process::await(); 
	if(status & Status::TOGGLE_FULLSCREEN)
		graphics::toggle_fullscreen(); 
	if(status & Status::RESIZE)
		graphics::resize(); 
	if(status & Status::SETUP_THREADS)
		process::setup_threads(); 
	if(status & Status::CLEAR)
		graphics::set_invalid(); 
	if(status & Status::SHIFT)
		graphics::shift(); 
	if(status & Status::DISPATCH) 
			process::dispatch(); 
}
