#include "exit_detector.h"
void exit_detector::init(double t,...) {
va_list parameters;
va_start(parameters,t);
latency = va_arg(parameters, Time);
printLog("[Exit detector] Detection latency: %.2f\n", latency);
sigma = INF;


}
double exit_detector::ta(double t) {
return sigma;
}
void exit_detector::dint(double t) {
sigma = INF;


}
void exit_detector::dext(Event x, double t) {
vehicleId = *(VehicleId*) x.value;
sigma = latency;

}
Event exit_detector::lambda(double t) {
output.setVehicleId(vehicleId);
output.setEventType(EventType::ExitRequested);
return Event(&output,0);
}
void exit_detector::exit() {
}
