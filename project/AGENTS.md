# Agent Instructions

## Project Layout

- `engine/` contains the PowerDEVS runtime and build templates.
- `atomics/project/` contains the Smart Parking System atomic models and shared types.
- `build/` contains generated model/build files and compiled objects.
- `output/` contains the model executable, simulation CSV files, and generated plots.
- The active PowerDEVS model definition is `~/Desktop/programs/sim/proyecto/proyecto.pdm` with compiled structure in `proyecto.pds`.

## Build

- Build from `build/`, not from `atomics/`: `make -f Makefile`.
- A clean rebuild is `make -f Makefile clean && make -f Makefile`.
- The executable is `output/model`.
- There are no repository tests; use a clean build as the required verification.

## PowerDEVS Generation

- `build/model.h` and `build/Makefile.include` are generated artifacts and may be overwritten when PowerDEVS regenerates the model.
- Persistent model changes belong in the `.pdm`/`.pds` files and atomic source directives, not only in generated files.
- PowerDEVS derives generated C++ constructor names from atomic header basenames. Keep atomic filenames, `class` names, constructors, and `.pdm`/`.pds` `Path` entries aligned, including PascalCase.
- Preserve `//CPP:` directives in atomic headers; they identify the implementation files and extra sources such as `randomc.cpp`.

## Analysis

- The analyzer is `atomics/project/analyze_simulation.py`; the launcher is `atomics/project/run_analysis.sh`.
- Run analysis from `atomics/project` with `./run_analysis.sh`; it reads `output/` and writes plots to `output/plots`.
- Python dependencies are listed in `atomics/project/requirements.txt`; use the project virtual environment when available.
- PowerDEVS output filenames can be English or legacy Spanish names, and the analyzer supports both. Do not assume the CSVs are in the current directory.
