#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
DATA_DIR="${SCRIPT_DIR}/../../output"
OUTPUT_DIR="${DATA_DIR}/plots"
PYTHON="${SCRIPT_DIR}/venv/bin/python"

if [[ ! -x "${PYTHON}" ]]; then
    PYTHON="python3"
fi

exec "${PYTHON}" "${SCRIPT_DIR}/analyze_simulation.py" \
    --data-dir "${DATA_DIR}" \
    --output-dir "${OUTPUT_DIR}" \
    "$@"
