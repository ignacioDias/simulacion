//CPP:project/exit_queue.cpp
#if !defined exit_queue_h
#define exit_queue_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"
#include <queue>



class exit_queue: public Simulator { 
Time sigma;
std::queue<VehicleId> vehicles;
bool pass;
VehicleId output;



public:
	exit_queue(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
