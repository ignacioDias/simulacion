//CPP:sources/registro.cpp
#if !defined registro_h
#define registro_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "common.h"
#include "map"
#include "queue"


class registro: public Simulator { 
// Estado
Time sigma; 
std::map<Patente, Time> cars;
// Cola para manejar multiples salidas consecutivas
std::queue<RegisterOutputTask> output_queue;
// Variables bufer para pasar punteros en la lambda
std::pair<Patente, Time> out_auditor_data; 
double out_patente_data;

public:
	registro(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
