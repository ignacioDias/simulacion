//CPP:project/vehicle_registry.cpp
#if !defined vehicle_registry_h
#define vehicle_registry_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class vehicle_registry: public Simulator { 
Time sigma;
std::map<VehicleId, Time> vehicles;
std::queue<RegistryOutputTask> outputQueue;
std::pair<VehicleId, Time> auditOutput;
double outputVehicleId;

public:
	vehicle_registry(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
