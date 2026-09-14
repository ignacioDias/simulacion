#include "barrera_entrada.h"
void barrera_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
minCrossingTime = va_arg(parameters, Time);
maxCrossingTime = va_arg(parameters, Time);
barrerTime = va_arg(parameters, Time);
leavingTime = va_arg(parameters, Time);

sigma = INF;
random = new CRandomMersenne(time(NULL));
}
double barrera_entrada::ta(double t) {
//This function returns a double.
return sigma;

}
void barrera_entrada::dint(double t) {
flag = false;
sigma = INF;
}
void barrera_entrada::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

double u;
double uniform;
Message xValue = *(Message*) x.value;
request = xValue.getSignal();

if(request == Signal::PermitirEntrada) {
	sign = request;
	car = xValue.getPatente();
	flag = true;
	u = (double) (random->IRandom(0,1000) / 1000.0);
	uniform = minCrossingTime + (maxCrossingTime - minCrossingTime) * u;
	sigma = barrier_time + uniform + barrier_time;
} else if(request == Signal::DenegarEntrada) {
	sigma = leavingTime;
	flag = false;
	car = xValue.getPatente();
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
int port;
output.setPatente(car);
if flag {
	output.setSignal(Signal::VehiculoIngresado);
	port = 1;
} else {
	output.setSignal(sign);
	port = 0;
}
return Event(&output, port);
}
void barrera_entrada::exit() {
//Code executed at the end of the simulation.

}
