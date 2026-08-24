#include "EntryQueue.h"
void EntryQueue::init(double t,...) {
va_list parameters;
va_start(parameters,t);
sigma = INF;
pass = true;
}
double EntryQueue::ta(double t) {
return sigma;
}
void EntryQueue::dint(double t) {
vehicles.pop();
pass = false;
sigma = INF;
}
void EntryQueue::dext(Event x, double t) {
if (x.port == 1) {
	VehicleId vehicleId = *(VehicleId*) x.value;
	vehicles.push(vehicleId);
} 

if (x.port == 0){
	pass = true;
}


if (pass && !vehicles.empty()){
	sigma = 0;
} else {
	sigma = INF;
}


}
Event EntryQueue::lambda(double t) {
if (pass && !vehicles.empty()) {
output = vehicles.front();
}
return Event(&output, 0);



}
void EntryQueue::exit() {
}
