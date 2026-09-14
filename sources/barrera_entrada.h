//CPP:sources/barrera_entrada.cpp
#if !defined barrera_entrada_h
#define barrera_entrada_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common.h"
#include "random/lib/randomc.h"


class barrera_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
Time barrier_time;
Time min_time_cross;
Time max_time_cross;
CRandomMersenne * random;
Signal request;
//state
Patente pat;
bool pass;
Message output;

public:
	barrera_entrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
