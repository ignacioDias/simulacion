#include "EntryDetector.h"
void EntryDetector::init(double t,...) {
va_list parameters;
va_start(parameters,t);
latency = va_arg(parameters, Time);
printLog("[Entry detector] Detection latency: %.2f\n", latency);
sigma = INF;


}
double EntryDetector::ta(double t) {
return sigma;
}
void EntryDetector::dint(double t) {
sigma = INF;


}
void EntryDetector::dext(Event x, double t) {
vehicleId = *(VehicleId*) x.value;
sigma = latency;

}
Event EntryDetector::lambda(double t) {
output.setVehicleId(vehicleId);
output.setEventType(EventType::VehicleDetected);
return Event(&output, 0);
}
void EntryDetector::exit() {
}
