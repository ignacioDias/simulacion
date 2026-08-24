#!/usr/bin/env python3
"""Analyze Smart Parking System CSV output and generate summary plots."""

from __future__ import annotations

import argparse
from pathlib import Path

try:
    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd
except ModuleNotFoundError as error:
    missing = error.name or "required package"
    raise SystemExit(
        f"Missing Python package: {missing}. Install dependencies with "
        "python3 -m pip install -r requirements.txt"
    ) from None


def read_pairs(path: Path, names: list[str]) -> pd.DataFrame:
    """Read a headerless PowerDEVS CSV file, returning an empty frame if absent."""
    if not path.is_file() or path.stat().st_size == 0:
        return pd.DataFrame(columns=names)

    data = pd.read_csv(path, header=None, names=names, usecols=range(len(names)))
    for column in names:
        data[column] = pd.to_numeric(data[column], errors="coerce")
    return data.dropna(subset=names)


def first_existing(data_dir: Path, names: list[str]) -> Path | None:
    for name in names:
        path = data_dir / name
        if path.is_file() and path.stat().st_size > 0:
            return path
    return None


def read_duration_report(path: Path | None) -> pd.DataFrame:
    if path is None or not path.is_file() or path.stat().st_size == 0:
        return pd.DataFrame(columns=["vehicle_id", "duration", "exit_delay", "simulation_time"])

    data = pd.read_csv(path)
    data.columns = data.columns.str.strip()
    aliases = {
        "Patente": "vehicle_id",
        "TiempoEstacionado": "duration",
        "DemoraSalida": "exit_delay",
        "TiempoSimulacion": "simulation_time",
    }
    data = data.rename(columns=aliases)
    required = ["vehicle_id", "duration", "exit_delay", "simulation_time"]
    for column in required:
        if column not in data:
            data[column] = pd.NA
    for column in required:
        data[column] = pd.to_numeric(data[column], errors="coerce")
    return data.dropna(subset=["vehicle_id", "duration"])[required]


def load_data(data_dir: Path) -> dict[str, pd.DataFrame]:
    duration_path = first_existing(
        data_dir, ["duraciones.csv", "dwelltime.csv", "dewlltime.csv", "DwellTimeLog"]
    )
    duration = read_duration_report(duration_path)

    return {
        "occupancy": read_pairs(
            first_existing(data_dir, ["occupancy.csv", "ocupacion.csv"]) or data_dir / "occupancy.csv",
            ["time", "occupancy"],
        ),
        "rejections": read_pairs(
            first_existing(data_dir, ["rejection.csv", "rechazos.csv"]) or data_dir / "rejection.csv",
            ["time", "vehicle_id"],
        ),
        "entries": read_pairs(
            first_existing(data_dir, ["entrygate.csv", "barreraEntrada.csv"]) or data_dir / "entrygate.csv",
            ["time", "vehicle_id"],
        ),
        "exits": read_pairs(
            first_existing(data_dir, ["exitgate.csv", "barreraSalida.csv"]) or data_dir / "exitgate.csv",
            ["time", "vehicle_id"],
        ),
        "duration": duration,
    }


def weighted_occupancy(occupancy: pd.DataFrame) -> tuple[float, float]:
    if len(occupancy) < 2:
        return 0.0, float(occupancy["time"].max()) if not occupancy.empty else 0.0

    ordered = occupancy.sort_values("time").drop_duplicates("time")
    intervals = ordered["time"].shift(-1) - ordered["time"]
    valid = intervals > 0
    total_time = float(ordered["time"].iloc[-1])
    if total_time <= 0:
        return 0.0, total_time
    average = float((ordered.loc[valid, "occupancy"] * intervals[valid]).sum() / total_time)
    return average, total_time


