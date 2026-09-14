//CPP:sources/cola_salida.cpp
#if !defined cola_salida_h
#define cola_salida_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common.h"


class cola_salida: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
std::queue<Patente> cars;
bool pass;
Patente output;



public:
	cola_salida(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
