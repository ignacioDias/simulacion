#include "OccupancyTracker.h"
void OccupancyTracker::init(double t,...) {
	va_list parameters;
	va_start(parameters,t);

	n = 0;
	max_capacity = (int)va_arg(parameters, double);
	while(!response_queue.empty()) response_queue.pop();
	sigma = INF;
}
double OccupancyTracker::ta(double t) {
  return sigma;
}
void OccupancyTracker::dint(double t) {
	// Remove the response emitted by lambda().
	if (!response_queue.empty()) {
		response_queue.pop();
	}

	// Process queued responses without additional delay.
	if (!response_queue.empty()) {
		sigma = 0;
	} else {
		sigma = INF;
	}
}
void OccupancyTracker::dext(Event x, double t) {
	InternalSignal signal = *(InternalSignal*)x.value;

	if (signal == InternalSignal::Increment) {
		n++;
		response_queue.push(ReportOccupancy);
		sigma = 0;
	} 
	else if (signal == InternalSignal::Decrement) {
		if (n > 0) n--;
		response_queue.push(ReportOccupancy);
		sigma = 0;
	}
	else if (signal == InternalSignal::AskIsFull) {
		response_queue.push(RespondToQuery);
		sigma = 0;
	}
}
Event OccupancyTracker::lambda(double t) {
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
void OccupancyTracker::exit() {
}
