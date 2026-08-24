//CPP:project/ParkingLot.cpp
#if !defined parking_lot_h
#define parking_lot_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"
#include "random/lib/randomc.h"



class ParkingLot: public Simulator { 
Time sigma;
Time maxParkingTime;
Time minParkingTime;
CRandomMersenne * random;
std::list<std::tuple<VehicleId,Time,Time>> vehicles;
std::pair<VehicleId, Time> output;

public:
	ParkingLot(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
