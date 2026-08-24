//CPP:project/EntryQueue.cpp
#if !defined entry_queue_h
#define entry_queue_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"
#include <queue>



class EntryQueue: public Simulator { 
Time sigma;
std::queue<VehicleId> vehicles;
bool pass;
VehicleId output;

public:
	EntryQueue(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
