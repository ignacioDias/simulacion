#include "ExitDetector.h"
void ExitDetector::init(double t,...) {
va_list parameters;
va_start(parameters,t);
latency = va_arg(parameters, Time);
printLog("[Exit detector] Detection latency: %.2f\n", latency);
sigma = INF;


}
double ExitDetector::ta(double t) {
return sigma;
}
void ExitDetector::dint(double t) {
sigma = INF;


}
void ExitDetector::dext(Event x, double t) {
vehicleId = *(VehicleId*) x.value;
sigma = latency;

}
Event ExitDetector::lambda(double t) {
output.setVehicleId(vehicleId);
output.setEventType(EventType::ExitRequested);
return Event(&output,0);
}
void ExitDetector::exit() {
}
