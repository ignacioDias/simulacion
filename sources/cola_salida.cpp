#include "cola_salida.h"
void cola_salida::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
sigma = INF;
pass = true;
}
double cola_salida::ta(double t) {
//This function returns a double.
return sigma;
}
void cola_salida::dint(double t) {
pass = false;
sigma = INF;
cars.pop();



}
void cola_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
if (x.port == 0) {
	std::pair<Patente, Time> xv = *(std::pair<Patente, Time>*) x.value;
	cars.push(xv.first);
	// printLog("[%.2f] [Cola de salida] Auto %.0f llega del parking, encolado \n", t, xv.first);
} 

if (x.port == 1) {
	pass = true;
	// printLog("[%.2f] [Cola de salida] Puede pasar el siguiente! \n", t);
}

if (pass && !cars.empty()) {
	sigma = 0;
} else {
	sigma = INF;
}


}
Event cola_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if (pass && !cars.empty()) {
	output = cars.front();
}

return Event(&output, 0);



}
void cola_salida::exit() {
//Code executed at the end of the simulation.

}
