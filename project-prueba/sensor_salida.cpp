#include "sensor_salida.h"
void sensor_salida::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
latency = va_arg(parameters, Time);
sigma = INF;

}
double sensor_salida::ta(double t) {
//This function returns a double.
return sigma;
}
void sensor_salida::dint(double t) {
sigma = INF;
}
void sensor_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
sigma = latency;
patente = *(Patente*) x.value
}
Event sensor_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)

output.setPatente(patente);
output.setSignal(Signal::SolicitarSalida);
return Event(&output, 0);
}
void sensor_salida::exit() {
//Code executed at the end of the simulation.

}
