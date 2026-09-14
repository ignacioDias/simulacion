//CPP:sources/estacionamiento.cpp
#if !defined estacionamiento_h
#define estacionamiento_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common.h"
#include "random/lib/randomc.h"


class estacionamiento: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
Time max_time_parked;
Time min_time_parked;
CRandomMersenne * random;
std::list<std::tuple<Patente,Time,Time>> cars;
std::pair<Patente, Time> output;

public:
	estacionamiento(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
