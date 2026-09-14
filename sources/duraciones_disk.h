#if !defined duraciones_disk_h
#define duraciones_disk_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include <stdio.h>
#include <string.h>

// Incluimos el header del auditor para conocer la struct AuditorMsg
#include "auditor.h" 

class duraciones_disk: public Simulator { 
Time sigma;
long int FOutput;

public:
	duraciones_disk(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif