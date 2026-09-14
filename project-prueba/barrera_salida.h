//CPP:project-prueba/barrera_salida.cpp
#if !defined barrera_salida_h
#define barrera_salida_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class barrera_salida: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
Message output;
Time maxCrossingTime;
Time minCrossingTime;
Time barrierTime;
CRandomMersenne * random;
Patente pat;
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
