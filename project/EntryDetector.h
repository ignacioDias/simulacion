//CPP:project/EntryDetector.cpp
#if !defined entry_detector_h
#define entry_detector_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class EntryDetector: public Simulator { 
Time sigma;
Time latency;
VehicleId vehicleId;
VehicleEvent output;

public:
	EntryDetector(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
