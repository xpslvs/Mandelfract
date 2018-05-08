/* state.hh */
#ifndef STATE_HH
#define STATE_HH

#define PROGRAM  "Mandelfract"
#define VERSION  "1.5.1"
#define MAX_THREADS    256
#define MIN_THREADS    1
#define MAX_ITERATIONS (~(1 << 31))
#define MIN_ITERATIONS 1
#define MAX_SCALE      (~(1L << 63))
#define MIN_SCALE      1

enum Status : int
{
	NONE              = 0x00, /* Nothing to be done */
	DISPATCH_AWAIT    = 0x01, /* Await exit of processing thread(s) */
	SETUP_THREADS     = 0x02, /* Assign area of screen to processing thread(s) */
	DISPATCH          = 0x04, /* Dispatch processing threads and start rendering */
	RESIZE            = 0x08, /* Resize the window and update all relevant variables */
	TOGGLE_FULLSCREEN = 0x10, /* Toggle fullscreen */
	SHIFT             = 0x20, /* Perform a video buffer shift */
	CLEAR             = 0x40, /* Mark every pixel as invalid */
};

struct State
{
	long double x;          /* Camera positon x */
	long double y;          /* Camera position y */
	long long   scale;      /* Zoom level */
	int         threads;    /* Amount of threads rendering */ 
	int         width;      /* Window width */
	int         height;     /* Window height */
	int         fractal;    /* Fractal index */
	int         iterations; /* Maximum iterations generating the fractal */
	int         variable;   /* Fractal specific options */
	int         color;      /* Type of color scheme */
	int         status;     /* Status flag */
	bool        running;    /* Global running flag */

	State(void);
	void move(int, int);
	void zoom(int);
	void switch_threads(int);
	void switch_fractal(int);
	void switch_iterations(int);
	void switch_variable(int);
	void switch_color(int);
	void set_status(int);
	void clear_status(int = 0xffffffff);
};

extern State state;

#endif /* STATE_HH */
