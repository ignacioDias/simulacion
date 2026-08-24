#include "VehicleArrivalGenerator.h"
#include <cmath>
void VehicleArrivalGenerator::init(double t,...) {
va_list parameters;
va_start(parameters,t);
meanInterarrivalTime =  va_arg(parameters,double);
maxSimulationTime = va_arg(parameters, double);
printLog("[Arrival generator] Mean interarrival time: %.2f\n", meanInterarrivalTime);
printLog("[Arrival generator] Simulation limit: %.2f\n", maxSimulationTime);
random = new CRandomMersenne(time(NULL));
sigma = 0;
nextVehicleId = 0;

}
double VehicleArrivalGenerator::ta(double t) {
return sigma;
}
void VehicleArrivalGenerator::dint(double t) {
nextVehicleId++; 

if (t >= maxSimulationTime) { 
	sigma = INF;
	printLog("[%.2f] [Arrival generator] Simulation limit reached; stopping\n", t);
	return; 
}

// Draw the next interarrival time using the inverse exponential distribution.
double u;
double lambda;
lambda = 1.0 / meanInterarrivalTime;
u = 1.0 - (double) (random->IRandom(1,999) / 1000.0);
sigma = (-log(u)) / lambda; 
printLog("[%.2f] [Arrival generator] Next vehicle in %.2f s (total generated: %.0f)\n", t, sigma, nextVehicleId);
}
void VehicleArrivalGenerator::dext(Event x, double t) {

}
Event VehicleArrivalGenerator::lambda(double t) {
output = nextVehicleId;
return Event(&output, 0);
}
void VehicleArrivalGenerator::exit() {
}
