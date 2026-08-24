#include "ExitCoordinator.h"
void ExitCoordinator::init(double t,...) {
va_list parameters;
va_start(parameters,t);
action = Idle;
sigma = INF;
}
double ExitCoordinator::ta(double t) {
return sigma;
}
void ExitCoordinator::dint(double t) {
if (action == AllowExit) {
	action = Idle;
	sigma = INF;
	return;
}

if (action == DecrementCapacity) {
	action = Idle;
	sigma = INF;
	return;
}
}
void ExitCoordinator::dext(Event x, double t) {

	VehicleEvent xv = *(VehicleEvent*) x.value;

	if (xv.getEventType() == EventType::ExitRequested) {
		vehicleId = xv.getVehicleId();
	action = AllowExit;
	sigma = 0;
}

	if (xv.getEventType() == EventType::VehicleExited) {
		vehicleId = xv.getVehicleId();
	action = DecrementCapacity;
	sigma = 0;
}
}
Event ExitCoordinator::lambda(double t) {
Event event = Event();

if (action == AllowExit) {
	to_exit_barrier_output_data = VehicleEvent(vehicleId, EventType::GrantExit);
	event = Event(&to_exit_barrier_output_data, EXIT_BARRIER_OUTPUT_PORT);
}
if (action == DecrementCapacity) {
	to_pcc_output_data = InternalSignal::Decrement;
	event = Event(&to_pcc_output_data, PCC_OUTPUT_PORT);
}

return event;
}
void ExitCoordinator::exit() {
}
