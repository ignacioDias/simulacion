#include "entry_queue.h"
void entry_queue::init(double t,...) {
va_list parameters;
va_start(parameters,t);
sigma = INF;
pass = true;
}
double entry_queue::ta(double t) {
return sigma;
}
void entry_queue::dint(double t) {
vehicles.pop();
pass = false;
sigma = INF;
}
void entry_queue::dext(Event x, double t) {
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
Event entry_queue::lambda(double t) {
if (pass && !vehicles.empty()) {
output = vehicles.front();
}
return Event(&output, 0);



}
void entry_queue::exit() {
}
