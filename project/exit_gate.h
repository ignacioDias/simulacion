//CPP:project/exit_gate.cpp
#if !defined exit_gate_h
#define exit_gate_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"
#include "random/lib/randomc.h"



class exit_gate: public Simulator { 
Time sigma;
Time barrier_time;
Time minCrossingTime;
Time maxCrossingTime;
CRandomMersenne * random;
VehicleId vehicleId;
bool pass;
VehicleEvent output;

public:
	exit_gate(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
