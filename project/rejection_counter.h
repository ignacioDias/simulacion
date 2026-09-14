//CPP:project/rejection_counter.cpp
#if !defined rejection_counter_h
#define rejection_counter_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class rejection_counter: public Simulator { 
Time sigma;
VehicleId rejectedVehicleId;
int rejections;
VehicleId output;
public:
	rejection_counter(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
