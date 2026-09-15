# Informe del Proyecto: Sistema de Estacionamiento Inteligente

**Sistema modelado:** Simulación DEVS de un estacionamiento con control de
ingreso, control de egreso, barreras, seguimiento de ocupación y generación de
métricas.

**Herramienta:** PowerDEVS · Runtime en C++ · Análisis en Python.

---

## 1. Resumen ejecutivo

El proyecto modela un estacionamiento de **30 lugares** mediante simulación de
eventos discretos (DEVS). Los vehículos llegan según una tasa exponencial,
ingresan por una barrera controlada, permanecen un tiempo aleatorio y egresan
por otra barrera. Un sistema de control de acceso consulta la ocupación actual
antes de permitir o rechazar cada ingreso. Se registran métricas de ocupación,
rechazos, ingresos, egresos y duración de estadía, y un analizador Python
calcula indicadores y genera gráficos.

---

## 2. Especificación

### 2.1 Parámetros de simulación

| Parámetro | Valor | Modelo |
|---|---|---|
| Tiempo medio entre arribos | 40 s | `vehicle_arrival_generator` |
| Duración máxima de simulación | 43 200 s (12 h) | `vehicle_arrival_generator` |
| Capacidad máxima | 30 vehículos | `occupancy_tracker` |
| Demora de autorización | 3 s | `entry_coordinator` |
| Latencia de detección | 1 s | `entry_detector` / `exit_detector` |
| Tiempo de barrera | 4 s | `entry_gate` / `exit_gate` |
| Tiempo de cruce (mín–máx) | 1 – 3 s | `entry_gate` / `exit_gate` |
| Estadía en estacionamiento (mín–máx) | 120 – 300 s | `parking_lot` |

### 2.2 Modelos atómicos

| Archivo | Rol |
|---|---|
| `vehicle_arrival_generator` | Genera arribos con intervalos exponenciales (media 40 s) hasta el tiempo límite. Salida: `VehicleId`. |
| `entry_queue` | Cola de espera de entrada. Recibe vehículos (puerto 1) y una señal de "paso" de la barrera (puerto 0); libera al vehículo frontal. |
| `entry_detector` | Detecta el vehículo con latencia de 1 s y emite `VehicleEvent(VehicleDetected)`. |
| `entry_coordinator` | Control de acceso: consulta capacidad, decide permitir/denegar y, al confirmar el ingreso, informa al estacionamiento y actualiza la ocupación. |
| `occupancy_tracker` | Contador de ocupación. Responde `Full`/`NotFull` a consultas y procesa incrementos/decrementos. |
| `entry_gate` | Barrera de entrada: cruce con duración aleatoria; niega el paso o confirma el ingreso. |
| `parking_lot` | Estacionamiento: asigna estadía aleatoria (120–300 s) y emite la salida del vehículo. |
| `exit_detector` | Detecta el vehículo que intenta salir con latencia de 1 s y emite `VehicleEvent(ExitRequested)`. |
| `exit_coordinator` | Control de salida: autoriza el egreso y decrementa la ocupación. |
| `exit_gate` | Barrera de salida: cruce con duración aleatoria y confirmación del egreso. |
| `exit_queue` | Cola de espera de salida. Recibe vehículos desde el estacionamiento (puerto 0) y señal de "paso" de la barrera (puerto 1). |
| `rejection_counter` | Cuenta los ingresos denegados. |
| `vehicle_registry` | Registra ingresos/egresos y calcula la permanencia de cada vehículo. |
| `dwell_time_auditor` | Consolida la información de permanencia y emite el reporte final por vehículo. |

### 2.3 Tipos de datos compartidos

Definidos en `common.h` y `common_controller.h`:

- `VehicleId` (`double`): identificador del vehículo.
- `enum class EventType`: `VehicleDetected`, `VehicleAccepted`, `VehicleExited`, `ExitRequested`, `GrantEntry`, `GrantExit`, `DenyEntry`.
- `VehicleEvent`: portador del vehículo + tipo de evento.
- `enum class InternalSignal`: `Increment`, `Decrement`, `AskIsFull`, `NotFull`, `Full`.
- `DwellTimeReport`, `RegistryOutputTask`, `RegistryOutputType`.

### 2.4 Arquitectura y conexiones (topología)

El modelo raíz (`SmartParkingSystem`) está formado por 8 bloques:

```
vehicle_arrival_generator ──► entry_detection_subsystem ──► access_controller ──► entry_gate ──► parking_lot ──► exit_detection_subsystem ──► access_controller ──► exit_gate
```

**Subsistema `entry_detection_subsystem`** (cola + detector de ingreso)

```
in(Gate)       → entry_queue.in0   (señal "paso" cuando la barrera acepta)
in(Generator)  → entry_queue.in1   (arrobo de nuevo vehículo)
entry_queue.out0    → entry_detector.in0
entry_detector.out0 → out(AccessController)   (VehicleEvent VehicleDetected)
```

