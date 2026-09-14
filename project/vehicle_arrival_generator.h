//CPP:project/vehicle_arrival_generator.cpp
//CPP:/random/lib/randomc.cpp
#if !defined vehicle_arrival_generator_h
#define vehicle_arrival_generator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"
#include "random/lib/randomc.h"



class vehicle_arrival_generator: public Simulator { 
Time sigma;
double meanInterarrivalTime;
Time maxSimulationTime;
VehicleId nextVehicleId;
CRandomMersenne * random;
double output;


public:
	vehicle_arrival_generator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
