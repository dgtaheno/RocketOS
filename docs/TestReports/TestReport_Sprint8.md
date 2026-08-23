# Test Report – Sprint 8

## Project

Flight Telemetry & Data Logger

Platform:

```text
ESP32 DevKitC V4
BMP388
u-blox NEO-M9N
MicroSD Storage
```

Date:

```text
2026-07-11
```

---

# Objective

Validate the Health Monitoring subsystem, SD diagnostics, Runtime Event System, SD State Machine and configuration parameters under real hardware conditions.

---

# Test Environment

## Hardware

```text
ESP32 DevKitC V4
BMP388
u-blox NEO-M9N
MicroSD Card
```

## Firmware

```text
Sprint 8.1B
```

Validated Modules:

```text
BMP388Sensor
GPSSensor
SDLogger
SystemHealth
SystemEvents
```

---

# Summary

| Test ID | Description | Result |
|----------|------------|----------|
| T01 | Normal Startup | PASS |
| T02 | BMP388 Detection | PASS |
| T03 | GPS Detection | PASS |
| T04 | SD Detection | PASS |
| T05 | Flight Log Creation | PASS |
| T06 | GPS Timestamped Filename | PASS |
| T07 | Runtime Health Monitoring | PASS |
| T08 | Fault Flags | PASS |
| T09 | SD Diagnostics | PASS |
| T10 | SD Write Failure Detection | PASS |
| T11 | SD Removal Detection | PASS |
| T12 | SD Recovery Detection | PASS |
| T13 | SD State Machine | PASS |
| T14 | Recovery Statistics | PASS |
| T15 | GPS Speed Deadband | PASS |
| T20 | Startup without SD | PASS |
| T21 | Startup without GPS Power | PASS |
| T22 | Startup without BMP388 | PASS |
| T50 | CSV Recovery Validation | PASS |

---

# Test Cases

---

## T01 – Normal Startup

### Procedure

```text
Power cycle system
Allow POST to execute
```

### Expected

```text
All modules pass self-test
System READY
```

### Result

```text
PASS
```

Observed:

```text
[PASS] BMP388 sensor
[PASS] SD card
[PASS] GPS receiver

System READY
```

---

## T02 – BMP388 Detection

### Procedure

```text
Run POST
```

### Expected

```text
BMP388 detected
```

### Result

```text
PASS
```

Observed:

```text
[PASS] BMP388 sensor
```

---

## T03 – GPS Detection

### Procedure

```text
Run POST
```

### Expected

```text
GPS receiver detected
```

### Result

```text
PASS
```

Observed:

```text
[PASS] GPS receiver
```

---

## T04 – SD Detection

### Procedure

```text
Run POST
```

### Expected

```text
SD card detected
```

### Result

```text
PASS
```

Observed:

```text
[PASS] SD card
```

---

## T05 – Flight Log Creation

### Procedure

```text
Start system with GPS FIX
```

### Expected

```text
Flight log created
```

### Result

```text
PASS
```

Observed:

```text
flight_YYYY-MM-DD_HH-MM-SS.csv
```

Example:

```text
flight_2026-07-11_23-01-48.csv
```

---

## T06 – GPS Timestamped Filename

### Procedure

```text
Acquire GPS FIX
Create flight log
```

### Expected

```text
Filename contains GPS date/time
```

### Result

```text
PASS
```

---

## T07 – Runtime Health Monitoring

### Procedure

```text
Operate system normally
```

### Expected

```text
All modules report OK
```

### Result

```text
PASS
```

Observed:

```text
GPS : OK
BMP : OK
SD  : OK

System Healthy : YES
```

---

## T08 – Fault Flags

### Procedure

```text
Run system normally
Remove SD card
```

### Expected

```text
SD fault flag set
```

### Result

```text
PASS
```

Observed:

```text
Fault Flags : 0x04
```

---

## T09 – SD Diagnostics

### Procedure

```text
Observe storage monitoring
```

### Expected

```text
Capacity values displayed
```

### Result

```text
PASS
```

Observed:

```text
SD Total
SD Used
SD Free
SD Usage %
```

---

# Runtime Event System

---

## T10 – SD Write Failure Detection

### Procedure

```text
Remove SD card during operation
```

### Expected

```text
SD write failure event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] SD_WRITE_FAILED
```

---

## T11 – SD Removal Detection

### Procedure

```text
Remove SD card
Leave removed for >30 seconds
```

### Expected

```text
SD_REMOVED event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] SD_REMOVED
```

---

## T12 – SD Recovery Detection

### Procedure

```text
Reinsert SD card
```

### Expected

```text
SD recovery event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] SD_INSERTED

[EVENT] SD_RECOVERED
```

---

## T13 – SD State Machine

### Procedure

```text
Remove SD
Wait
Reinsert SD
```

### Expected

```text
OK
↓
WRITE_FAILED
↓
REMOVED
↓
RECOVERED
↓
OK
```

### Result

```text
PASS
```

Observed:

```text
State : WRITE_FAILED

State : REMOVED

State : OK
```

---

## T14 – Recovery Statistics

### Procedure

```text
Perform SD removal and reinsertion
```

### Expected

```text
Counters updated
```

### Result

```text
PASS
```

