#include "registro.h"
void registro::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);

	sigma = INF;
	cars.clear();
	while(!output_queue.empty()) output_queue.pop();
}
double registro::ta(double t) {
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
		Message xv = *(Message*)x.value;
	Patente pat = xv.getPatente(); 
	Signal message = xv.getSignal(); 

	if (message == Signal::VehiculoIngresado) { 
		cars[pat] = t;
		
		// Tarea: Mandar Patente a BarreraEntradaDisk (Puerto 0)
		RegisterOutputTask task;
		task.type = BarreraEntrada;
		task.pat = pat;
		output_queue.push(task);

		sigma = 0;
		// printLog("[%.2f] [Registro] Ingreso Auto %.0f. Encolando reporte entrada.\n", t, pat);
	} 
	else if (message == Signal::VehiculoHaSalido) {
		if (cars.count(pat)) {
			Time entry_time = cars[pat];
			Time duration = t - entry_time;
			cars.erase(pat);

			// Tarea A: Mandar Patente a BarreraSalidaDisk (Puerto 1)
			RegisterOutputTask task1;
			task1.type = BarreraSalida;
			task1.pat = pat;
			output_queue.push(task1);

			// Tarea B: Mandar Datos al Auditor (Puerto 2)
			RegisterOutputTask task2;
			task2.type = Auditor;
			task2.pat = pat;
			task2.duration = duration;
			output_queue.push(task2);

			sigma = 0; // Activar lambda para procesar la cola
			// printLog("[%.2f] [Registro] Salida Auto %.0f. Encolando 2 reportes (Disk y Auditor).\n", t, pat);
		} else {
			printLog("[%.2f] [Registro ERROR] El auto %.0f intenta salir sin haber entrado!\n", t, pat);
			sigma = INF; 
		}
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

}
