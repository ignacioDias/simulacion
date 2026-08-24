#include "DwellTimeAuditor.h"
void DwellTimeAuditor::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);
	
	sigma = INF;
	parkingData.clear();
}
double DwellTimeAuditor::ta(double t) {
  return sigma;
}
void DwellTimeAuditor::dint(double t) {
	sigma = INF;
}

void DwellTimeAuditor::dext(Event x, double t) {
	std::pair<VehicleId, Time>* xv = (std::pair<VehicleId, Time>*)x.value;
	VehicleId vehicleId = xv->first;
	Time time_value = xv->second; 

	if (x.port == 1) {
		parkingData[vehicleId] = time_value;
	}
	else if (x.port == 0) {
		if (parkingData.count(vehicleId)) {
			Time parking_time = parkingData[vehicleId]; 
			Time total_sys_time = time_value;      
			Time exit_delay = total_sys_time - parking_time;
			outputData.vehicleId = vehicleId;
			outputData.assignedTime = parking_time;
			outputData.exitDelay = exit_delay;
			outputData.simulationTime = t;
			parkingData.erase(vehicleId);
			sigma = 0;
		} else {
			printLog("[Dwell time auditor] Exit recorded without a prior entry: %.0f\n", vehicleId);
			sigma = INF;
		}
	}
}
Event DwellTimeAuditor::lambda(double t) {
  return Event(&outputData, 0);
}
void DwellTimeAuditor::exit() {
}
