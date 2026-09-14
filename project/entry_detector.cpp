#include "entry_detector.h"
void entry_detector::init(double t,...) {
va_list parameters;
va_start(parameters,t);
latency = va_arg(parameters, Time);
printLog("[Entry detector] Detection latency: %.2f\n", latency);
sigma = INF;


}
double entry_detector::ta(double t) {
return sigma;
}
void entry_detector::dint(double t) {
sigma = INF;


}
void entry_detector::dext(Event x, double t) {
vehicleId = *(VehicleId*) x.value;
sigma = latency;

}
Event entry_detector::lambda(double t) {
output.setVehicleId(vehicleId);
output.setEventType(EventType::VehicleDetected);
return Event(&output, 0);
}
void entry_detector::exit() {
}
