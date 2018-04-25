
/* process.cc */
#include <cstddef>
#include <ctime>
#include <cmath>
#include <pthread.h>
#include "process.hh"
#include "state.hh"
#include "graphics.hh"
#include "fractal.hh"

#define THREADS_MULTIPLIER            4
#define MAXIMUM_CONSECUTIVE_REFRESHES 3

namespace process 
{
	enum PROCSIG
	{
		STOP,
		IDLE,
		EXEC
	};

	struct ThreadData
	{
		pthread_t thread;
		int       x[2];
		int       y[2];
	};

	volatile int threads = 4;

	static pthread_t    procthread = (pthread_t)NULL;
	static volatile int signal     = PROCSIG::IDLE;
	static volatile int refresh    = MAXIMUM_CONSECUTIVE_REFRESHES;
	static volatile int rate       = 0;

	static void assign_segments(ThreadData *);
	static void dispatch_threads(ThreadData *, int);
	static void *process(void *);
	static void *process_segment(void *);
	
	void execute(void)
	{
		signal = PROCSIG::EXEC;
		pthread_create(&procthread, NULL, process, NULL);
	}

	void interrupt(void)
	{
		signal = PROCSIG::STOP;
		pthread_join(procthread, NULL);
	}

	void reload(void (*function)(void))
	{
		interrupt();
		if(function != NULL) 
			function(); 
		execute();
	}

	void pause(void)
	{
		signal = signal == PROCSIG::IDLE ? PROCSIG::EXEC : PROCSIG::IDLE;
	}

	void toggle_refresh(int c)
	{
		refresh += c;
		if(refresh > MAXIMUM_CONSECUTIVE_REFRESHES || refresh < 0)
			refresh = MAXIMUM_CONSECUTIVE_REFRESHES;
	}

	void toggle_threads(int signum)
	{
		if(signum > 0)
			threads *= THREADS_MULTIPLIER;
		else if(signum < 0)
			threads /= THREADS_MULTIPLIER;
		threads = threads ? threads : 1;
		reload(NULL);
	}

	float framerate(void)
	{
		return (float)rate / CLOCKS_PER_SEC;
	}
	
	/* Assign every thread a rectangular segment of the screen
	 * of a size depending on how many threads are used
	 */
	static void assign_segments(ThreadData *data, int count)
	{
		int const segments  = std::sqrt(count);
		int const segwidth  = graphics::width  / segments;
		int const segheight = graphics::height / segments;

		int i = 0;
		for(int x = 0; x < segments; ++x)
		{
			for(int y = 0; y < segments; ++y)
			{
				data[i].x[0] = x * segwidth;
				data[i].x[1] = data[i].x[0] + segwidth;
				data[i].y[0] = y * segheight;
				data[i].y[1] = data[i].y[0] + segheight;

				++i;
			}
		}
	}
	
	/* Dispatches all threads, allowing them to work on their assigned
	 * segments
	 */
	static void dispatch_threads(ThreadData *data, int count)
	{
		// Dispatch threads
		for(int i = 0; i < count; ++i)
		{
			pthread_create(&data[i].thread, NULL, process_segment, &data[i]);
		}
		// Wait for return
		for(int i = 0; i < count; ++i)
		{
			pthread_join(data[i].thread, NULL);
		}
	}
	
	/* This is the main loop, it dispatches all threads
	 * and then makes the necessary calls to render
	 */
	static void *process(void *argp)
	{
		// Threads need to be saved as a local variable, otherwise
		// segfaults may occurr when toggling thread amount
		int const count = threads;  
		ThreadData thread_array[count]; 
		assign_segments(thread_array, count);

		while(state::running && signal != PROCSIG::STOP) 
		{
			if(refresh == 0 || signal == PROCSIG::IDLE)
				continue;

			while(refresh--)
			{
				rate = std::clock(); 
				dispatch_threads(thread_array, count);
				rate = std::clock() - rate; 
			}
			
			graphics::clear();
			graphics::load_pixels();
			graphics::post_process();
			graphics::load_interface();
			graphics::refresh();
		}

		pthread_exit(NULL);
	}

	/* Processes a single segment and writes to video buffer
	 */
	static void *process_segment(void *argp)
	{
		ThreadData const *data = (ThreadData *)argp;
		
		// Preload x coordinates
		long double const base_x  = state::x;
		int const         relat_x = data->x[0] - (graphics::width/ 2);
		int const         size_x  = data->x[1] - data->x[0];
		long double       x_coord[size_x];
		for(int x = 0; x < size_x; ++x)
		{
			x_coord[x] = base_x + ((long double)(relat_x + x) / state::scale);
		}
		
		// Preload y coordinates
		long double const base_y  = state::y;
		int const         relat_y = (graphics::height / 2) - data->y[0];
		int const         size_y  = data->y[1] - data->y[0];
		long double       y_coord[size_y];
		for(int y = 0; y < size_y; ++y)
		{
			y_coord[y] =  base_y + (long double)(relat_y - y) / state::scale;
		}
		
		int i = 0;
		for(int x = data->x[0]; x < data->x[1]; ++x)
		{
			int j = 0;
			for(int y = data->y[0]; y < data->y[1]; ++y)
			{
				if(graphics::get(x, y) != -1)
					continue;
				int const c = fractal::render(x_coord[i], y_coord[j]);
				graphics::set(x, y, c);
				++j;
			}
			++i;
		}
		
		pthread_exit(NULL);
	}
}

