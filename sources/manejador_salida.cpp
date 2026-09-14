#include "manejador_salida.h"
void manejador_salida::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
//
action = Idle;
sigma = INF;
}
double manejador_salida::ta(double t) {
//This function returns a double.
return sigma;
}
void manejador_salida::dint(double t) {
if (action == AllowExit) {
	action = Idle;
	sigma = INF;
	return;
}

if (action == DecrementCapacity) {
	action = Idle;
	sigma = INF;
	return;
}
}
void manejador_salida::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

Message xv = *(Message*) x.value;

if (xv.getSignal() == Signal::SolicitarSalida) {
	pat = xv.getPatente();
	action = AllowExit;
	sigma = 0;
	// printLog("[%.2f] [Manejador de salida] Auto %.0f solicitando salida \n", t, pat);
}

if (xv.getSignal() == Signal::VehiculoHaSalido) {
	pat = xv.getPatente();
	action = DecrementCapacity;
	sigma = 0;
	// printLog("[%.2f] [Manejador de salida] Avisando al contador que el auto %.0f se fue \n", t, pat);
}
}
Event manejador_salida::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
Event event = Event();

if (action == AllowExit) {
	to_exit_barrier_output_data = Message(pat, Signal::PermitirSalida);
	event = Event(&to_exit_barrier_output_data, EXIT_BARRIER_OUTPUT_PORT);
}
if (action == DecrementCapacity) {
	to_pcc_output_data = InternalSignal::Decrement;
	event = Event(&to_pcc_output_data, PCC_OUTPUT_PORT);
}

return event;
}
void manejador_salida::exit() {
//Code executed at the end of the simulation.

}
