//CPP:project-prueba/contador_rechazos.cpp
#if !defined contador_rechazos_h
#define contador_rechazos_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class contador_rechazos: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
Patente output;
Patente pat;

public:
	contador_rechazos(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
