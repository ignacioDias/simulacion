//CPP:project-prueba/barrera_entrada.cpp
#if !defined barrera_entrada_h
#define barrera_entrada_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class barrera_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
bool flag;
Signal sign;
Patente car;
Message output;
CRandomMersenne * random;
Time minCrossingTime;
Time maxCrossingTime;
Time barrerTime;
Time leavingTime;
public:
	barrera_entrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
