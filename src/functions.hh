
/* functions.hh */
#pragma once

#include <tuple>
#include <complex>

/* Macro versions of fractal calls */
#define Mandelbrot(x, y)   mandelbrot  (x, y, STATE::Iterations)
#define Multibrot(x, y, p) multibrot   (x, y, p       , STATE::Iterations)
#define Negabrot(x, y, p)  negabrot    (x, y, p       , STATE::Iterations)
#define Burning_Ship(x, y) burning_ship(x,-y, STATE::Iterations)
#define Julia_0(x, y)      julia       (x, y, 0.285L  , 0.01L   , STATE::Iterations)
#define Julia_1(x, y)      julia       (x, y,-0.70176L, -0.3842L, STATE::Iterations)
#define Julia_2(x, y)      julia       (x, y,-0.7269L , 0.1889L , STATE::Iterations)
#define Julia_3(x, y)      julia       (x, y, 0.279L  , 0.0L    , STATE::Iterations)
#define Julia_4(x, y)      julia       (x, y,-0.624L  , 0.425L  , STATE::Iterations)
#define Julia_5(x, y)      julia       (x, y, 0.0L    , 1.0L    , STATE::Iterations);
#define Julia_6(x, y)      julia       (x, y,-1.0L    , 0.0L    , STATE::Iterations);
#define Newton(x, y, i)    newton      (x, y, i       , STATE::Iterations)
#define Nova(x, y)         nova        (x, y, STATE::Iterations)

namespace fractal 
{
	using val = std::tuple<int, std::complex<long double>>;

	val mandelbrot(long double, long double, int);
	/*i32 multibrot(f80, f80, i32, i32);
	i32 negabrot(f80, f80, i32, i32);
	i32 burning_ship(f80, f80, i32);
	i32 julia(f80, f80, f80, f80, i32);
	i32 newton(f80, f80, i32, i32);
	i32 nova(f80, f80, i32); */
}

