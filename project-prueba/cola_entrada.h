//CPP:project-prueba/cola_entrada.cpp
#if !defined cola_entrada_h
#define cola_entrada_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class cola_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
std::queue<Patente> cars;
bool flag;
Patente output;
public:
	cola_entrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
