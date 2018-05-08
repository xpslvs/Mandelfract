/* state.cc */
#include <cstdio>
#include "state.hh"
#include "fractal.hh"

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define THREADS_FACTOR    4
#define ITERATIONS_FACTOR 2
#define MOVEMENT_FACTOR   16   
#define SCALE_FACTOR      2

State::State(void)
{
	this->x           = 0.0L;
	this->y           = 0.0L;
	this->scale       = 128;
	this->threads     = 4;
	this->width       = 768;
	this->height      = 768;
	this->fractal     = 0;
	this->iterations  = 256;
	this->variable    = 0;
	this->color       = 0;
	this->status      = Status::CLEAR | Status::SETUP_THREADS | Status::DISPATCH;
	this->running     = true;
}

void State::move(int dx, int dy)
{
	this->x += (long double)(dx * MOVEMENT_FACTOR) / this->scale;
	this->y += (long double)(dy * MOVEMENT_FACTOR) / this->scale;
}

void State::zoom(int signum)
{
	if(signum > 0)
		this->scale *= SCALE_FACTOR;
	else if(signum < 0)
		this->scale /= SCALE_FACTOR;

	this->scale = MAX(this->scale, MIN_SCALE);	
	this->scale = MIN(this->scale, MAX_SCALE);
}

void State::switch_threads(int signum)
{
	if(signum > 0)
		this->threads *= THREADS_FACTOR;
	else if(signum < 0)
		this->threads /= THREADS_FACTOR;

	this->threads = MAX(this->threads, MIN_THREADS);
	this->threads = MIN(this->threads, MAX_THREADS);
}

void State::switch_fractal(int signum)
{
	if(signum > 0)
		this->fractal++;
	else if(signum < 0)
		this->fractal--;
	this->fractal %= FRACTALS;
}

void State::switch_iterations(int signum)
{
	if(signum > 0)
		this->iterations *= ITERATIONS_FACTOR;
	else if(signum < 0)
		this->iterations /= ITERATIONS_FACTOR;
	
	this->iterations = MAX(this->iterations, MIN_ITERATIONS);
	this->iterations = MIN(this->iterations, MAX_ITERATIONS);
}

void State::switch_variable(int signum)
{

}

void State::switch_color(int signum)
{

}

void State::set_status(int status)
{
	this->status |= status;

}

void State::clear_status(int status)
{
	this->status &= ~status;
}
