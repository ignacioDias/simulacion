#include "parking_lot.h"
void parking_lot::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);

maxParkingTime = va_arg(parameters, Time);
printLog("[Parking lot] Maximum parking duration: %.2f\n", maxParkingTime);
minParkingTime = va_arg(parameters, Time);
printLog("[Parking lot] Minimum parking duration: %.2f\n", minParkingTime);

	sigma = INF;
	random = new CRandomMersenne(time(NULL)); 
}
double parking_lot::ta(double t) {
	return sigma;
}
void parking_lot::dint(double t) {
	if (!vehicles.empty()) {
		vehicles.pop_front();
	}

	for(std::tuple<VehicleId,Time,Time> &par : vehicles) {
		std::get<1>(par) = std::get<1>(par) - sigma;
	}

	if(vehicles.empty()) {
		sigma = INF;
	} else {
		sigma = std::get<1>(vehicles.front());
	}
}
void parking_lot::dext(Event x, double t) {
	double u;
	double uniform;
	double time_car;
	std::tuple<VehicleId,Time,Time> car;

	VehicleEvent xv = *(VehicleEvent*) x.value;

	if(xv.getEventType() == EventType::VehicleAccepted) {
		u = (double) (random->IRandom(0,1000) / 1000.0);
uniform = minParkingTime + (maxParkingTime - minParkingTime) * u;
		time_car = uniform;
		
		
		std::get<0>(car) = xv.getVehicleId();
		std::get<1>(car) = time_car;
		std::get<2>(car) = time_car;


		for(std::tuple<VehicleId,Time,Time> &par : vehicles) {
			std::get<1>(par) = std::get<1>(par) - e;
		}
		
		auto it = vehicles.begin();
		while (it != vehicles.end() && std::get<1>(*it) < time_car) {
			it++;
		}
		
		vehicles.insert(it, car);

		sigma = std::get<1>(vehicles.front());
	}
}
Event parking_lot::lambda(double t) {
	if (!vehicles.empty()) {
		std::tuple<VehicleId,Time,Time> car = vehicles.front();
		output.first = std::get<0>(car); 
		output.second = std::get<2>(car);
		return Event(&output, 0);
	}
	return Event();
}
void parking_lot::exit() {
}
