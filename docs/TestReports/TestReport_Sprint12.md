# TestReport_Sprint12.md

# Flight Telemetry & Data Logger
## Sprint 12 – Flight Event Detection

**Project:** Flight Telemetry & Data Logger
**Repository:** flight-telemetry-data-logger
**Sprint:** 12
**Release:** v0.12.0
**Date:** August 2026
**Author:** David Garcia-Taheño

---

# Objective

The objective of Sprint 12 was to add autonomous flight event detection to the
Flight Telemetry & Data Logger, transitioning the project from a telemetry
platform into a flight computer capable of recognising the phases of a flight
and reporting them to a Ground Control Station.

This sprint focused on:

- Design of a flight state machine for model rocketry
- Barometric, GPS-independent event detection
- Launch, burnout, apogee, descent and landing detection
- Climb rate estimation with noise rejection
- Unit testing with GoogleTest on the host (native environment)
- Synthetic flight simulation for hardware validation
- MAVLink STATUSTEXT reporting of flight events to QGroundControl

---

# Scope

The flight state machine models a **ballistic flight profile** as used in model
rocketry: powered boost, unpowered coast, a single apogee, and parachute
recovery. It assumes an irreversible trajectory (the vehicle goes up once and
comes down once).

It is **not** intended for powered controlled flight such as multirotors or
fixed-wing aircraft, where altitude changes are arbitrary and repeatable. This
scope is documented in the source and in the README.

---

# Software Configuration

| Component | Version |
|------------|----------|
| PlatformIO | Latest |
| ESP32 Arduino Framework | Current |
| GoogleTest | 1.17.0 (native environment) |
| Host toolchain | MinGW-w64 GCC 16.1.0 (UCRT64) |
| QGroundControl | Installed and validated |
| Firmware Branch | sprint12-flight-events |

---

# Flight State Machine

## States

```text
IDLE -> BOOST -> COAST -> APOGEE -> DESCENT -> LANDED
```

| State | Meaning | Detection |
|--------|---------|-----------|
| IDLE | On the pad, waiting | Altitude stable, no rapid climb |
| BOOST | Motor burning, thrust | Climb rate above launch threshold at altitude |
| COAST | Motor off, rising by inertia | Climb rate stops increasing (burnout) |
| APOGEE | Highest point | Climb rate crosses to zero at altitude |
| DESCENT | Falling under recovery | Sustained negative climb rate |
| LANDED | Back on the ground | Altitude near zero and climb rate near zero |

## Detection thresholds

| Parameter | Value |
|------------|--------|
| LAUNCH_CLIMB_MS | 5.0 m/s |
| LAUNCH_ALTITUDE_M | 3.0 m |
| APOGEE_MIN_ALTITUDE_M | 5.0 m |
| DESCENT_CLIMB_MS | -2.0 m/s |
| LANDED_ALTITUDE_M | 2.0 m |
| LANDED_CLIMB_MS | 0.5 m/s |

Each transition requires several consecutive confirming samples, and the climb
rate is smoothed with a low-pass filter to reject barometric noise.

## Design notes

- The state machine is fed from the barometric relative altitude, so it operates
  independently of GPS fix. A rocket can lose GPS lock under acceleration; the
  barometer is autonomous and provides fine altitude resolution.
- The class contains no Arduino dependencies (uses `<cstdint>` and `<cmath>` and
  receives time as a parameter), which allows it to compile and run both on the
  ESP32 and on the host for unit testing.

---

# Unit Testing (GoogleTest)

Unit tests run on the host PC through the PlatformIO `native` environment,
independently of the ESP32 hardware.

## Test cases

| Test | Purpose |
|------|---------|
| FullFlightSequence | Synthetic flight walks through every state and finishes LANDED |
| IgnoresBarometricNoiseAtIdle | Sensor noise while stationary does not trigger a false launch |
| DoesNotLaunchOnSlowRise | A slow 1 m/s rise (e.g. lifting by hand) does not launch |
| LaunchesOnFastRise | A fast 15 m/s rise leaves IDLE (launch detected) |
| ApogeeAltitudeMatchesPeak | The recorded apogee altitude matches the peak altitude |
| ResetReturnsToIdle | reset() returns the machine to a clean IDLE state |

## Result

```text
FlightStateMachine.FullFlightSequence            [PASSED]
FlightStateMachine.IgnoresBarometricNoiseAtIdle  [PASSED]
FlightStateMachine.DoesNotLaunchOnSlowRise       [PASSED]
FlightStateMachine.LaunchesOnFastRise            [PASSED]
FlightStateMachine.ApogeeAltitudeMatchesPeak     [PASSED]
FlightStateMachine.ResetReturnsToIdle            [PASSED]

6 test cases: 6 succeeded
```

**Status:** All 6 unit tests passing.

---

# Design Finding – Landing Detection

During test development, two design characteristics of the landing detection
were identified and documented.

## Parachute recovery assumption

The landing detection assumes recovery with a parachute. A rocket in a purely
ballistic fall reaches the ground at a high velocity (tens of m/s), which the
logic correctly does not classify as a landing (that is an impact, not a
landing). Landing is confirmed only for a low descent velocity, consistent with
a deployed recovery system.

## Confirmation while at rest

