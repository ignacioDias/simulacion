# Smart Parking System

This directory contains the atomic models and shared C++ types for the
PowerDEVS Smart Parking System. The model simulates vehicle arrivals, entry
and exit queues, gates, parking duration, capacity tracking, rejections, and
simulation metrics.

## Atomic Models

- `VehicleArrivalGenerator`
- `EntryQueue` and `EntryDetector`
- `ExitQueue` and `ExitDetector`
- `OccupancyTracker`
- `EntryCoordinator` and `ExitCoordinator`
- `EntryGate` and `ExitGate`
- `ParkingLot`
- `RejectionCounter`
- `VehicleRegistry`
- `DwellTimeAuditor`

Shared event and data types are defined in `project/common.h` and
`project/common_controller.h`.

## Build

Build from the PowerDEVS build directory:

```bash
cd /path/to/powerdevs/build
make -f Makefile
```

The executable is created at `powerdevs/output/model`.

## Simulation Output

PowerDEVS writes CSV metrics to the output directory, including:

- `occupancy.csv`
- `entrygate.csv`
- `exitgate.csv`
- `rejection.csv`
- `duraciones.csv`

## Analysis and Graphs

The project includes a Python analyzer that prints statistics and generates
occupancy, rejection, barrier-utilization, and dashboard plots.

```bash
cd /path/to/powerdevs/atomics/project
python3 -m venv venv
venv/bin/python -m pip install -r requirements.txt
./run_analysis.sh
```

Plots are written to `powerdevs/output/plots`. The analyzer can also be run
directly with custom paths:

```bash
venv/bin/python analyze_simulation.py \
  --data-dir /path/to/powerdevs/output \
  --output-dir /path/to/powerdevs/output/plots
```

The PowerDEVS model definition is maintained separately in the `.pdm` and
`.pds` files used by the simulator. Keep their atomic header paths aligned
with the PascalCase filenames in `project/` when editing the model.
