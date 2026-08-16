# TestReport_Sprint14b.md

# Flight Telemetry & Data Logger
## Sprint 14b - Serial Hardware Flight Profile Replay

**Project:** Flight Telemetry & Data Logger  
**Repository:** flight-telemetry-data-logger  
**Sprint:** 14b  
**Release:** v0.14.1  
**Date:** August 2026  
**Author:** David Garcia-Taheño

---

# Objective

The objective of Sprint 14b was to extend the Sprint 14 flight profile replay framework from host-based native tests to real ESP32 hardware.

Sprint 14 validated profiles on the PC using the PlatformIO native environment. Sprint 14b adds a serial hardware replay mode where a PC streams CSV altitude samples to the ESP32 over USB serial, and the actual firmware feeds those samples into the real `FlightStateMachine`.

This provides a hardware-in-the-loop style validation path without requiring a real rocket flight.

---

# Scope

Implemented scope:

- Serial flight profile replay mode
- PC-to-ESP32 CSV streaming tool
- Hardware execution of replay profiles
- Clean serial output mode controlled by `SERIAL_REPLAY_VERBOSE`
- Replay-ready handshake using `[REPLAY] READY`
- MAVLink disabled during serial replay to avoid binary output on the command/debug serial port
- Validation of `low_apogee.csv`
- Validation of corrected `midpower_machdip.csv`

Out of scope:

- Servo or actuator deployment
- MAVLink replay over a separate telemetry port
- SD-based profile replay
- Real launch validation

---

# Architecture

```text
PC CSV profile
      |
      v
send_profile_serial.py
      |
      v
USB Serial
      |
      v
SerialProfileReplay
      |
      v
FlightStateMachine
      |
      v
Flight events + buzzer feedback
```

---

# Serial Protocol

The PC sends:

```text
PROFILE_START
R,<time_ms>,<altitude_m>
R,<time_ms>,<altitude_m>
...
PROFILE_END
```

The ESP32 responds:

```text
[REPLAY] READY
[REPLAY] START
[FLIGHT] State -> BOOST
[FLIGHT] State -> COAST
[FLIGHT] State -> APOGEE
[FLIGHT] State -> DESCENT
[FLIGHT] State -> LANDED
[REPLAY] END | samples=<count>
```

---

# Configuration

Replay mode is controlled from `Config.h`:

```cpp
#define SERIAL_PROFILE_REPLAY_MODE 1
#define SERIAL_REPLAY_ALLOW_ACTUATORS 0
#define SERIAL_REPLAY_VERBOSE 0
```

Before committing or using normal firmware mode, replay is disabled:

```cpp
#define SERIAL_PROFILE_REPLAY_MODE 0
#define SERIAL_REPLAY_ALLOW_ACTUATORS 0
#define SERIAL_REPLAY_VERBOSE 0
```

`SERIAL_REPLAY_ALLOW_ACTUATORS` is reserved for future recovery hardware safety. It remains disabled by default.

---

# Important Fix: Replay Ready Handshake

## Problem

The first version of the Python sender began transmitting when the ESP32 printed:

```text
[INFO] Waiting for PROFILE_START...
```

That message appeared too early, while the ESP32 was still inside `setup()` and not yet processing serial replay samples in `loop()`.

This caused samples to be lost at the beginning of the replay.

Example observed on `low_apogee.csv`:

```text
[REPLAY] END | samples=382
```

The expected number of samples was:

```text
491
```

## Fix

A new replay-ready marker was added at the end of `setup()`:

```cpp
#if SERIAL_PROFILE_REPLAY_MODE

    Serial.println("[REPLAY] READY");

#endif
```

The Python script now waits specifically for:

```text
[REPLAY] READY
```

before sending `PROFILE_START`.

## Result

No samples are lost at the beginning of the replay.

Corrected result:

```text
[REPLAY] END | samples=491
```

---

# Profile Correction: midpower_machdip

## Problem

The original synthetic `midpower_machdip.csv` profile was cut at 120 seconds while the vehicle was still descending at approximately 241 m.

This produced an artificial jump:

```text
240.97 m -> 0.00 m
```

This was not physically valid and made the landing portion unsuitable for validation.

## Fix

The profile was regenerated so that descent continues smoothly to ground level.

