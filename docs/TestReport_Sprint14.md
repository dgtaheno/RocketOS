# TestReport_Sprint14.md

# Flight Telemetry & Data Logger
## Sprint 14 - Flight Profile Replay Testing

**Project:** Flight Telemetry & Data Logger  
**Repository:** flight-telemetry-data-logger  
**Sprint:** 14  
**Date:** August 2026  
**Author:** David Garcia-Taheno

---

# Objective

The objective of Sprint 14 was to add data-driven flight profile replay testing for the `FlightStateMachine`.

Previous tests validated the flight state machine using isolated synthetic altitude sequences defined directly in C++ unit tests. Sprint 14 extends this by replaying complete altitude-vs-time CSV profiles through the same flight state machine logic used by the firmware.

This provides a more realistic validation method and prepares the project to consume profiles from:

- synthetic generated flights
- OpenRocket exports
- real altimeter logs
- community-contributed flight data

---

# Scope

Sprint 14 focused on software validation only. No firmware behaviour was changed for the ESP32 target.

Implemented scope:

- CSV replay test framework
- Manifest-based profile discovery
- Per-profile metadata files
- Three initial test profiles
- Synthetic mach-dip stress case
- OpenRocket import adapter
- Generic CSV import adapter
- Native test integration with PlatformIO and GoogleTest

Out of scope:

- Real flight validation
- Recovery deployment output
- Sensor fusion with IMU data
- Automatic download of community flight files

---

# Repository Structure

The final structure of the Sprint 14 test framework is:

```text
firmware/flight-computer/
|-- test/
|   |-- test_flight_state_machine/
|   |   |-- test_flight_state_machine.cpp
|   |
|   |-- test_flight_profiles/
|       |-- test_flight_profiles.cpp
|
|-- test_profiles/
|   |-- manifest.txt
|   |-- estes_c6.csv
|   |-- estes_c6.meta
|   |-- midpower_machdip.csv
|   |-- midpower_machdip.meta
|   |-- low_apogee.csv
|   |-- low_apogee.meta
|
|-- tools/
|   |-- import_profile.py
|
|-- platformio.ini
```

---

# Test Profile Format

Each flight profile is represented by two files:

```text
profile_name.csv
profile_name.meta
```

The CSV contains altitude samples:

```csv
time_s,altitude_m
0.05,0.21
0.10,0.64
0.15,1.28
```

The `.meta` file contains the expected validation values:

```text
name: Mid-power - transonic mach dip
source: synthetic
expected_apogee_m: 904.4
tolerance_m: 8.0
```

The manifest file lists which profiles are included in the test run:

```text
estes_c6
midpower_machdip
low_apogee
```

This design avoids platform-specific directory scanning and keeps test discovery portable across Windows, Linux and CI environments.

---

# Implemented Flight Profiles

## 1. estes_c6

| Field | Value |
|-------|-------|
| Description | Small clean flight |
| Source | Synthetic |
| Expected apogee | 253.9 m |
| Tolerance | 4.0 m |
| Purpose | Nominal flight validation |

This profile validates a clean, small-rocket-style flight sequence with launch, boost, coast, apogee, descent and landing.

---

## 2. midpower_machdip

| Field | Value |
|-------|-------|
| Description | Mid-power transonic mach-dip case |
| Source | Synthetic |
| Expected apogee | 904.4 m |
| Tolerance | 8.0 m |
| Purpose | False barometric dip rejection |

This profile includes a synthetic transonic barometric disturbance. It is designed to ensure that the flight state machine does not trigger a false apogee when the barometric altitude briefly dips during fast ascent.

This test is important for model rocketry because transonic pressure effects can produce misleading barometric readings.

---

## 3. low_apogee

| Field | Value |
|-------|-------|
| Description | Low apogee short flight |
| Source | Synthetic |
| Expected apogee | 71.5 m |
| Tolerance | 3.0 m |
| Purpose | Short-flight validation |

This profile validates detection behaviour on a low-altitude, short-duration flight where timing margins are tighter.

---

# Test Implementation

## Parameterized GoogleTest

The replay test is implemented as a parameterized GoogleTest suite:

```text
RealFlightProfiles/FlightProfileTest.DetectsApogeeWithinTolerance/<profile_name>
```

For each profile, the test:

1. Reads the CSV samples
2. Reads the `.meta` expected apogee and tolerance
3. Replays each altitude sample through `FlightStateMachine::update()`
4. Tracks visited flight states
5. Confirms that the full flight sequence occurred
6. Compares detected apogee against the expected value

---

# Import Tool

The sprint also added:

```text
firmware/flight-computer/tools/import_profile.py
```

The import tool converts external flight data into the internal replay format.

Supported adapters:

| Adapter | Purpose |
|---------|---------|
| `openrocket` | Import OpenRocket CSV exports by detecting time and altitude columns |
| `generic` | Import generic CSV files using configurable time and altitude columns |

