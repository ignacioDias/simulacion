#include "barrera_salida.h"
void barrera_salida::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
sigma = INF;
maxCrossingTime = va_arg(parameters, Time);
minCrossingTime = va_arg(parameters, Time);
barrierTime = va_arg(parameters, Time);
random = new CRandomMersenne(time(NULL));
}
double barrera_salida::ta(double t) {
//This function returns a double.
return sigma;

}
void barrera_salida::dint(double t) {
sigma = INF;
}
void barrera_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
if x.value.getSignal() == (Signal::PermitirSalida) {
	pat = x.value.getPatente();
	u = (double) (random->IRandom(0,1000) / 1000.0);
	uniform = min_time_cross + (max_time_cross - min_time_cross) * u;
	// printLog("[%.2f] [Barrera de salida] Abriendo barrera de salida para el auto %.0f \n", t, pat);
	sigma = barrier_time + uniform + barrier_time;	
} else {
	sigma = INF;
}
}
Event barrera_salida::lambda(double t) {
output.setPatente(pat);
output.setSignal(Signal::VehiculoHaSalido);
// printLog("[%.2f] [Barrera de salida] El auto %.0f se fue! \n", t, pat);
return Event(&output, 0);
 
}
void barrera_salida::exit() {
//Code executed at the end of the simulation.

}