**Subsistema `exit_detection_subsystem`** (cola + detector de egreso)

```
in(ParkingLot) → exit_queue.in0   (vehículo que deja el estacionamiento)
in(ExitGate)   → exit_queue.in1   (señal "paso" cuando la barrera confirma el cruce)
exit_queue.out0   → exit_detector.in0
exit_detector.out0 → out(AccessController)   (VehicleEvent ExitRequested)
```

**Subsistema `access_controller`** (coordinador de ingreso, ocupación, coordinador de egreso)

```
in(Inputs)  → entry_coordinator.in1   (detectado / aceptado)
in(Outputs) → exit_coordinator.in0    (salida solicitada / egreso confirmado)
entry_coordinator.out0 → occupancy_tracker.in0  (AskIsFull / Increment)
entry_coordinator.out1 → out(EntryGate)   (GrantEntry / DenyEntry)  → entry_gate.in0
entry_coordinator.out2 → out(ParkingLot)  (VehicleAccepted)          → parking_lot.in0
occupancy_tracker.out0 → entry_coordinator.in0  (Full / NotFull)
occupancy_tracker.out1 → to_disk "occupancy.csv"
exit_coordinator.out0  → occupancy_tracker.in1  (Decrement)
exit_coordinator.out1  → out(ExitGate)   (GrantExit) → exit_gate.in0
```

**Subsistema `metrics_monitor`** (contadores y registros)

```
in(Rejections)       → rejection_counter.in0    (VehicleEvent DenyEntry)
in(VehicleEvents)    → vehicle_registry.in0     (VehicleAccepted / VehicleExited)
in(ParkingDurations) → dwell_time_auditor.in0   (pair <VehicleId, Time>)
rejection_counter.out0   → to_disk "rejection.csv"
vehicle_registry.out0    → to_disk "entrygate.csv"
vehicle_registry.out1    → to_disk "exitgate.csv"
vehicle_registry.out2    → dwell_time_auditor.in1
dwell_time_auditor.out0  → to_disk "dewlltime.csv"
```

**Conexiones a nivel raíz**

```
vehicle_arrival_generator.out0 → entry_detection_subsystem.in(Generator)
entry_detection_subsystem.out  → access_controller.in(Inputs)
entry_gate.out1                → entry_detection_subsystem.in(Gate)   (libera la cola)
entry_gate.out1                → access_controller.in(Inputs)         (reporta ingreso aceptado)
entry_gate.out1                → metrics_monitor.in(VehicleEvents)
entry_gate.out0                → metrics_monitor.in(Rejections)       (ingreso denegado)
access_controller.out(EntryGate)  → entry_gate.in0
access_controller.out(ParkingLot) → parking_lot.in0
access_controller.out(ExitGate)   → exit_gate.in0
parking_lot.out0                 → exit_detection_subsystem.in(ParkingLot)
parking_lot.out0                 → metrics_monitor.in(ParkingDurations)
exit_detection_subsystem.out     → access_controller.in(Outputs)
exit_gate.out0                   → exit_detection_subsystem.in(ExitGate) (libera la cola)
exit_gate.out0                   → access_controller.in(Outputs)          (egreso confirmado)
exit_gate.out0                   → metrics_monitor.in(VehicleEvents)
```

### 2.5 Salidas del sistema

PowerDEVS escribe los CSVs en `output/`:

| Archivo | Contenido |
|---|---|
| `occupancy.csv` | (tiempo, ocupación) |
| `rejection.csv` | (tiempo, vehículo) de ingresos denegados |
| `entrygate.csv` | (tiempo, vehículo) de ingresos aceptados |
| `exitgate.csv` | (tiempo, vehículo) de egresos |
| `dewlltime.csv` | reporte por vehículo de permanencia |

> El analizador admite también los nombres históricos en español
> (`ocupacion.csv`, `rechazos.csv`, `barreraEntrada.csv`, `barreraSalida.csv`, `duraciones.csv`).

---

## 3. Análisis

### 3.1 Análisis del modelo DEVS

- **Tasa de servicio del sistema de entrada:** un vehículo aceptado ocupa la
  barrera ~6–10 s (4 s de barrera + 1–3 s de cruce + autorización). Con arribos
  cada 40 s en promedio y capacidad 30, el sistema opera normalmente sin
  saturación; la ocupación media esperada es inferior a la capacidad.
- **Mecanismo de cola con "paso":** las colas (`entry_queue`, `exit_queue`)
  liberan un vehículo por evento de "paso" proveniente de la barrera. Esta
  sincronización asegura que no salgan dos vehículos a la vez por la barrera.
- **Estructura modular:** los bloques acoplados (`entry_detection_subsystem`,
  `exit_detection_subsystem`, `access_controller`, `metrics_monitor`) aíslan la
  lógica de control de la de medición, facilitando la mantenibilidad.