Example OpenRocket import:

```bash
python tools/import_profile.py --source openrocket exported_flight.csv my_openrocket_flight
```

Example generic import:

```bash
python tools/import_profile.py --source generic altimeter.csv my_real_flight --time-col 0 --alt-col 2 --alt-unit ft --skip-header
```

The tool writes:

```text
<name>.csv
<name>.meta
```

and updates:

```text
manifest.txt
```

---

# Test Execution

Command used:

```powershell
cd C:\Users\scrap\Documents\projects\flight-telemetry-data-logger\firmware\flight-computer
pio test -e native
```

---

# Test Output

```text
Processing test_flight_profiles in native environment
Building...
Testing...
RealFlightProfiles/FlightProfileTest.DetectsApogeeWithinTolerance/estes_c6          [PASSED]
RealFlightProfiles/FlightProfileTest.DetectsApogeeWithinTolerance/midpower_machdip  [PASSED]
RealFlightProfiles/FlightProfileTest.DetectsApogeeWithinTolerance/low_apogee        [PASSED]

native:test_flight_profiles [PASSED]

Processing test_flight_state_machine in native environment
Building...
Testing...
FlightStateMachine.FullFlightSequence              [PASSED]
FlightStateMachine.IgnoresBarometricNoiseAtIdle    [PASSED]
FlightStateMachine.DoesNotLaunchOnSlowRise         [PASSED]
FlightStateMachine.LaunchesOnFastRise              [PASSED]
FlightStateMachine.ApogeeAltitudeMatchesPeak       [PASSED]
FlightStateMachine.ResetReturnsToIdle              [PASSED]

native:test_flight_state_machine [PASSED]

9 test cases: 9 succeeded
```

---

# Results Summary

| Test Suite | Result |
|------------|--------|
| Flight profile replay tests | PASS |
| Flight state machine unit tests | PASS |
| Total test cases | 9 passed, 0 failed |

---

# Per-Profile Results

| Profile | Expected Apogee | Detected Result | Status |
|---------|-----------------|-----------------|--------|
| `estes_c6` | 253.9 m | Within tolerance | PASS |
| `midpower_machdip` | 904.4 m | Within tolerance | PASS |
| `low_apogee` | 71.5 m | Within tolerance | PASS |

---

# Key Finding: Mach-Dip Robustness

The most important result of Sprint 14 is that the flight state machine passed the `midpower_machdip` profile.

This profile includes a synthetic false barometric dip during high-speed ascent. The state machine did not trigger a false apogee and correctly detected the real apogee.

This confirms that the current apogee detection logic is robust against brief false altitude disturbances because it uses:

- filtered climb rate
- minimum altitude threshold
- multi-sample confirmation
- full state progression through BOOST and COAST before APOGEE

This result is especially relevant for future discussion with model rocketry communities, since mach-dip effects are a known concern in barometric flight computers.

---

# Issues Encountered

## 1. Incorrect test folder placement

### Problem

The initial `test_flight_profiles.cpp` file was accidentally placed inside the existing `test_flight_state_machine` test folder. This caused both test files to be compiled into the same executable and produced a duplicate `main()` linker error.

### Error

```text
multiple definition of main
```

### Fix

The file was moved to its own PlatformIO test folder:

```text
test/test_flight_profiles/test_flight_profiles.cpp
```

### Result

RESOLVED

---

## 2. Data folder placed inside test folder

### Problem

The `test_profiles` data folder was initially placed under `test/`, causing PlatformIO to treat it as a test suite and report:

```text
Error: Nothing to build
```

### Fix

The data folder was moved outside `test/`:

```text
firmware/flight-computer/test_profiles/
```

### Result

RESOLVED

---

# Validation Impact

Sprint 14 significantly improves confidence in the flight state machine because it validates behaviour against complete flight profiles instead of only isolated unit-test sequences.

This enables future validation against:

- OpenRocket simulations
- real altimeter exports
- community-contributed CSV flight logs
- edge cases such as mach dip, slow launches, low apogee flights and noisy data

---

# Known Limitations

- Current profiles are synthetic, not real flight logs
- OpenRocket import has been implemented but not yet validated against an actual OpenRocket export file from this repository
- Replay testing currently uses altitude only; future IMU-based validation may include acceleration and angular-rate data
- Real field validation is still pending

---

# Conclusion

Sprint 14 successfully added data-driven flight profile replay testing to the project.

The framework validates the flight state machine against complete CSV profiles and confirms that the current apogee detection logic handles nominal, low-apogee and synthetic mach-dip scenarios correctly.

All tests pass:

```text
9 test cases: 9 succeeded
```

The project is now ready to incorporate additional flight profiles from OpenRocket, real altimeters or community-contributed data.

**Sprint Status:** COMPLETED

**Recommended next step:** Recovery Deployment Output (servo) or additional OpenRocket/real flight profile imports.
