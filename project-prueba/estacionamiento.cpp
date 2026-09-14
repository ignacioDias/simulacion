#include "estacionamiento.h"
void estacionamiento::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
minTimeParked = va_arg(parameters,Time);
maxTimeParked = va_arg(parameters,Time);
sigma = INF;
random = new CRandomMersenne(time(NULL)); 
}
double estacionamiento::ta(double t) {
//This function returns a double.
return sigma;

}
void estacionamiento::dint(double t) {
if(!cars.empty()) {
	cars.pop_front();
}

for(std::tuple<Patente,Time,Time>> &car : cars) {
	std::get<1>(car) = std::get<1>(car) - sigma;
}

if(cars.empty()) {
	sigma = INF;
} else {
	sigma = std::get<1>(cars.front());
}
}
void estacionamiento::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
	double u;
	double uniform;
	double time_car;
	std::tuple<Patente,Time,Time> car;

	Message xv = *(Message*) x.value;

if(xv.getSignal() == Signal::VehiculoIngresado) {
	u = (double) (random->IRandom(0,1000) / 1000.0);
	uniform = min_time_parked + (max_time_parked - min_time_parked) * u;
	time_car = uniform;
			std::get<0>(car) = xv.getPatente();
		std::get<1>(car) = time_car;
		std::get<2>(car) = time_car;

		for(std::tuple<Patente,Time,Time> &par : cars) {
			std::get<1>(par) = std::get<1>(par) - e;
		}
			auto it = cars.begin();
		while (it != cars.end() && std::get<1>(*it) < time_car) {
			it++;
		}
		
		cars.insert(it, car);

		sigma = std::get<1>(cars.front());
}
}
Event estacionamiento::lambda(double t) {
	if (!cars.empty()) {
		std::tuple<Patente,Time,Time> car = cars.front();
		output.first = std::get<0>(car); 
		output.second = std::get<2>(car);
		return Event(&output, 0);
	}
	return Event();
}
void estacionamiento::exit() {
//Code executed at the end of the simulation.

}
