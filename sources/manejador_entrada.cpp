#include "manejador_entrada.h"
void manejador_entrada::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%) 
delay = va_arg(parameters, Time);
printLog("[Manejador de entrada] Delay: %.2f \n", delay);
action = Idle;
sigma = INF;
}
double manejador_entrada::ta(double t) {
return sigma;
}
void manejador_entrada::dint(double t) {
if (action == AskCapacity) {
	action = AwaitCapacity;
	sigma = INF;
	return;
}
    
if (action == AllowAccess) {
	action = Idle;
	sigma = INF;
	return;
}

if (action == DeclineAccess) {
	action = Idle;
	sigma = INF;
	return;
}
    
if (action == ReportAccess) {
	action = IncrementCapacity;
	sigma = 0;
	return;
}

if (action == IncrementCapacity) {
	action = Idle;
	sigma = INF;
	return;
}
}
void manejador_entrada::dext(Event x, double t) {
// Port 1: Mensajes de los sensores/barreras (DetectarVehiculo, VehiculoIngresado)
if (x.port == 1) {
	Message xv = *(Message*) x.value;

	// Caso 1: Llega un auto nuevo (del Generador/Sensor de entrada)
	if (xv.getSignal() == Signal::DetectarVehiculo) {
		pat = xv.getPatente(); // GUARDAMOS LA PATENTE EN EL ESTADO
		action = AskCapacity;
		sigma = 0;
		// printLog("[%.2f] [Manejador de entrada] Auto %.0f queriendo ingresar \n", t, pat);
	}
        
	// Caso 2: La barrera confirma que el auto ya cruzo
	if (xv.getSignal() == Signal::VehiculoIngresado) {
		pat = xv.getPatente(); // Actualizamos patente por seguridad
		action = ReportAccess;
		sigma = 0;
		printLog("[%.2f] [Manejador de entrada] Auto %.0f ingresado \n", t, pat);
	}
}

// Port 0: Respuesta del PCC (Capacity Counter)
if (x.port == 0) {
	InternalSignal xv = *(InternalSignal*) x.value;
        
	if (xv == InternalSignal::Full) {
		action = DeclineAccess;
		sigma = delay;
		// printLog("[%.2f] [Manejador de entrada] Estacionamiento lleno \n", t);
	}

	if (xv == InternalSignal::NotFull) {
		action = AllowAccess;
		sigma = delay;
		// printLog("[%.2f] [Manejador de entrada] Estacionamiento disponible (Autorizando a: %.0f) \n", t, pat);
	}
}
}
Event manejador_entrada::lambda(double t) {
Event event;

if (action == AskCapacity) {
	to_pcc_output_data = InternalSignal::AskIsFull;
	event = Event(&to_pcc_output_data, PCC_OUTPUT_PORT);
	// printLog("[%.2f] [Manejador de entrada] Preguntando capacidad... \n", t);
}
    
if (action == DeclineAccess) {
	to_input_barrier_output_data = Message(pat, Signal::DenegarEntrada);
	// printLog("[%.2f] [Manejador de entrada] Denegar acceso a la barrera del auto %.0f \n", t, pat);
	event = Event(&to_input_barrier_output_data, INPUT_BARRIER_OUTPUT_PORT);
}
    
if (action == AllowAccess) {
	to_input_barrier_output_data = Message(pat, Signal::PermitirEntrada);
	// printLog("[%.2f] [Manejador de entrada] Permitir acceso a la barrera del auto %.0f \n", t, pat);
	event = Event(&to_input_barrier_output_data, INPUT_BARRIER_OUTPUT_PORT);
}

// -----------------------------

if (action == ReportAccess) {
	to_parking_output_data = Message(pat, Signal::VehiculoIngresado);
	// printLog("[%.2f] [Manejador de entrada] Reportando acceso al parking del auto %.0f \n", t, pat);
	event = Event(&to_parking_output_data, PARKING_OUTPUT_PORT);
}

if (action == IncrementCapacity) {
	// printLog("[%.2f] [Manejador de entrada] Avisando al PCC que debe incrementar \n", t);
	to_pcc_output_data = InternalSignal::Increment;
	event = Event(&to_pcc_output_data, PCC_OUTPUT_PORT);
}

return event;
}
void manejador_entrada::exit() {
    //Code executed at the end of the simulation.
}
