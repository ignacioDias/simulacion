//CPP:project/ExitDetector.cpp
#if !defined exit_detector_h
#define exit_detector_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class ExitDetector: public Simulator { 
Time sigma;
Time latency;
VehicleId vehicleId;
VehicleEvent output;

public:
	ExitDetector(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
