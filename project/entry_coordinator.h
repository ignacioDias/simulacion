//CPP:project/entry_coordinator.cpp
#if !defined entry_coordinator_h
#define entry_coordinator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "common.h"



class entry_coordinator: public Simulator { 
Time sigma;
enum Action {
    AskCapacity,
    AwaitCapacity,
    IncrementCapacity,
    DeclineAccess,
    AllowAccess,
    ReportAccess,
    Idle
};
Time delay;
Action action;
VehicleId vehicleId;

InternalSignal to_pcc_output_data;
VehicleEvent to_input_barrier_output_data;
VehicleEvent to_parking_output_data;

const int PCC_OUTPUT_PORT = 0;
const int INPUT_BARRIER_OUTPUT_PORT = 1;
const int PARKING_OUTPUT_PORT = 2;
public:
	entry_coordinator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
