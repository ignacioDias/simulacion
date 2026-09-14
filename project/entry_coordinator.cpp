#include "entry_coordinator.h"
void entry_coordinator::init(double t,...) {
va_list parameters;
va_start(parameters,t);
delay = va_arg(parameters, Time);
 printLog("[Entry coordinator] Authorization delay: %.2f\n", delay);
action = Idle;
sigma = INF;
}
double entry_coordinator::ta(double t) {
return sigma;
}
void entry_coordinator::dint(double t) {
if (action == AskCapacity) {
	action = AwaitCapacity;
	sigma = INF;
	return;
}
    
if (action == AllowAccess) {
	action = Idle;
	sigma = INF;
	return;
}

if (action == DeclineAccess) {
	action = Idle;
	sigma = INF;
	return;
}
    
if (action == ReportAccess) {
	action = IncrementCapacity;
	sigma = 0;
	return;
}

if (action == IncrementCapacity) {
	action = Idle;
	sigma = INF;
	return;
}
}
void entry_coordinator::dext(Event x, double t) {
if (x.port == 1) {
	VehicleEvent xv = *(VehicleEvent*) x.value;

	if (xv.getEventType() == EventType::VehicleDetected) {
		vehicleId = xv.getVehicleId();
		action = AskCapacity;
		sigma = 0;
	}
        
	if (xv.getEventType() == EventType::VehicleAccepted) {
		vehicleId = xv.getVehicleId();
		action = ReportAccess;
		sigma = 0;
		printLog("[%.2f] [Entry coordinator] Vehicle %.0f entered the parking lot\n", t, vehicleId);
	}
}

if (x.port == 0) {
	InternalSignal signal = *(InternalSignal*) x.value;
        
	if (signal == InternalSignal::Full) {
		action = DeclineAccess;
		sigma = delay;
	}

	if (signal == InternalSignal::NotFull) {
		action = AllowAccess;
		sigma = delay;
	}
}
}
Event entry_coordinator::lambda(double t) {
Event event;

if (action == AskCapacity) {
	to_pcc_output_data = InternalSignal::AskIsFull;
	event = Event(&to_pcc_output_data, PCC_OUTPUT_PORT);
}
    
if (action == DeclineAccess) {
	to_input_barrier_output_data = VehicleEvent(vehicleId, EventType::DenyEntry);
	event = Event(&to_input_barrier_output_data, INPUT_BARRIER_OUTPUT_PORT);
}
    
if (action == AllowAccess) {
	to_input_barrier_output_data = VehicleEvent(vehicleId, EventType::GrantEntry);
	event = Event(&to_input_barrier_output_data, INPUT_BARRIER_OUTPUT_PORT);
}

if (action == ReportAccess) {
	to_parking_output_data = VehicleEvent(vehicleId, EventType::VehicleAccepted);
	event = Event(&to_parking_output_data, PARKING_OUTPUT_PORT);
}

if (action == IncrementCapacity) {
	to_pcc_output_data = InternalSignal::Increment;
	event = Event(&to_pcc_output_data, PCC_OUTPUT_PORT);
}

return event;
}
void entry_coordinator::exit() {
}
