//FLAGS:-std=c++11
//CPP:sources/manejador_entrada.cpp
#if !defined manejador_entrada_h
#define manejador_entrada_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "./common_controller.h"


class manejador_entrada: public Simulator { 
// Declare the state,
// output variables
// and parameters
Time sigma;
enum Action {
    AskCapacity,
    AwaitCapacity,
    IncrementCapacity,
    DeclineAccess,
    AllowAccess,
    ReportAccess,
    Idle
};
Time delay;
// State
Action action;
Patente pat;

InternalSignal to_pcc_output_data; // port 0
Message to_input_barrier_output_data; // port 1
Message to_parking_output_data; // port 2

const int PCC_OUTPUT_PORT = 0;
const int INPUT_BARRIER_OUTPUT_PORT = 1;
const int PARKING_OUTPUT_PORT = 2;
public:
	manejador_entrada(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
