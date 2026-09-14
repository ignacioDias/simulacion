#include "duraciones_disk.h"

void duraciones_disk::init(double t,...) {
    va_list parameters;
    va_start(parameters,t);

    char* FName = va_arg(parameters, char*);
    FOutput = PDFileOpen(FName, 'w');
    
    // Header opcional
    char header[] = "Patente,TiempoEstacionado,DemoraSalida,TiempoSimulacion\n";
    PDFileWrite(FOutput, header, strlen(header));
    
    sigma = INF;
}

double duraciones_disk::ta(double t) {
    return sigma;
}

void duraciones_disk::dint(double t) {
    sigma = INF;
}

void duraciones_disk::dext(Event x, double t) {
    // 1. Recibimos el puntero y lo casteamos a nuestra estructura
    AuditorMsg* data = (AuditorMsg*)x.value;
    
    // 2. Formateamos la linea
    char buf[1024];
    sprintf(buf, "%.0f, %.2f, %.2f, %.2f\n", 
            data->patente, 
            data->tiempo_asignado, 
            data->demora_salida, 
            data->tiempo_simulacion);

    // 3. Escribimos
    PDFileWrite(FOutput, buf, strlen(buf));
    
    sigma = INF;
}

Event duraciones_disk::lambda(double t) {
    return Event();
}

void duraciones_disk::exit() {
    PDFileClose(FOutput);
}