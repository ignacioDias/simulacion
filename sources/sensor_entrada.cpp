#include "sensor_entrada.h"
void sensor_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
latency = va_arg(parameters, Time);
printLog("[Sensor de entrada] Latencia: %.2f \n", latency);
sigma = INF;


}
double sensor_entrada::ta(double t) {
//This function returns a double.
return sigma;
}
void sensor_entrada::dint(double t) {
sigma = INF;


}
void sensor_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
pat = *(Patente*) x.value;
sigma = latency;

}
Event sensor_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
output.setPatente(pat);
output.setSignal(Signal::DetectarVehiculo);
// printLog("[%.2f] [Sensor de entrada] Auto %.0f detectado \n", t, pat);
return Event(&output, 0);
}
void sensor_entrada::exit() {
//Code executed at the end of the simulation.

}
