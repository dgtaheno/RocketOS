# TestReport_Sprint13a.md

# Flight Telemetry & Data Logger
## Sprint 13a – Environmental Sensing & Audible Event Feedback

**Project:** Flight Telemetry & Data Logger
**Repository:** flight-telemetry-data-logger
**Sprint:** 13a
**Release:** v0.13.0
**Date:** August 2026
**Author:** David Garcia-Taheño

---

# Objective

The objective of Sprint 13a was to extend the flight computer with environmental
sensing and audible event feedback, and to correct the flight-altitude
reference.

This sprint focused on:

- DS18B20 external ambient thermometer integration
- Active buzzer with a non-blocking driver and distinct event patterns
- Recording the detected flight state in the flight log
- Adding external temperature to the flight log
- Correcting the flight-altitude reference to remove pre-fix barometric drift
- KiCad schematic update to v0.13.0

---

# Hardware Configuration

| Component | Status |
|------------|---------|
| ESP32 DevKitC V4 | ✅ Operational |
| u-blox NEO-M9N GPS | ✅ Operational |
| BMP388 Barometer | ✅ Operational |
| DS18B20 External Thermometer | ✅ Operational |
| INA219 Current Sensor | ✅ Operational |
| Active Buzzer | ✅ Operational |
| MicroSD Module | ✅ Operational |
| MP1584 Buck Converter | ✅ Operational |
| Tattu 4S LiPo Battery | ✅ Operational |

## Pin Assignment

| Function | Pin |
|-----------|-----|
| I2C (BMP388, INA219) | GPIO21 / GPIO22 |
| SD SPI (CS) | GPIO5 |
| GPS UART | GPIO16 / GPIO17 |
| Buzzer | GPIO25 |
| DS18B20 (1-Wire) | GPIO4 |

The DS18B20 breakout board includes an onboard 4.7 kOhm pull-up, so no external
pull-up is required. The buzzer and the DS18B20 are both powered from 3.3 V so
their signal pins stay within the ESP32 GPIO range.

---

# Software Configuration

| Component | Version |
|------------|----------|
| PlatformIO | Latest |
| ESP32 Arduino Framework | Current |
| OneWire | ^2.3.7 |
| DallasTemperature | ^3.11.0 |
| QGroundControl | Installed and validated |
| Firmware Branch | sprint13a-buzzer-thermometer |

---

# Features Implemented

## 1. DS18B20 External Thermometer

### Description

A DS18B20 1-Wire sensor provides ambient (outside) temperature, independent of
the BMP388 internal temperature, which reads higher due to self-heating of the
electronics.

### Behaviour

The sensor is read every 5 seconds, since a 12-bit conversion takes about
750 ms. Ambient temperature changes slowly, so this rate is sufficient and
avoids stalling the main loop.

### Result

✅ External temperature read, shown on serial and logged to CSV

---

## 2. Active Buzzer

### Description

An active buzzer provides audible feedback for flight and system events using
distinct rhythmic patterns. The buzzer is single-tone; character comes from
timing.

### Patterns

| Event | Pattern |
|--------|---------|
| Startup | Power-up chime |
| GPS Lock | Ready chirp |
| Launch | Launch tone |
| Apogee | Peak alert (priority) |
| Landed | Recovery beacon (repeating) |
| Battery Critical | Warning stutter |

### Non-blocking Driver

The buzzer plays patterns in the background using a `millis()`-based state
machine. No `delay()` is used during flight operation, so telemetry, logging and
event detection are never stalled. The startup fanfare is played to completion
in `setup()`, before the main loop starts, so it is not interrupted by heavy
loop tasks.

Flight event sounds do not interrupt each other; apogee has priority and always
plays. A short silence at the end of each pattern separates consecutive sounds.

### Result

✅ All six patterns validated; startup plays cleanly; sounds do not overlap

---

## 3. Flight State in CSV

### Description

The detected flight state (IDLE, BOOST, COAST, APOGEE, DESCENT, LANDED) is now
recorded for every logged sample, enabling precise post-flight analysis of when
each event was detected relative to the altitude trace.

### Result

✅ `flight_state` column present and correct in the log

---

## 4. Flight Altitude Reference Fix

### Issue

Flight altitude was computed as `gps_reference_altitude + bmp_altitude`, using
the absolute barometric altitude. When the GPS fix was acquired minutes after
boot, the barometer had drifted, and the sum produced incorrect (even negative,
around -36 m) flight altitude values.

### Root Cause

The barometric relative altitude accumulates from boot. Adding it to the GPS
reference captured later mixed two references, carrying the pre-fix drift into
the result.

### Fix

The barometric altitude at the moment of the GPS fix is now captured as an
offset, and flight altitude is computed from the barometric change since then:

```text
flight_altitude = gps_reference_altitude + (bmp_altitude - bmp_altitude_at_fix)
```

The same offset is applied to the MAVLink relative altitude for consistency
between the CSV and the ground station.

### Result

✅ RESOLVED — flight altitude consistent with the GPS reference, no drift

---

# Functional Tests

## Test 1 – Power-On Self Test

