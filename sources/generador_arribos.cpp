#include "generador_arribos.h"
void generador_arribos::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
media =  va_arg(parameters,double);
max_time = va_arg(parameters, double);
printLog("[Generador] Media: %.2f \n", media);
printLog("[Generador] Max time: %.2f \n", max_time);
random = new CRandomMersenne(time(NULL));
sigma = 0;
pat = 0;

}
double generador_arribos::ta(double t) {
//This function returns a double.
return sigma;
}
void generador_arribos::dint(double t) {
pat++; 

if (t >= max_time) { 
	sigma = INF; // Detiene el generador 
	printLog("[%.2f] [Generador] Limite alcanzado. Apagando... \n", t); 
	return; 
}

// Si no llegamos al l?mite, calculamos el tiempo para el siguiente 
double u; // Variable auxiliar para claridad 
double lambda; 
lambda = 1.0 / media; // Generamos uniforme (0, 1] para evitar log(0) 
u = 1.0 - (double) (random->IRandom(1,999) / 1000.0); // Transformada inversa para exponencial 
sigma = (-log(u)) / lambda; 
printLog("[%.2f] [Generador] Proximo auto en %.2f seg (Total generados: %.0f) \n", t, sigma, pat);
}
void generador_arribos::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event generador_arribos::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
output = pat;
return Event(&output, 0);
}
void generador_arribos::exit() {
//Code executed at the end of the simulation.

}
