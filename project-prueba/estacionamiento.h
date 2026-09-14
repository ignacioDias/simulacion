//CPP:project-prueba/estacionamiento.cpp
#if !defined estacionamiento_h
#define estacionamiento_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class estacionamiento: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
std::list<std::tuple<Patente,Time,Time>> cars;	
std::tuple<Patente,Time> output;
Time minTimeParked;
Time maxTimeParked;
CRandomMersenne * random;

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