Observed:

```text
Removal Count  : 1
Recovery Count : 1
Write Failures : 6
```

---

## T15 – GPS Speed Deadband

### Procedure

```text
Leave system stationary
```

### Expected

```text
Speed = 0.00 km/h
```

### Result

```text
PASS
```

Observed:

```text
Speed : 0.00 km/h
```

---

# Startup Validation Tests

---

## T20 – Startup without SD

### Procedure

```text
Remove SD card
Power cycle system
```

### Expected

```text
SD card not detected
```

### Result

```text
PASS
```

Observed:

```text
[FAIL] SD card not detected

Please check:
 - microSD card is inserted
 - SD module wiring
 - SD module power supply (5V)
 - SD card format (FAT32)
```

### Conclusion

```text
POST correctly detects SD absence.
```

---

## T21 – Startup without GPS Power

### Procedure

```text
Disconnect GPS VCC
Power cycle system
```

### Expected

```text
GPS receiver not detected

GPS Detected = NO
```

### Result

```text
PASS
```

Observed:

```text
[WARN] GPS receiver not detected

[WARN] GPS fix unavailable

GPS Detected : NO

GPS Fix      : NO

Flight log created

Fallback filename used
```

### Conclusion

```text
GPS absence correctly detected.

System remains operational
using degraded mode.
```

---

## T22 – Startup without BMP388

### Procedure

```text
Disconnect BMP388
Power cycle system
```

### Expected

```text
BMP388 self-test fails
```

### Result

```text
PASS
```

Observed:

```text
[Wire.cpp:499] requestFrom(): i2cWriteReadNonStop returned Error 263

[FAIL] BMP388 self-test failed
```

### Conclusion

```text
BMP388 absence correctly detected.

I2C communication loss correctly triggers self-test failure.
```

---

# Recovery Validation Tests

---

## T50 – CSV Recovery Validation

### Procedure

```text
Create log

Remove SD card

Wait approximately 40 seconds

Reinsert SD card

Continue logging

Open CSV file
```

### Expected

```text
Logging resumes after SD recovery.
```

### Result

```text
PASS
```

### Observed

```text
Timestamp sequence:

24

...

65

...

106
```

### Conclusion

```text
CSV logging resumes correctly after SD recovery.

Records generated while SD storage
was unavailable are lost.

Buffered logging is required
to prevent data loss.
```

---

# Configuration Validation

The following configuration parameters defined in Config.h have been validated using real hardware tests.

---

## C01 – GPS Detection Timeout

### Parameter

```text
GPS_DETECTION_TIMEOUT_MS
```

### Validation

```text
Startup with GPS disconnected.
```

### Result

```text
PASS
```

Observed:

```text
[WARN] GPS receiver not detected

GPS Detected : NO

GPS Fix      : NO
```

---

## C02 – GPS Stale Data Timeout

### Parameter

```text
GPS_STALE_DATA_TIMEOUT_MS
```

### Validation

```text
Disconnect GPS during operation.
```

### Result

```text
PASS
```

Observed:

```text
GPS Detected : NO

GPS Fix      : NO

GPS : WARNING

Error : NO_DATA
```

---

## C03 – GPS Fix Timeout

### Parameter

```text
GPS_FIX_TIMEOUT_SECONDS
```

### Validation

```text
Startup without GPS availability.
```

### Result

```text
PASS
```

Observed:

```text
[WARN] GPS fix unavailable

[INFO] Using fallback filename

flight_boot_014.csv
```

---

## C04 – SD Removal Threshold

### Parameter

```text
SD_REMOVAL_FAILURE_THRESHOLD
```

### Validation

```text
Remove SD card during operation.
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] SD_WRITE_FAILED

[EVENT] SD_REMOVED
```

---

## C05 – GPS Speed Deadband

### Parameter

```text
GPS_SPEED_DEADBAND_KMH
```

### Validation

```text
System stationary.
```

### Result

```text
PASS
```

Observed:

```text
Speed : 0.00 km/h
```

---

# Known Issues

```text
No known open issues.
```

---

# Conclusions

Sprint 8.0 objectives successfully validated.

Validated in real hardware:

```text
POST
Health Monitoring
Fault Flags
SD Diagnostics
Runtime Event System
SD State Machine
SD Write Failure Detection
SD Removal Detection
SD Recovery Detection
Recovery Statistics
GPS Runtime Loss Detection
GPS Startup Detection
CSV Recovery Validation
```

Configuration validation completed:

```text
GPS Detection Timeout
GPS Stale Data Timeout
GPS Fix Timeout
SD Removal Threshold
GPS Speed Deadband
```

Overall Result:

```text
SPRINT 8.0 PASSED

SPRINT 8.1A PASSED

SPRINT 8.1B PASSED

NO OPEN ISSUES
```

---

# Next Sprint

```text
Sprint 8.2

- Buffered Logging
- Circular Buffer
- Pending Records Counter
- Automatic SD Flush
- Automatic SD Recovery Storage
```

Future:

```text
Sprint 9

- LoRa Telemetry
- Ground Station Integration

Sprint 10

- INA219 Power Monitoring

Sprint 11

- Flight Analytics

Sprint 12

- FreeRTOS Architecture
```