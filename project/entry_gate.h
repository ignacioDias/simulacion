//CPP:project/entry_gate.cpp
#if !defined entry_gate_h
#define entry_gate_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "common.h"
#include "random/lib/randomc.h"


class entry_gate: public Simulator { 
Time sigma;
Time barrier_time;
Time minCrossingTime;
Time leavingTime;
Time maxCrossingTime;
CRandomMersenne * random;
EventType request;
VehicleId vehicleId;
bool pass;
VehicleEvent output;

public:
	entry_gate(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
