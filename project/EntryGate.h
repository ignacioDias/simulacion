//CPP:project/EntryGate.cpp
#if !defined entry_gate_h
#define entry_gate_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"
#include "random/lib/randomc.h"



class EntryGate: public Simulator { 
Time sigma;
Time barrier_time;
Time minCrossingTime;
Time maxCrossingTime;
CRandomMersenne * random;
EventType request;
VehicleId vehicleId;
bool pass;
VehicleEvent output;

public:
	EntryGate(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
