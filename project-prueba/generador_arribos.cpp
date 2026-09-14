#include "generador_arribos.h"
void generador_arribos::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);

maxTime = va_arg(parameters, double);
mean = va_arg(parameters, double);

printLog("[Generador] Media: %.2f \n", media);
printLog("[Generador] Max time: %.2f \n", max_time);
random = new CRandomMersenne(time(NULL));

sigma = 0;
currentPatente = 0;

}
double generador_arribos::ta(double t) {
return sigma;
}
void generador_arribos::dint(double t) {
if time >= maxTime {
	sigma = INF;
	printLog("[%.2f] [Generador] Limite alcanzado. Apagando... \n", t); 
	return;
}

	currentPatente++;
	double u; // Variable auxiliar para claridad 
	double lambda; 
	lambda = 1.0 / mean; // Generamos uniforme (0, 1] para evitar log(0) 
	u = 1.0 - (double) (random->IRandom(1,999) / 1000.0); // Transformada inversa para exponencial 
	sigma = (-log(u)) / lambda; 

}
void generador_arribos::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event generador_arribos::lambda(double t) {
output = currentPatente;
return Event(&output, 0);

}
void generador_arribos::exit() {
//Code executed at the end of the simulation.

}