def print_summary(data: dict[str, pd.DataFrame], capacity: int) -> None:
    occupancy = data["occupancy"]
    entries = data["entries"]
    exits = data["exits"]
    duration = data["duration"]
    rejections = data["rejections"]
    average_occupancy, simulation_time = weighted_occupancy(occupancy)

    accepted = len(duration) if not duration.empty else len(entries)
    rejected = len(rejections)
    attempts = accepted + rejected
    rejection_rate = 100 * rejected / attempts if attempts else 0.0

    print("\nSmart Parking System - Simulation Summary")
    print("=" * 48)
    print(f"Simulation time:       {simulation_time:.2f} s")
    print(f"Accepted vehicles:     {accepted}")
    print(f"Rejected vehicles:     {rejected}")
    print(f"Rejection rate:        {rejection_rate:.2f}%")
    print(f"Average occupancy:     {average_occupancy:.2f} vehicles")
    if not duration.empty:
        print(f"Average parking time:  {duration['duration'].mean():.2f} s")
    else:
        print("Average parking time:  unavailable")

    print("\nProperty checks")
    print("=" * 48)
    if occupancy.empty:
        print("Safety:                unavailable (no occupancy data)")
    else:
        maximum = int(occupancy["occupancy"].max())
        status = "PASS" if maximum <= capacity else "FAIL"
        print(f"Safety (capacity <= {capacity}): {status} (maximum: {maximum})")

    entered = set(entries["vehicle_id"].astype(int))
    exited = set(exits["vehicle_id"].astype(int))
    still_inside = entered - exited
    status = "PASS" if not still_inside else "FAIL"
    print(f"Liveness (all vehicles exit):   {status} (inside: {len(still_inside)})")


