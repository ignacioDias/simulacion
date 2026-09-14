//FLAGS:-std=c++11
//CPP:sources/contador_capacidad.cpp
#if !defined contador_capacidad_h
#define contador_capacidad_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "queue"
#include "./common_controller.h"


class contador_capacidad: public Simulator { 
int n;
int max_capacity;
Time sigma;

// Cola para manejar la concurrencia
std::queue<Action> response_queue;

InternalSignal signal_output;
double occupancy_output;

public:
	contador_capacidad(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
