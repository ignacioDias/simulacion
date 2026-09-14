#include "registro.h"
void registro::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
sigma = INF;
cars.clear();
while(!output_queue.empty()) output_queue.pop();

}
double registro::ta(double t) {
//This function returns a double.
return sigma;
}
void registro::dint(double t) {
	// Ya se ejecuta la lambda para la tarea del frente, la sacamos.
	if (!output_queue.empty()) {
		output_queue.pop();
	}

	// Si quedan tareas pendientes, programamos una transicion inmediata.
	if (!output_queue.empty()) {
		sigma = 0;
	} else {
		sigma = INF;
	}
}
void registro::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

Message xValue = *(Message*) x.value;

if xValue.getSignal == (Signal::VehiculoIngresado) {
	cars[xValue.getPatente()] = t;
	sigma = INF;
} else if xValue.getSignal == (Signal::VehiculoHaSalido) {

}
}
Event registro::lambda(double t) {
	if (output_queue.empty()) return Event();

	RegisterOutputTask current = output_queue.front();

	// --- SALIDA 1: DATA BARRERA ENTRADA DISK (Puerto 0) ---
	if (current.type == BarreraEntrada) {
		out_patente_data = current.pat;
		return Event(&out_patente_data, 0);
	}

	// --- SALIDA 2: DATA BARRERA SALIDA DISK (Puerto 1) ---
	if (current.type == BarreraSalida) {
		out_patente_data = current.pat;
		return Event(&out_patente_data, 1);
	}

	// --- SALIDA 3: DATA AL AUDITOR (Puerto 2) ---
	if (current.type == Auditor) {
		out_auditor_data.first = current.pat;
		out_auditor_data.second = current.duration;
		return Event(&out_auditor_data, 2);
	}

	return Event();
}
void registro::exit() {
//Code executed at the end of the simulation.

}
