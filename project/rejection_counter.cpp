#include "rejection_counter.h"
void rejection_counter::init(double t,...) {
va_list parameters;
va_start(parameters,t);
rejections = 0;
sigma = INF;
}
double rejection_counter::ta(double t) {
return sigma;
}
void rejection_counter::dint(double t) {
sigma = INF;
}
void rejection_counter::dext(Event x, double t) {
VehicleEvent xv = *(VehicleEvent*)x.value;
rejectedVehicleId = xv.getVehicleId();
rejections++;
sigma = 0;
}
Event rejection_counter::lambda(double t) {
output = rejectedVehicleId; 
printLog("[%.2f] [Rejection counter] Vehicle rejection recorded\n", t);
return Event(&output, 0);

}
void rejection_counter::exit() {
printLog("[Rejection counter] Total rejections: %d\n", rejections);

}
