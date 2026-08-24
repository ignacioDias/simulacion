#include "ExitQueue.h"
void ExitQueue::init(double t,...) {
va_list parameters;
va_start(parameters,t);
sigma = INF;
pass = true;
}
double ExitQueue::ta(double t) {
return sigma;
}
void ExitQueue::dint(double t) {
pass = false;
sigma = INF;
vehicles.pop();



}
void ExitQueue::dext(Event x, double t) {
if (x.port == 0) {
	std::pair<VehicleId, Time> xv = *(std::pair<VehicleId, Time>*) x.value;
	vehicles.push(xv.first);
} 

if (x.port == 1) {
	pass = true;
}

if (pass && !vehicles.empty()) {
	sigma = 0;
} else {
	sigma = INF;
}


}
Event ExitQueue::lambda(double t) {
if (pass && !vehicles.empty()) {
output = vehicles.front();
}

return Event(&output, 0);



}
void ExitQueue::exit() {
}
