//CPP:project/VehicleArrivalGenerator.cpp
//CPP:/random/lib/randomc.cpp
#if !defined vehicle_arrival_generator_h
#define vehicle_arrival_generator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"
#include "random/lib/randomc.h"



class VehicleArrivalGenerator: public Simulator { 
Time sigma;
double meanInterarrivalTime;
Time maxSimulationTime;
VehicleId nextVehicleId;
CRandomMersenne * random;
double output;


public:
	VehicleArrivalGenerator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
