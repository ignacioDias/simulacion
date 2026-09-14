#include "cola_entrada.h"
void cola_entrada::init(double t,...) {
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
double cola_entrada::ta(double t) {
//This function returns a double.
return sigma;
}
void cola_entrada::dint(double t) {
cars.pop();
pass = false;
sigma = INF;
}
void cola_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
if (x.port == 1) {
	Patente pat = *(Patente*) x.value;
	cars.push(pat);
	// printLog("[%.2f] [Cola de entrada] Auto %.0f encolado \n", t, pat);
} 

if (x.port == 0){
	pass = true;
	// printLog("[%.2f] [Cola de entrada] Puede pasar el siguiente! \n", t);
}


if (pass && !cars.empty()){ // Revisar si hace falta controlar el tiempo
	sigma = 0;
} else {
	sigma = INF;
}


}
Event cola_entrada::lambda(double t) {
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
void cola_entrada::exit() {
//Code executed at the end of the simulation.

}
