#include "barrera_entrada.h"
void barrera_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
barrier_time = va_arg(parameters, Time);
printLog("[Barrera de entrada] Tiempo de barrera: %.2f \n", barrier_time);
min_time_cross = va_arg(parameters, Time);
printLog("[Barrera de entrada] Tiempo minimo para cruzar: %.2f \n", min_time_cross);
max_time_cross = va_arg(parameters, Time);
printLog("[Barrera de entrada] Tiempo maximo para cruzar: %.2f \n", max_time_cross);
sigma = INF;
random = new CRandomMersenne(time(NULL));
}
double barrera_entrada::ta(double t) {
//This function returns a double.
return sigma;
}
void barrera_entrada::dint(double t) {
sigma = INF;
pass = false;



}
void barrera_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
double u;
double uniform;
Message xv = *(Message*) x.value;
request = xv.getSignal();

if (request == Signal::PermitirEntrada) {
	pass = true;
	pat = xv.getPatente();
	u = (double) (random->IRandom(0,1000) / 1000.0);
	uniform = min_time_cross + (max_time_cross - min_time_cross) * u;
	sigma = barrier_time + uniform + barrier_time;
  	// printLog("[%.2f] [Barrera de entrada] Permitiendo entrada del auto %.0f \n", t, pat);
} else if (request == Signal::DenegarEntrada){
	pass = false;
	pat = xv.getPatente();
	sigma = 2;
	// printLog("[%.2f] [Barrera de entrada] Denegando entrada del auto %.0f \n", t, pat);
} else {
	sigma = INF;
}
}
Event barrera_entrada::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
output.setPatente(pat);
output.setSignal(request);

if (pass) {
	// printLog("[%.2f] [Barrera de entrada] El auto %.0f ingresa al parking \n", t, pat);
	output.setPatente(pat);
	output.setSignal(Signal::VehiculoIngresado);
	return Event(&output, 1);
} else {
	// printLog("[%.2f] [Barrera de entrada] El auto %.0f se retira sin ingresar \n", t, pat);
	return Event(&output, 0);
}

}
void barrera_entrada::exit() {
//Code executed at the end of the simulation.

}
