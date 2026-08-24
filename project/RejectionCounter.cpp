#include "RejectionCounter.h"
void RejectionCounter::init(double t,...) {
va_list parameters;
va_start(parameters,t);
rejections = 0;
sigma = INF;
}
double RejectionCounter::ta(double t) {
return sigma;
}
void RejectionCounter::dint(double t) {
sigma = INF;
}
void RejectionCounter::dext(Event x, double t) {
VehicleEvent xv = *(VehicleEvent*)x.value;
rejectedVehicleId = xv.getVehicleId();
rejections++;
sigma = 0;
}
Event RejectionCounter::lambda(double t) {
output = rejectedVehicleId; 
printLog("[%.2f] [Rejection counter] Vehicle rejection recorded\n", t);
return Event(&output, 0);

}
void RejectionCounter::exit() {
printLog("[Rejection counter] Total rejections: %d\n", rejections);

}
