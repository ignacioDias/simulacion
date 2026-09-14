#include "contador_rechazos.h"
void contador_rechazos::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
rejections = 0;
sigma = INF;
}
double contador_rechazos::ta(double t) {
//This function returns a double.
return sigma;
}
void contador_rechazos::dint(double t) {
sigma = INF;
}
void contador_rechazos::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
Message xv = *(Message*)x.value;
rejected_pat = xv.getPatente();
rejections++;
sigma = 0;
}
Event contador_rechazos::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
output = rejected_pat; 
printLog("[%.2f] [Contador de rechazos] Ocurrio un rechazo \n", t);
return Event(&output, 0);

}
void contador_rechazos::exit() {
//Code executed at the end of the simulation.
printLog("[Contador de rechazos] Cantidad de rechazos: %d \n", rejections);

}
