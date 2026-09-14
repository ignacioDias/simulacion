#include "vehicle_registry.h"
void vehicle_registry::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);

	sigma = INF;
	vehicles.clear();
	while(!outputQueue.empty()) outputQueue.pop();
}
double vehicle_registry::ta(double t) {
  return sigma;
}
void vehicle_registry::dint(double t) {
	// Remove the task just emitted by lambda().
	if (!outputQueue.empty()) {
		outputQueue.pop();
	}

	// Process pending tasks without additional delay.
	if (!outputQueue.empty()) {
		sigma = 0;
	} else {
		sigma = INF;
	}
}
void vehicle_registry::dext(Event x, double t) {
	VehicleEvent xv = *(VehicleEvent*)x.value;
	VehicleId vehicleId = xv.getVehicleId(); 
	EventType eventType = xv.getEventType(); 

	if (eventType == EventType::VehicleAccepted) { 
		vehicles[vehicleId] = t;
		
		RegistryOutputTask task;
		task.type = EntryGateRecord;
		task.vehicleId = vehicleId;
		outputQueue.push(task);

		sigma = 0;
	} 
	else if (eventType == EventType::VehicleExited) {
		if (vehicles.count(vehicleId)) {
			Time entry_time = vehicles[vehicleId];
			Time duration = t - entry_time;
			vehicles.erase(vehicleId);

			RegistryOutputTask task1;
			task1.type = ExitGateRecord;
			task1.vehicleId = vehicleId;
			outputQueue.push(task1);

			RegistryOutputTask task2;
			task2.type = DwellTimeRecord;
			task2.vehicleId = vehicleId;
			task2.duration = duration;
			outputQueue.push(task2);

			sigma = 0;
		} else {
			printLog("[%.2f] [Vehicle registry ERROR] Vehicle %.0f attempted to exit without a recorded entry\n", t, vehicleId);
			sigma = INF; 
		}
	}
}
Event vehicle_registry::lambda(double t) {
	if (outputQueue.empty()) return Event();

	RegistryOutputTask current = outputQueue.front();

	// Entry-gate record.
	if (current.type == EntryGateRecord) {
		outputVehicleId = current.vehicleId;
		return Event(&outputVehicleId, 0);
	}

	// Exit-gate record.
	if (current.type == ExitGateRecord) {
		outputVehicleId = current.vehicleId;
		return Event(&outputVehicleId, 1);
	}

	// Audit record.
	if (current.type == DwellTimeRecord) {
		auditOutput.first = current.vehicleId;
		auditOutput.second = current.duration;
		return Event(&auditOutput, 2);
	}

	return Event();
}
void vehicle_registry::exit() {
}
