//CPP:project/occupancy_tracker.cpp
#if !defined occupancy_tracker_h
#define occupancy_tracker_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common_controller.h"
#include <queue>



class occupancy_tracker: public Simulator { 
int n;
int max_capacity;
Time sigma;

std::queue<Action> response_queue;

InternalSignal signal_output;
double occupancy_output;

public:
	occupancy_tracker(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
