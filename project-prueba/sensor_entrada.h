//CPP:project-prueba/sensor_entrada.cpp
#if !defined sensor_entrada_h
#define sensor_entrada_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common.h"

class sensor_entrada: public Simulator { 
Time sigma;
Time latency;
Patente currentPatente;
Message output;
public:
	sensor_entrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
