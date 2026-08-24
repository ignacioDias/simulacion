#include "ExitGate.h"
void ExitGate::init(double t,...) {
va_list parameters;
va_start(parameters,t);
barrier_time = va_arg(parameters, Time);
printLog("[Exit gate] Barrier duration: %.2f\n", barrier_time);
minCrossingTime = va_arg(parameters, Time);
printLog("[Exit gate] Minimum crossing duration: %.2f\n", minCrossingTime);
maxCrossingTime = va_arg(parameters, Time);
printLog("[Exit gate] Maximum crossing duration: %.2f\n", maxCrossingTime);
sigma = INF;
random = new CRandomMersenne(time(NULL));
}
double ExitGate::ta(double t) {
return sigma;
}
void ExitGate::dint(double t) {
sigma = INF;




}
void ExitGate::dext(Event x, double t) {
double u;
double uniform;
VehicleEvent xv = *(VehicleEvent*) x.value;

if (xv.getEventType() == EventType::GrantExit) {
	vehicleId = xv.getVehicleId();
	u = (double) (random->IRandom(0,1000) / 1000.0);
uniform = minCrossingTime + (maxCrossingTime - minCrossingTime) * u;
	sigma = barrier_time + uniform + barrier_time;
} else {
	sigma = INF;	
}

}
Event ExitGate::lambda(double t) {
output.setVehicleId(vehicleId);
output.setEventType(EventType::VehicleExited);
return Event(&output, 0);
 
}
void ExitGate::exit() {
}
