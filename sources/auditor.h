//CPP:sources/auditor.cpp
#if !defined auditor_h
#define auditor_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "common.h"
#include "map"


class auditor: public Simulator { 
// Estado
Time sigma; 
std::map<Patente, Time> parking_data; 

// Variable de salida
AuditorMsg output_data;

public:
	auditor(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
