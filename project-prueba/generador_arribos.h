//CPP:project-prueba/generador_arribos.cpp
#if !defined generador_arribos_h
#define generador_arribos_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common.h"
#include "random/lib/randomc.h"
#include "continuous/mathexpr.h"


class generador_arribos: public Simulator { 
Time maxTime;
Time sigma;
Patente currentPatente;
double mean;
CRandomMersenne * random;
double output;
public:
	generador_arribos(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
