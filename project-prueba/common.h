#pragma once

#include "common_controller.h"
#include <limits>
#include <list>
#include <queue>
#include <string>
#include <tuple>
#include <utility>

const double INF = std::numeric_limits<double>::infinity();

typedef double Patente;

enum class Signal {
    DetectarVehiculo,   // "detectarVehiculo"
    VehiculoIngresado,  // "vehiculoIngresado"
    VehiculoHaSalido,   // "vehiculoHaSalido"
    SolicitarSalida,    // "solicitarSalida"
    PermitirEntrada,    // "permitirEntrada"
    PermitirSalida,     // "permitirSalida"
    DenegarEntrada,     // "denegarEntrada"
    RechazarEntrada     // "rechazarEntrada"
};

struct AuditorMsg {
    Patente patente;
    Time tiempo_asignado;   // Cuanto debia estar
    Time demora_salida;     // Cuanto tardo en salir
    Time tiempo_simulacion; // t
};

enum RegisterTaskType {
    BarreraEntrada,     // Puerto 0
    BarreraSalida,      // Puerto 1
    Auditor             // Puerto 2
};

// Estructura para guardar en la cola lo que hay que enviar
struct RegisterOutputTask {
    RegisterTaskType type;
    Patente pat;
    Time duration; // Solo se usa para el Auditor
};

inline const char* signalToString(Signal s) {
    switch(s) {
        case Signal::DetectarVehiculo: return "detectarVehiculo";
        case Signal::VehiculoIngresado: return "vehiculoIngresado";
        case Signal::VehiculoHaSalido: return "vehiculoHaSalido";
        case Signal::SolicitarSalida: return "solicitarSalida";
        case Signal::PermitirEntrada: return "permitirEntrada";
        case Signal::PermitirSalida: return "permitirSalida";
        case Signal::DenegarEntrada: return "denegarEntrada";
        case Signal::RechazarEntrada: return "rechazarEntrada";
        default: return "unknown";
    }
}



class Message {
    Patente patente;
    Signal signal;
    
public:
    // Constructors
    Message() : patente(0), signal(Signal::DetectarVehiculo) {}
    Message(Patente p, Signal s) : patente(p), signal(s) {}
    
    // Getters
    Patente getPatente() const { return patente; }
    Signal getSignal() const { return signal; }
    
    // Setters
    void setPatente(Patente p) { patente = p; }
    void setSignal(Signal s) { signal = s; }
    
    // to_string
    std::string to_string() const {
        return "Message(" + std::to_string(patente) + ", " + signalToString(signal) + ")";
    }
};
