#include "EntryGate.h"
void EntryGate::init(double t,...) {
va_list parameters;
va_start(parameters,t);
barrier_time = va_arg(parameters, Time);
printLog("[Entry gate] Barrier duration: %.2f\n", barrier_time);
minCrossingTime = va_arg(parameters, Time);
printLog("[Entry gate] Minimum crossing duration: %.2f\n", minCrossingTime);
maxCrossingTime = va_arg(parameters, Time);
printLog("[Entry gate] Maximum crossing duration: %.2f\n", maxCrossingTime);
sigma = INF;
random = new CRandomMersenne(time(NULL));
}
double EntryGate::ta(double t) {
return sigma;
}
void EntryGate::dint(double t) {
sigma = INF;
pass = false;



}
void EntryGate::dext(Event x, double t) {
double u;
double uniform;
VehicleEvent xv = *(VehicleEvent*) x.value;
request = xv.getEventType();

if (request == EventType::GrantEntry) {
	pass = true;
	vehicleId = xv.getVehicleId();
	u = (double) (random->IRandom(0,1000) / 1000.0);
uniform = minCrossingTime + (maxCrossingTime - minCrossingTime) * u;
	sigma = barrier_time + uniform + barrier_time;
} else if (request == EventType::DenyEntry){
	pass = false;
	vehicleId = xv.getVehicleId();
	sigma = 2;
} else {
	sigma = INF;
}
}
Event EntryGate::lambda(double t) {
output.setVehicleId(vehicleId);
output.setEventType(request);

if (pass) {
	output.setVehicleId(vehicleId);
	output.setEventType(EventType::VehicleAccepted);
	return Event(&output, 1);
} else {
	return Event(&output, 0);
}

}
void EntryGate::exit() {
}
