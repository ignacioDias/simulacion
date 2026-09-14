//FLAGS:-std=c++11
//CPP:sources/manejador_salida.cpp
#if !defined manejador_salida_h
#define manejador_salida_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common_controller.h"


class manejador_salida: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
enum Action {
	DecrementCapacity,
	AllowExit,
	Idle
};
// State
Action action;
Patente pat;

// Output
InternalSignal to_pcc_output_data; // port 0
Message to_exit_barrier_output_data; // port 1

const int PCC_OUTPUT_PORT = 0;
const int EXIT_BARRIER_OUTPUT_PORT = 1;





public:
	manejador_salida(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
