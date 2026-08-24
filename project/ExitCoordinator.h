//CPP:project/ExitCoordinator.cpp
#if !defined exit_coordinator_h
#define exit_coordinator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class ExitCoordinator: public Simulator { 
Time sigma;
enum Action {
	DecrementCapacity,
	AllowExit,
	Idle
};
Action action;
VehicleId vehicleId;

InternalSignal to_pcc_output_data;
VehicleEvent to_exit_barrier_output_data;

const int PCC_OUTPUT_PORT = 0;
const int EXIT_BARRIER_OUTPUT_PORT = 1;





public:
	ExitCoordinator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
