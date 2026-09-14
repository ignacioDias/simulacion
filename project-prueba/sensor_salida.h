//CPP:project-prueba/sensor_salida.cpp
#if !defined sensor_salida_h
#define sensor_salida_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class sensor_salida: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time latency;
Time sigma;
Patente patente;
Message output;
public:
	sensor_salida(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