def plot_results(data: dict[str, pd.DataFrame], output_dir: Path, capacity: int,
                 window: float, barrier_cycle: float) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    occupancy = data["occupancy"]
    rejections = data["rejections"]
    entries = data["entries"]
    exits = data["exits"]

    if not occupancy.empty:
        figure, axis = plt.subplots(figsize=(11, 6))
        axis.step(occupancy["time"], occupancy["occupancy"], where="post", label="Occupancy")
        axis.axhline(capacity, color="red", linestyle="--", label="Capacity")
        axis.set(title="Parking Occupancy", xlabel="Simulation time (s)", ylabel="Vehicles")
        axis.grid(True, alpha=0.3)
        axis.legend()
        figure.tight_layout()
        figure.savefig(output_dir / "occupancy.png", dpi=150)
        plt.close(figure)

    if not rejections.empty:
        figure, axis = plt.subplots(figsize=(11, 4))
        axis.eventplot(rejections["time"], orientation="horizontal", colors="crimson", linewidths=2)
        axis.set(title="Vehicle Rejections", xlabel="Simulation time (s)", yticks=[])
        axis.grid(True, axis="x", alpha=0.3)
        figure.tight_layout()
        figure.savefig(output_dir / "rejections.png", dpi=150)
        plt.close(figure)

    if not entries.empty or not exits.empty:
        last_time = max(
            entries["time"].max() if not entries.empty else 0,
            exits["time"].max() if not exits.empty else 0,
        )
        bins = np.arange(0, last_time + window, window)
        if len(bins) < 2:
            bins = np.array([0, window])
        entry_counts, _ = np.histogram(entries["time"], bins) if not entries.empty else (np.zeros(len(bins) - 1), bins)
        exit_counts, _ = np.histogram(exits["time"], bins) if not exits.empty else (np.zeros(len(bins) - 1), bins)
        centers = (bins[:-1] + bins[1:]) / 2 / 3600
        entry_utilization = entry_counts * barrier_cycle / window * 100
        exit_utilization = exit_counts * barrier_cycle / window * 100

        figure, axis = plt.subplots(figsize=(11, 6))
        axis.plot(centers, entry_utilization, marker="o", label="Entry gate")
        axis.plot(centers, exit_utilization, marker="o", label="Exit gate")
        axis.set(title="Barrier Utilization", xlabel="Simulation time (hours)", ylabel="Utilization (%)")
        axis.set_ylim(bottom=0)
        axis.grid(True, alpha=0.3)
        axis.legend()
        figure.tight_layout()
        figure.savefig(output_dir / "barrier_utilization.png", dpi=150)
        plt.close(figure)

    # Create a report-style figure so all key results are visible at once.
    figure, axes = plt.subplots(2, 2, figsize=(14, 9))
    occupancy_axis, rejection_axis, barrier_axis, summary_axis = axes.flat

    if occupancy.empty:
        occupancy_axis.text(0.5, 0.5, "No occupancy data", ha="center", va="center")
    else:
        occupancy_axis.step(occupancy["time"], occupancy["occupancy"], where="post", color="royalblue")
        occupancy_axis.axhline(capacity, color="crimson", linestyle="--", label="Capacity")
        occupancy_axis.legend()
    occupancy_axis.set(title="Parking Occupancy", xlabel="Simulation time (s)", ylabel="Vehicles")
    occupancy_axis.grid(True, alpha=0.3)

    if rejections.empty:
        rejection_axis.text(0.5, 0.5, "No rejection events", ha="center", va="center")
    else:
        rejection_axis.eventplot(rejections["time"], orientation="horizontal", colors="crimson", linewidths=2)
    rejection_axis.set(title="Vehicle Rejections", xlabel="Simulation time (s)", yticks=[])
    rejection_axis.grid(True, axis="x", alpha=0.3)

    if entries.empty and exits.empty:
        barrier_axis.text(0.5, 0.5, "No gate data", ha="center", va="center")
    else:
        last_time = max(
            entries["time"].max() if not entries.empty else 0,
            exits["time"].max() if not exits.empty else 0,
        )
        bins = np.arange(0, last_time + window, window)
        if len(bins) < 2:
            bins = np.array([0, window])
        entry_counts = np.histogram(entries["time"], bins)[0] if not entries.empty else np.zeros(len(bins) - 1)
        exit_counts = np.histogram(exits["time"], bins)[0] if not exits.empty else np.zeros(len(bins) - 1)
        centers = (bins[:-1] + bins[1:]) / 2 / 3600
        barrier_axis.plot(centers, entry_counts * barrier_cycle / window * 100, label="Entry gate")
        barrier_axis.plot(centers, exit_counts * barrier_cycle / window * 100, label="Exit gate")
        barrier_axis.legend()
    barrier_axis.set(title="Barrier Utilization", xlabel="Simulation time (hours)", ylabel="Utilization (%)")
    barrier_axis.set_ylim(bottom=0)
    barrier_axis.grid(True, alpha=0.3)

    average_occupancy, simulation_time = weighted_occupancy(occupancy)
    summary_axis.axis("off")
    summary_axis.text(
        0.05,
        0.9,
        "Simulation Statistics\n\n"
        f"Duration: {simulation_time:.2f} s\n"
        f"Accepted vehicles: {len(entries)}\n"
        f"Rejected vehicles: {len(rejections)}\n"
        f"Average occupancy: {average_occupancy:.2f}\n"
        f"Peak occupancy: {occupancy['occupancy'].max() if not occupancy.empty else 0:.0f}\n"
        f"Capacity: {capacity}",
        va="top",
        fontsize=12,
    )

    figure.suptitle("Smart Parking System - Simulation Report", fontsize=16)
    figure.tight_layout()
    figure.savefig(output_dir / "simulation_dashboard.png", dpi=180)
    plt.close(figure)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data-dir", type=Path, default=Path("."), help="Directory containing simulation CSV files")
    parser.add_argument("--output-dir", type=Path, default=Path("plots"), help="Directory for generated plots")
    parser.add_argument("--capacity", type=int, default=30)
    parser.add_argument("--window", type=float, default=1800, help="Utilization window in seconds")
    parser.add_argument("--barrier-cycle", type=float, default=10, help="Estimated gate cycle per vehicle in seconds")
    args = parser.parse_args()

    data = load_data(args.data_dir)
    print_summary(data, args.capacity)
    plot_results(data, args.output_dir, args.capacity, args.window, args.barrier_cycle)
    print(f"\nPlots written to {args.output_dir}")


if __name__ == "__main__":
    main()