Landing is confirmed with the vehicle at rest on the ground (altitude near zero,
climb rate near zero), not during the descent. Therefore the flight computer must
continue logging after touchdown, allowing the state machine to observe the
resting condition and confirm LANDED.

Both characteristics were reproduced in the unit tests and in the synthetic
flight profile.

---

# Synthetic Flight Simulation

A `FlightSimulator` generates a ballistic flight profile with parachute recovery:

```text
powered boost -> ballistic coast -> apogee -> parachute descent -> resting
```

## Simulation parameters

| Parameter | Value |
|------------|--------|
| Boost duration | 1.5 s |
| Boost acceleration | 30 m/s^2 |
| Gravity | 9.81 m/s^2 |
| Parachute terminal velocity | 5 m/s |

---

# Functional Tests

## Test 1 – Host Unit Tests

### Procedure

Run `pio test -e native`.

### Expected Result

All GoogleTest cases pass.

### Actual Result

PASS – 6 of 6 test cases succeeded.

---

## Test 2 – Simulation on Hardware

### Procedure

Run the synthetic flight profile through the FlightStateMachine on the ESP32
and observe the state transitions on the serial monitor.

### Expected Result

The full sequence IDLE to LANDED, with a physically sensible apogee.

### Actual Result

PASS

```text
[0.7 s] -> BOOST    | alt = 5.7 m
[2.1 s] -> COAST    | alt = 40.3 m
[4.9 s] -> APOGEE   | alt = 68.4 m
[5.2 s] -> DESCENT  | alt = 66.9 m
        -> LANDED
Apogee altitude : 69.5 m
Final state     : LANDED
```

---

## Test 3 – Real Sensor Robustness

### Procedure

Run the FlightStateMachine fed from the real BMP388 relative altitude while the
prototype is stationary and while being moved slowly by hand.

### Expected Result

No false launch detection.

### Actual Result

PASS

Over several minutes, with the barometer fluctuating within approximately
plus or minus 0.3 m and the prototype being handled, the state machine remained
in IDLE. No false positives were observed.

A slow rise of about one metre performed by hand also did not trigger a launch,
confirming that detection requires both altitude and vertical speed
simultaneously.

---

## Test 4 – MAVLink STATUSTEXT Events

### Procedure

Transmit flight events as MAVLink STATUSTEXT and observe them in QGroundControl.

### Expected Result

The flight event sequence appears in the QGroundControl Vehicle Messages panel.

### Actual Result

PASS

```text
Info: FLIGHT STATE: BOOST
Info: FLIGHT STATE: COAST
Info: APOGEE DETECTED - 69.5 m
Info: FLIGHT STATE: DESCENT
Info: FLIGHT STATE: LANDED
```

The apogee altitude computed on board (69.5 m) is reported to the ground station.

---

## Test 5 – Continuous Operation

### Procedure

Run the firmware for an extended period with event detection active.

### Expected Result

Stable operation, no regression in existing telemetry or logging.

### Actual Result

PASS

```text
Fault Flags   : 0x00
System Healthy: YES
```

No crashes, brownouts or watchdog resets observed. Existing telemetry, logging
and fault-tolerance behaviour were unaffected.

---

# Results Summary

| Test | Result |
|------|--------|
| Host Unit Tests (GoogleTest) | PASS |
| Simulation on Hardware | PASS |
| Real Sensor Robustness | PASS |
| MAVLink STATUSTEXT Events | PASS |
| Continuous Operation | PASS |

---

# Sprint Achievements

### Completed

- FlightStateMachine designed and implemented
- Barometric, GPS-independent event detection
- Launch, burnout, apogee, descent and landing detection
- Climb rate estimation with low-pass filtering
- 6 GoogleTest unit tests passing on the host
- Host toolchain (MinGW-w64 GCC) and native test environment configured
- FlightSimulator synthetic flight profile
- Simulation validated on ESP32 hardware
- Robustness validated against real BMP388 noise (no false positives)
- MAVLink STATUSTEXT flight events validated in QGroundControl
- Project scope documented as a model rocketry flight computer

### Repository Status

- Build stable
- Event detection operational
- Unit tests passing
- No regression in existing functionality

---

# Known Limitations

- Detection thresholds are fixed and tuned for a small rocket. Adaptive or
  configurable thresholds are not yet implemented.
- The flight state machine targets ballistic recovery flight only. Powered
  controlled vehicles are out of scope by design.
- Field validation with a real launch has not yet been performed; validation to
  date is by unit test, synthetic simulation and bench robustness testing.

---

# Conclusion

Sprint 12 added autonomous flight event detection to the Flight Telemetry & Data
Logger, implemented as a barometric, GPS-independent state machine covering
launch, burnout, apogee, descent and landing.

The detection logic is covered by six GoogleTest unit tests running on the host,
validated in a synthetic flight simulation on the ESP32 (apogee 69.5 m), and
shown to be robust against real BMP388 sensor noise with no false positives.
Flight events are reported to QGroundControl as MAVLink STATUSTEXT messages,
including the on-board computed apogee altitude.

With this sprint, the project evolves from a MAVLink telemetry platform into a
flight computer with autonomous flight phase detection, backed by unit tests.

**Sprint Status:** COMPLETED

**Ready for Sprint 13 – Recovery Deployment Output**
