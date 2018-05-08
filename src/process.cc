/* process.cc */
#include <cstring>
#include <cstddef>
#include <ctime>
#include <cmath>
#include <pthread.h>
#include <vector>
#include "process.hh"
#include "input.hh"
#include "state.hh"
#include "graphics.hh"
#include "fractal.hh"

namespace process 
{
	struct ThreadData
	{
		pthread_t thread;
		int x, y;
		int width, height;
	};
	
	static ThreadData   threads[MAX_THREADS];
	static volatile int active = 0;

	static void *process_segment(void *);
	
	/* Await all the dispatched threads to finish
	 */
	void await(void)
	{
		pthread_t poll;
		bool flag = true;
		pthread_create(&poll, NULL, input::freeze, &flag);
		for(int i = 0; i < active; ++i)
		{
			pthread_join(threads[i].thread, NULL);
		}
		flag = false;
		pthread_join(poll, NULL);
	}

	/* Assign every thread a rectangular segment of the screen
	 * of a size depending on how many threads are used
	 */
	void setup_threads(void)
	{
		// Thread array needs to be zeroed in order to prevent segfaults
		std::memset(threads, 0L, MAX_THREADS * sizeof(ThreadData));

		active = state.threads;
		int segments = std::sqrt(active);
		int segw = state.width  / segments;
		int segh = state.height / segments;

		for(int y = 0, i = 0; y < segments; ++y)
		{
			for(int x = 0; x < segments; ++x, ++i)
			{
				threads[i].x      = x * segw;
				threads[i].width  = segw;
				threads[i].y      = y * segh;
				threads[i].height = segh;
			}
		}
	}
	
	/* Dispatches all threads, allowing them to work on their assigned
	 * segments
	 */
	void dispatch(void)
	{
		await();
		for(int i = 0; i < active; ++i)
		{
			pthread_create(&threads[i].thread, NULL, process_segment, &threads[i]);
		}
	}

	/* Processes a single segment and writes to video buffer
	 */
	static void *process_segment(void *argp)
	{
		const ThreadData *thread   = (ThreadData *)argp;
		const int         x_offset = thread->x - (state.width / 2);
		const int         y_offset = (state.height / 2) - thread->y;
		long double       x_coord[thread->width];
		long double       y_coord[thread->height];

		// Preload x coordinates
		for(int x = 0; x < thread->width; ++x)
		{
			x_coord[x] = state.x + (long double)(x_offset + x) / state.scale;
		}
		// Preload y coordinates
		for(int y = 0; y < thread->height; ++y)
		{
			y_coord[y] =  state.y + (long double)(y_offset - y) / state.scale;
		}
		
		for(int y = thread->y, j = 0; y < thread->y + thread->height; ++y, ++j)
		{
			for(int x = thread->x, i = 0; x < thread->x + thread->width; ++x, ++i)
			{
				if(graphics::color(x, y) == VALUE_INVALID)
				{
					int color = fractal::render(x_coord[i], y_coord[j]);
					graphics::set_manual(y * state.width + x, color);
				}
			}
		}

		pthread_exit(NULL);
	}
}
