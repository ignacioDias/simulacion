#include "contador_capacidad.h"
void contador_capacidad::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);

	n = 0;
	max_capacity = (int)va_arg(parameters, double);
	while(!response_queue.empty()) response_queue.pop();
	sigma = INF;
}
double contador_capacidad::ta(double t) {
  return sigma;
}
void contador_capacidad::dint(double t) {
	// Si entramos aca, es porque ya ejecutamos la lambda para el evento que estaba al frente de la cola. Lo sacamos
	if (!response_queue.empty()) {
		response_queue.pop();
	}

	// Si TODAVIA quedan cosas en la cola, ponemos sigma=0 para atender el siguiente YA MISMO.
	if (!response_queue.empty()) {
		sigma = 0;
	} else {
		sigma = INF;
	}
}
void contador_capacidad::dext(Event x, double t) {
	InternalSignal xv = *(InternalSignal*)x.value;

	if (xv == InternalSignal::Increment) {
		n++;
		// printLog("[%.2f] [Contador] Auto entro. Ocupacion: %d/%d\n", t, n, max_capacity);
		response_queue.push(ReportOccupancy);
		sigma = 0;
	} 
	else if (xv == InternalSignal::Decrement) {
		if (n > 0) n--;
		// printLog("[%.2f] [Contador] Auto salio. Ocupacion: %d/%d\n", t, n, max_capacity);
		response_queue.push(ReportOccupancy);
		sigma = 0;
	}
	else if (xv == InternalSignal::AskIsFull) {
		response_queue.push(RespondToQuery);
		sigma = 0;
	}
}
Event contador_capacidad::lambda(double t) {
  if (response_queue.empty()) return Event();

  Action current_action = response_queue.front();

	if (current_action == RespondToQuery) {
		if (n >= max_capacity) {
			signal_output = InternalSignal::Full;
		} else {
			signal_output = InternalSignal::NotFull;
		}
		return Event(&signal_output, 0);
	}

	if (current_action == ReportOccupancy) {
		occupancy_output = (double)(n); 
		return Event(&occupancy_output, 1);
	}

  return Event();
}
void contador_capacidad::exit() {

}
