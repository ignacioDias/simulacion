//CPP:project/dwell_time_auditor.cpp
#if !defined dwell_time_auditor_h
#define dwell_time_auditor_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class dwell_time_auditor: public Simulator { 
Time sigma; 
std::map<VehicleId, Time> parkingData;

DwellTimeReport outputData;

public:
	dwell_time_auditor(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
