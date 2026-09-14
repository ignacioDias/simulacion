//CPP:sources/barrera_salida.cpp
#if !defined barrera_salida_h
#define barrera_salida_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common.h"
#include "random/lib/randomc.h"


class barrera_salida: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
Time barrier_time;
Time min_time_cross;
Time max_time_cross;
CRandomMersenne * random;
Patente pat;
bool pass;
Message output;

public:
	barrera_salida(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
