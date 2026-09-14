#include "auditor.h"
void auditor::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);
	
	sigma = INF;
	parking_data.clear();
}
double auditor::ta(double t) {
  return sigma;
}
void auditor::dint(double t) {
	sigma = INF;
}
void auditor::dext(Event x, double t) {
	std::pair<Patente, Time>* xv = (std::pair<Patente, Time>*)x.value;
	Patente pat = xv->first;
	Time time_value = xv->second; 

	// Info desde Parking
	if (x.port == 1) {
		parking_data[pat] = time_value;
	}
	// Info desde Registro 
	else if (x.port == 0) {
		if (parking_data.count(pat)) {
			Time parking_time = parking_data[pat]; 
			Time total_sys_time = time_value;      
			Time exit_delay = total_sys_time - parking_time;
			// Preparamos el paquete de datos para el Disk
			output_data.patente = pat;
			output_data.tiempo_asignado = parking_time;
			output_data.demora_salida = exit_delay;
			output_data.tiempo_simulacion = t;
			parking_data.erase(pat);
			sigma = 0;
		} else {
			printLog("[Auditor] Salida sin ingreso previo: %.0f\n", pat);
			sigma = INF;
		}
	}
}
Event auditor::lambda(double t) {
  return Event(&output_data, 0);
}
void auditor::exit() {

}
