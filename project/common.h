#pragma once

#include "common_controller.h"
#include <limits>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <utility>

const double INF = std::numeric_limits<double>::infinity();

typedef double VehicleId;

enum class EventType {
    VehicleDetected,
    VehicleAccepted,
    VehicleExited,
    ExitRequested,
    GrantEntry,
    GrantExit,
    DenyEntry
};

struct DwellTimeReport {
    VehicleId vehicleId;
    Time assignedTime;
    Time exitDelay;
    Time simulationTime;
};

enum RegistryOutputType {
    EntryGateRecord,
    ExitGateRecord,
    DwellTimeRecord
};

struct RegistryOutputTask {
    RegistryOutputType type;
    VehicleId vehicleId;
    Time duration;
};

inline const char* eventTypeToString(EventType s) {
    switch(s) {
        case EventType::VehicleDetected: return "vehicleDetected";
        case EventType::VehicleAccepted: return "vehicleAccepted";
        case EventType::VehicleExited: return "vehicleExited";
        case EventType::ExitRequested: return "exitRequested";
        case EventType::GrantEntry: return "grantEntry";
        case EventType::GrantExit: return "grantExit";
        case EventType::DenyEntry: return "denyEntry";
        default: return "unknown";
    }
}



class VehicleEvent {
    VehicleId vehicleId;
    EventType eventType;
    
public:
    VehicleEvent() : vehicleId(0), eventType(EventType::VehicleDetected) {}
    VehicleEvent(VehicleId id, EventType type) : vehicleId(id), eventType(type) {}

    VehicleId getVehicleId() const { return vehicleId; }
    EventType getEventType() const { return eventType; }

    void setVehicleId(VehicleId id) { vehicleId = id; }
    void setEventType(EventType type) { eventType = type; }

    std::string to_string() const {
        return "VehicleEvent(" + std::to_string(vehicleId) + ", " + eventTypeToString(eventType) + ")";
    }
};