Corrected descent excerpt:

```text
t=160.20 s | alt=0.07 m
t=160.25 s | alt=0.00 m
```

The corrected profile contains:

```text
3265 samples
```

---

# Hardware Replay Tests

## Test 1 - low_apogee

Command:

```powershell
python tools\send_profile_serial.py test_profiles\low_apogee.csv --port COM5 --speed 1
```

Result:

```text
[PC] Waiting for ESP32 ready message...
[REPLAY] READY
[PC] ESP32 ready. Starting replay.
[PC] Sending PROFILE_START
[REPLAY] START
[FLIGHT] State -> BOOST
[FLIGHT] State -> COAST
[FLIGHT] State -> APOGEE
[FLIGHT] State -> DESCENT
[FLIGHT] State -> LANDED
[PC] Sending PROFILE_END
[REPLAY] END | samples=491

[PC] Done.
```

Status:

```text
PASS
```

---

## Test 2 - midpower_machdip

Command:

```powershell
python tools\send_profile_serial.py test_profiles\midpower_machdip.csv --port COM5 --speed 5
```

Result:

```text
[PC] Sending PROFILE_START
[REPLAY] START
[FLIGHT] State -> BOOST
[FLIGHT] State -> COAST
[FLIGHT] State -> APOGEE
[FLIGHT] State -> DESCENT
[FLIGHT] State -> LANDED
[PC] Sending PROFILE_END
[REPLAY] END | samples=3265

[PC] Done.
```

Status:

```text
PASS
```

The corrected mach-dip profile replayed completely and triggered the full flight state sequence on the ESP32.

---

# Native Test Regression

After correcting the profile, the native test suite was executed again.

Command:

```powershell
pio test -e native
```

Result:

```text
RealFlightProfiles/FlightProfileTest.DetectsApogeeWithinTolerance/estes_c6          [PASSED]
RealFlightProfiles/FlightProfileTest.DetectsApogeeWithinTolerance/midpower_machdip  [PASSED]
RealFlightProfiles/FlightProfileTest.DetectsApogeeWithinTolerance/low_apogee        [PASSED]

FlightStateMachine.FullFlightSequence              [PASSED]
FlightStateMachine.IgnoresBarometricNoiseAtIdle    [PASSED]
FlightStateMachine.DoesNotLaunchOnSlowRise         [PASSED]
FlightStateMachine.LaunchesOnFastRise              [PASSED]
FlightStateMachine.ApogeeAltitudeMatchesPeak       [PASSED]
FlightStateMachine.ResetReturnsToIdle              [PASSED]

9 test cases: 9 succeeded
```

Status:

```text
PASS
```

---

# Results Summary

| Test | Expected | Actual | Status |
|------|----------|--------|--------|
| `low_apogee` hardware replay | 491 samples | 491 samples | PASS |
| `midpower_machdip` hardware replay | 3265 samples | 3265 samples | PASS |
| Full state sequence on hardware | BOOST -> COAST -> APOGEE -> DESCENT -> LANDED | Detected | PASS |
| Native replay tests | 9 passing | 9 passing | PASS |

---

# Validation Impact

Sprint 14b improves validation depth by adding a third validation layer:

```text
1. Unit-level FSM tests on PC
2. CSV profile replay tests on PC
3. Serial hardware replay on ESP32
```

This makes the project more robust before adding recovery deployment hardware and before attempting real field tests.

---

# Known Limitations

- Serial replay is a bench-testing mode, not a flight mode
- MAVLink is disabled during serial replay because USB Serial is used for replay commands and debug output
- Current replay input uses altitude only
- Real flight validation is still pending
- Servo and other actuators remain disabled during replay by default

---

# Conclusion

Sprint 14b successfully extended the replay testing framework to real ESP32 hardware.

Both `low_apogee` and corrected `midpower_machdip` were streamed over USB serial and replayed successfully by the ESP32. The firmware detected the complete flight sequence in hardware:

```text
BOOST -> COAST -> APOGEE -> DESCENT -> LANDED
```

The corrected mach-dip profile now descends continuously to ground level and remains valid in both native tests and hardware replay.

**Sprint Status:** COMPLETED

**Recommended Release:** v0.14.1
