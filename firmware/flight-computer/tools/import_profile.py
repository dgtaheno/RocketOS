#!/usr/bin/env python3
"""
import_profile.py — convert a flight data export into the project's
internal test-profile format and auto-detect the expected apogee.

Adapter architecture: each source format has its own reader that
produces (time_s, altitude_m) samples in SI units. Everything else
(apogee detection, file writing, manifest update) is shared.

Usage:
    python import_profile.py --source openrocket <in.csv> <name>
    python import_profile.py --source generic <in.csv> <name> \
        --time-col 0 --alt-col 2 --alt-unit ft --skip-header

Output (written into test_profiles/):
    <name>.csv    (time_s,altitude_m)
    <name>.meta   (name, source, expected_apogee_m, tolerance_m)
    manifest.txt  (updated to include <name>)
"""

import argparse
import csv
import os
import sys

FT_TO_M = 0.3048
OUT_DIR = os.path.join(os.path.dirname(__file__), "..", "test_profiles")


# ----------------------------------------------------------------------
# Adapters: each returns a list of (time_s, altitude_m) in SI units.
# ----------------------------------------------------------------------

def adapter_openrocket(path):
    """
    OpenRocket CSV export.
    OpenRocket writes comment lines starting with '#', and a header row
    with column names. Typical columns include 'Time (s)' and
    'Altitude (m)'. We locate them by name, case-insensitive.
    """
    samples = []
    time_idx = alt_idx = None

    with open(path, newline="") as f:
        for row in csv.reader(f):
            if not row:
                continue
            first = row[0].strip()
            if first.startswith("#"):
                continue
            # Header row: find columns by name
            if time_idx is None:
                lowered = [c.strip().lower() for c in row]
                for i, c in enumerate(lowered):
                    if "time" in c and time_idx is None:
                        time_idx = i
                    if "altitude" in c and alt_idx is None:
                        alt_idx = i
                if time_idx is not None and alt_idx is not None:
                    continue  # header consumed
                # not a header we recognise; keep scanning
                continue
            # Data row
            try:
                t = float(row[time_idx])
                a = float(row[alt_idx])
                samples.append((t, a))
            except (ValueError, IndexError):
                continue

    if not samples:
        raise ValueError(
            "Could not parse OpenRocket file: no 'Time'/'Altitude' columns found."
        )
    return samples


def adapter_generic(path, time_col, alt_col, alt_unit, skip_header):
    """
    Generic CSV: user specifies column indices and altitude unit.
    Works for most altimeter exports when the format is unknown.
    """
    samples = []
    scale = FT_TO_M if alt_unit == "ft" else 1.0

    with open(path, newline="") as f:
        reader = csv.reader(f)
        rows = list(reader)

    start = 1 if skip_header else 0
    for row in rows[start:]:
        if not row or row[0].strip().startswith("#"):
            continue
        try:
            t = float(row[time_col])
            a = float(row[alt_col]) * scale
            samples.append((t, a))
        except (ValueError, IndexError):
            continue

    if not samples:
        raise ValueError("Generic parse produced no samples; check --time-col/--alt-col.")
    return samples


# ----------------------------------------------------------------------
# Shared processing
# ----------------------------------------------------------------------

def normalise(samples):
    """
    Ensure time starts near zero and altitude is relative to the
    launch altitude (first sample). Keeps the internal format simple:
    ground = 0.
    """
    t0 = samples[0][0]
    a0 = samples[0][1]
    return [(round(t - t0, 3), round(a - a0, 2)) for (t, a) in samples]


def detect_apogee(samples):
    """Expected apogee = maximum altitude in the profile."""
    return round(max(a for (_, a) in samples), 1)


def choose_tolerance(apogee):
    """A simple tolerance heuristic: ~1.5% of apogee, min 3 m."""
    return round(max(3.0, apogee * 0.015), 1)


def write_outputs(name, source, samples, apogee, tolerance):
    os.makedirs(OUT_DIR, exist_ok=True)

    csv_path = os.path.join(OUT_DIR, f"{name}.csv")
    with open(csv_path, "w") as f:
        f.write("time_s,altitude_m\n")
        for t, a in samples:
            f.write(f"{t},{a}\n")

    meta_path = os.path.join(OUT_DIR, f"{name}.meta")
    with open(meta_path, "w") as f:
        f.write(f"name: {name}\n")
        f.write(f"source: {source}\n")
        f.write(f"expected_apogee_m: {apogee}\n")
        f.write(f"tolerance_m: {tolerance}\n")

    update_manifest(name)

    print(f"Wrote {name}.csv ({len(samples)} samples)")
    print(f"Wrote {name}.meta (apogee {apogee} m, tol {tolerance} m)")


def update_manifest(name):
    manifest = os.path.join(OUT_DIR, "manifest.txt")
    existing = []
    if os.path.exists(manifest):
        with open(manifest) as f:
            existing = [l.strip() for l in f if l.strip()]
    if name not in existing:
        existing.append(name)
        with open(manifest, "w") as f:
            for n in existing:
                f.write(n + "\n")
        print(f"Added '{name}' to manifest.txt")


# ----------------------------------------------------------------------
# CLI
# ----------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser(description="Import a flight profile for testing.")
    ap.add_argument("--source", required=True,
                    choices=["openrocket", "generic"],
                    help="input format adapter")
    ap.add_argument("input", help="input CSV file")
    ap.add_argument("name", help="profile name (used for output files)")

    # generic options
    ap.add_argument("--time-col", type=int, default=0)
    ap.add_argument("--alt-col", type=int, default=1)
    ap.add_argument("--alt-unit", choices=["m", "ft"], default="m")
    ap.add_argument("--skip-header", action="store_true")

    args = ap.parse_args()

    if args.source == "openrocket":
        raw = adapter_openrocket(args.input)
    else:
        raw = adapter_generic(
            args.input, args.time_col, args.alt_col,
            args.alt_unit, args.skip_header)

    samples = normalise(raw)
    apogee = detect_apogee(samples)
    tol = choose_tolerance(apogee)

    write_outputs(args.name, args.source, samples, apogee, tol)
    print("Done. Run 'pio test -e native' to include the new profile.")


if __name__ == "__main__":
    main()
