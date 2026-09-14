//CPP:project/exit_detector.cpp
#if !defined exit_detector_h
#define exit_detector_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class exit_detector: public Simulator { 
Time sigma;
Time latency;
VehicleId vehicleId;
VehicleEvent output;

public:
	exit_detector(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
