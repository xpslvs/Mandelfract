
/* main.cc */
#include "state.hh"
#include "input.hh"
#include "process.hh"
#include "graphics.hh"

static void initialize(void);
static void quit(void);

int main(int argc, char **argv)
{
	initialize();
	
	process::execute();
	while(state::running)
	{
		input::poll();
	}

	quit();
	return 0;
}

void initialize(void)
{
	graphics::initialize();
}

static void quit(void)
{
	process::interrupt();
	graphics::quit();
}
	