- **Determinismo parcial:** los generadores aleatorios usan `time(NULL)` como
  semilla; cada corrida es distinta (ver sugerencia 4.3).

### 3.2 Análisis del flujo de eventos

1. `vehicle_arrival_generator` emite un `VehicleId` nuevo → `entry_queue` (puerto 1).
2. `entry_queue`, al recibir la señal de "paso", libera el vehículo frontal →
   `entry_detector`, que emite `VehicleDetected` tras 1 s.
3. `entry_coordinator` consulta a `occupancy_tracker` (`AskIsFull`) y:
   - **capacidad disponible:** envía `GrantEntry` a la barrera; al confirmar el
     cruce (`VehicleAccepted`) informa a `parking_lot` y a la `vehicle_registry`,
     incrementa la ocupación y libera la cola;
   - **estacionamiento lleno:** envía `DenyEntry` a la barrera y al
     `rejection_counter`.
4. `parking_lot` asigna una estadía aleatoria; al vencer, emite `<id, tiempo>`
   hacia la `exit_queue` y el `dwell_time_auditor`.
5. `exit_queue` libera el vehículo hacia `exit_detector` (`ExitRequested`); el
   `exit_coordinator` autoriza (`GrantExit`), la barrera confirma el cruce
   (`VehicleExited`), se decrementa la ocupación y se registra el egreso.
6. `vehicle_registry` y `dwell_time_auditor` producen los reportes de permanencia.

### 3.3 Análisis de resultados (herramienta `analyze_simulation.py`)

El analizador (`./run_analysis.sh`) lee los CSVs de `output/` y calcula:

- Tiempo simulado, vehículos aceptados/rechazados y **tasa de rechazo**;
- **Ocupación media ponderada en el tiempo** y máximo registrado;
- Estadia promedio en el estacionamiento;
- **Chequeo de seguridad:** la ocupación nunca debe superar la capacidad;
- **Chequeo de vivacidad:** todo vehículo que ingresó debe haber egresado.

Y genera los gráficos en `output/plots/`:

- `occupancy.png` — ocupación vs. tiempo con línea de capacidad;
- `rejections.png` — cronograma de rechazos;
- `barrier_utilization.png` — utilización de barreras por ventana de tiempo;
- `simulation_dashboard.png` — panel resumen combinado.

### 3.4 Observaciones detectadas (casos límite)

- **Bloqueo de la cola de entrada ante un rechazo:** cuando la primera cola
  (cabeza) es rechazada, `entry_gate` solo emite el evento `DenyEntry` al
  contador (y a `parking_lot`, cuyo efecto es nulo). La cola queda con `pass =
  false`, y la señal de "paso" únicamente llega cuando **algún** vehículo es
  aceptado y cruza. Como la cola no libera más vehículos hasta recibir ese
  "paso", un rechazo en la cabeza de la cola puede bloquear indefinidamente a
  los vehículos posteriores (cabeza de línea con hambruna/estancamiento).
- **Conexión inerte** `entry_gate.out0 → parking_lot.in0`: transmite `DenyEntry`
  que `parking_lot` ignora; no aporta funcionalidad y confunde la lectura del
  modelo.
---

## 4. Sugerencias de mejora

1. **Resolver el bloqueo por rechazo en la cola de entrada.** Al denegar un
   vehículo, la cadena debe liberar al siguiente: por ejemplo, conectar también
   `entry_gate.out0` (denegación) al `entry_detection_subsystem.in(Gate)` como
   señal de "paso", o bien hacer que el `entry_coordinator` emita
   `DenyEntry` como disparador de liberación de cola.
2. **Eliminar la conexión inerte** `entry_gate.out0 → parking_lot.in0`.
3. **Colas con capacidad finita** y medición de longitud: en escenarios de alta
   demanda la cola infinita no refleja la realidad de un patio de espera
   limitado.
4. **Encolar los vehículos rechazados o redirigirlos** (girar y salir) como un
   estado explícito, para que el recuento de rechazos sea auditable.
5. **Definir los tiempos de cruce y autorización como parámetros configurables**
   del modelo (ya lo son vía editor) y documentar su rango válido.
6. **Estadística de tiempos de espera en las colas** (agregar estados de
   sincronización con sello de tiempo de ingreso a la cola).

---

## 5. Conclusión

El modelo cumple la especificación funcional de un estacionamiento inteligente:
control de acceso por capacidad, barreras con tiempos de cruce realistas,
estancias aleatorias y generación completa de métricas. La arquitectura modular
y la herramienta de análisis facilitan la validación (seguridad y vivacidad).
Las mejoras prioritarias son: corregir el bloqueo de la cola de entrada ante
rechazos, eliminar conexiones inertes, y robustecer la experimentación con
réplicas, warm-up e intervalos de confianza.
