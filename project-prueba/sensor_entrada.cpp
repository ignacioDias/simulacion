#include "sensor_entrada.h"
void sensor_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

latency = va_arg(parameters, double);
currentPatente = 0;
sigma = INF;
}
double sensor_entrada::ta(double t) {
return sigma;
}
void sensor_entrada::dint(double t) {
sigma = INF;
}
void sensor_entrada::dext(Event x, double t) {
currentPatente =  *(Patente*) x.value;
sigma = latency;
}
Event sensor_entrada::lambda(double t) {
output.setPatente(currentPatente);
output.setSignal(Signal::DetectarVehiculo);

return Event(&output, 0);
}
void sensor_entrada::exit() {
//Code executed at the end of the simulation.

}