### Procedure

Power up and observe POST.

### Expected Result

All subsystems, including the DS18B20, pass initialization.

### Actual Result

✅ PASS

```text
[PASS] BufferedLogger
[PASS] BMP388 sensor
[PASS] INA219 sensor
[PASS] DS18B20 external thermometer
[PASS] SD card
[PASS] GPS receiver
```

---

## Test 2 – External Temperature Reading

### Procedure

Observe external temperature on serial, then hold the probe by hand.

### Expected Result

Stable ambient reading that rises when the probe is warmed and falls when
released.

### Actual Result

✅ PASS

External temperature tracked from ~24 C up to ~29 C when held, and back down
when released, independent of the BMP388 internal temperature.

---

## Test 3 – Buzzer Patterns

### Procedure

Run the dedicated pattern test, which plays all six patterns in a loop.

### Expected Result

Each pattern is audible and distinct, with clear separation.

### Actual Result

✅ PASS

All six patterns played correctly and were distinguishable. The startup pattern
played cleanly (fluid, no stutter) when played to completion before the loop.

---

## Test 4 – Buzzer Non-blocking Behaviour

### Procedure

Run the full firmware and confirm telemetry and logging continue while sounds
play.

### Expected Result

No interruption of telemetry, logging or event detection while the buzzer plays.

### Actual Result

✅ PASS

Startup fanfare and GPS-lock chirp played while telemetry, logging and the state
machine continued without interruption.

---

## Test 5 – Flight State Logging

### Procedure

Inspect the CSV file for the `flight_state` column.

### Expected Result

Each row records the current flight state.

### Actual Result

✅ PASS

`flight_state` recorded as IDLE during ground testing, aligned with the state
machine output.

---

## Test 6 – Flight Altitude Reference

### Procedure

Boot indoors without fix, then acquire GPS fix, and observe flight altitude.

### Expected Result

Flight altitude close to GPS altitude after fix, with no negative jump.

### Actual Result

✅ PASS

```text
GPS Alt    : 42.5 m
Flight Alt : 41.06 m
```

Flight altitude stayed stable around 41 m and consistent with the GPS reference,
more stable than the raw GPS altitude. The previous negative-value behaviour did
not occur.

---

## Test 7 – Continuous Operation

### Procedure

Run the firmware for an extended period.

### Expected Result

Stable operation, no regression.

### Actual Result

✅ PASS

```text
Fault Flags   : 0x00
System Healthy: YES
```

No crashes, brownouts or watchdog resets. Existing telemetry, logging and
fault-tolerance behaviour were unaffected.

---

# Results Summary

| Test | Result |
|------|--------|
| Power-On Self Test | ✅ PASS |
| External Temperature Reading | ✅ PASS |
| Buzzer Patterns | ✅ PASS |
| Buzzer Non-blocking Behaviour | ✅ PASS |
| Flight State Logging | ✅ PASS |
| Flight Altitude Reference | ✅ PASS |
| Continuous Operation | ✅ PASS |

---

# CSV Format (16 columns)

```csv
timestamp_s,
temperature_c,
pressure_hpa,
bmp_altitude_m,
ext_temperature_c,
flight_state,
gps_fix,
latitude,
longitude,
gps_altitude_m,
flight_altitude_m,
speed_kmh,
battery_voltage_v,
current_ma,
power_mw,
battery_soc_percent
```

---

# Sprint Achievements

### Completed

- DS18B20 external thermometer integrated (serial + CSV)
- Active buzzer with non-blocking driver and six event patterns
- Startup fanfare played cleanly before the main loop
- Flight event sounds with apogee priority and no overlap
- flight_state recorded in the flight log
- External temperature recorded in the flight log
- Flight altitude reference corrected (no pre-fix drift)
- All audible patterns original with neutral names (no third-party marks)
- KiCad schematic updated to v0.13.0
- README updated
- Release v0.13.0 tagged

### Repository Status

- Build stable
- Environmental sensing operational
- Audible feedback operational
- Flight altitude reference correct
- No regression in existing functionality

---

# Known Limitations

- The DS18B20 conversion takes ~750 ms, so external temperature is sampled every
  5 s; adequate for ambient temperature
- Flight event sounds (launch, apogee, locator) can only be triggered in flight
  or via a lowered-threshold bench demo; they were validated with a dedicated
  pattern test on real hardware
- Field validation with a real launch has not yet been performed

---

# Conclusion

Sprint 13a extended the flight computer with environmental sensing and audible
event feedback, and corrected the flight-altitude reference.

The DS18B20 provides ambient temperature independent of the BMP388 internal
sensor, both logged to the CSV. The active buzzer signals startup, GPS lock,
launch, apogee and a repeating recovery beacon on landing, through a
non-blocking driver that never stalls telemetry or logging. The detected flight
state is now recorded for every sample, and the flight-altitude reference was
corrected so that flight altitude stays consistent with the GPS reference at
liftoff.

All features were validated on real hardware with no regression.

**Sprint Status:** COMPLETED

**Ready for Sprint 13b – Recovery Deployment Output**
