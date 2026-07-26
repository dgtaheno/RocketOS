# Test Report – Sprint 9

## Project

Flight Telemetry & Data Logger

Platform:

```text
ESP32 DevKitC V4
BMP388
u-blox NEO-M9N
MicroSD Storage
INA219 Current Sensor
MP1584 Buck Converter
Tattu LiPo 4S Battery
```

Date:

```text
2026-07-26
```

---

# Objective

Validate the Sprint 9 power architecture, INA219 integration, battery monitoring subsystem, battery event system and power telemetry functionality under real hardware conditions.

---

# Test Environment

## Hardware

```text
ESP32 DevKitC V4

BMP388
u-blox NEO-M9N GPS

MicroSD Module

INA219 Current Sensor

MP1584 Buck Converter

Tattu LiPo 4S
14.8V Nominal
16.8V Full Charge
```

## Firmware

```text
v0.9.2
```

Validated Modules:

```text
BMP388Sensor
GPSSensor
SDLogger
SystemHealth
SystemEvents
BufferedLogger
INA219Sensor
BatteryMonitor
```

---

# Summary

| Test ID | Description | Result |
|----------|------------|----------|
| T90 | INA219 Detection | PASS |
| T91 | Battery Voltage Monitoring | PASS |
| T92 | Current Measurement | PASS |
| T93 | Power Measurement | PASS |
| T94 | Battery Connected Detection | PASS |
| T95 | Battery Disconnect Detection | PASS |
| T96 | Battery Hot-Plug Validation | PASS |
| T97 | Battery State Monitoring | PASS |
| T98 | Battery SOC Estimation | PASS |
| T99 | USB + LiPo Dual-Power Validation | PASS |
| T100 | Battery Connected Event | PASS |
| T101 | Battery Disconnected Event | PASS |
| T102 | Battery Telemetry CSV Logging | PASS |
| T103 | Battery Event Validation | PASS |

---

# Power Monitoring Validation

## T90 – INA219 Detection

### Procedure

```text
Run POST
```

### Expected

```text
INA219 sensor detected
```

### Result

```text
PASS
```

Observed:

```text
[PASS] INA219 sensor
```

---

## T91 – Battery Voltage Monitoring

### Procedure

```text
Connect LiPo battery

Run system normally
```

### Expected

```text
Battery voltage reported
```

### Result

```text
PASS
```

Observed:

```text
Battery V : 15.00 - 15.12 V
```

### Conclusion

```text
Battery voltage measurement
operates correctly and remains stable.
```

---

## T92 – Current Measurement

### Procedure

```text
Connect LiPo battery

Observe telemetry
```

### Expected

```text
Current measurement available
```

### Result

```text
PASS
```

Observed:

```text
Current : 25 - 52 mA
```

### Conclusion

```text
INA219 current measurement
operates correctly.
```

---

## T93 – Power Measurement

### Procedure

```text
Connect LiPo battery

Observe telemetry
```

### Expected

```text
Power consumption reported
```

### Result

```text
PASS
```

Observed:

```text
Power : 380 - 780 mW
```

### Conclusion

```text
Real-time power monitoring
operates correctly.
```

---

# Battery Monitoring Validation

## T94 – Battery Connected Detection

### Procedure

```text
Connect LiPo battery
```

### Expected

```text
Battery Conn : YES
```

### Result

```text
PASS
```

Observed:

```text
Battery Conn : YES
```

### Conclusion

```text
Battery presence correctly detected.
```

---

## T95 – Battery Disconnect Detection

### Procedure

```text
Disconnect LiPo battery

Power system from USB only
```

### Expected

```text
Battery Conn : NO

Battery St. : DISCONNECTED
```

### Result

```text
PASS
```

Observed:

```text
Battery V   : 4.1 V

Battery Conn: NO

Battery SOC : 0 %

Battery St. : DISCONNECTED
```

### Conclusion

```text
Battery absence correctly detected.
```

---

## T96 – Battery Hot-Plug Validation

### Procedure

```text
Start system using USB only

Connect LiPo while running
```

### Expected

```text
Battery transitions from

DISCONNECTED

to

CONNECTED

without reboot.
```

### Result

```text
PASS
```

Observed:

```text
Battery Conn : NO

↓

Battery Conn : YES
```

### Conclusion

```text
Hot-plug operation validated.

No reboot required.

No instability observed.
```

---

## T97 – Battery State Monitoring

### Procedure

```text
Operate system using LiPo battery
```

### Expected

```text
Battery state reported correctly
```

### Result

```text
PASS
```

Observed:

```text
Battery St. : OK
```

### Conclusion

```text
Battery state monitoring
operates correctly.
```

---

## T98 – Battery SOC Estimation

### Procedure

```text
Operate system using LiPo battery
```

### Expected

```text
SOC estimation available
```

### Result

```text
PASS
```

Observed:

```text
Battery SOC : 54 - 58 %
```

### Conclusion

```text
SOC estimation operates correctly
using the LiPo 4S voltage model.
```

---

## T99 – USB + LiPo Dual-Power Validation

### Procedure

```text
Connect USB

Connect LiPo

Operate system normally
```

### Expected

```text
System remains healthy

No fault flags generated
```

### Result

```text
PASS
```

Observed:

```text
System Healthy : YES

Fault Flags : 0x00
```

### Conclusion

```text
Dual-power architecture validated.
```

---

# Battery Event System Validation

## T100 – Battery Connected Event

### Procedure

```text
Start system on USB power

Connect LiPo battery
```

### Expected

```text
BATTERY_CONNECTED event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] BATTERY_CONNECTED

Battery Conn : YES

Battery St. : OK
```

### Conclusion

```text
Battery connection event generated correctly.
```

---

## T101 – Battery Disconnected Event

### Procedure

```text
Operate system using LiPo battery

Disconnect LiPo battery
```

### Expected

```text
BATTERY_DISCONNECTED event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] BATTERY_DISCONNECTED

Battery Conn : NO

Battery St. : DISCONNECTED
```

### Conclusion

```text
Battery disconnection event generated correctly.
```

---

## T102 – Battery Telemetry CSV Logging

### Procedure

```text
Operate system normally

Record telemetry

Inspect generated CSV file
```

### Expected

```text
Battery telemetry saved to CSV
```

### Result

```text
PASS
```

Observed:

```csv
timestamp_s,
temperature_c,
pressure_hpa,
bmp_altitude_m,
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

### Conclusion

```text
Battery telemetry successfully recorded.
```

---

## T103 – Battery Event Validation

### Procedure

```text
Connect battery

Disconnect battery

Observe event stream
```

### Expected

```text
Single event generated per transition
No event spam
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] BATTERY_CONNECTED

...

[EVENT] BATTERY_DISCONNECTED
```

### Conclusion

```text
Event state machine validated.

Anti-spam behaviour operates correctly.
```

---

# Architecture Validation

Validated Hardware:

```text
ESP32 DevKitC V4

BMP388

u-blox NEO-M9N

INA219 Current Sensor

MicroSD Storage

MP1584 Buck Converter

Tattu LiPo 4S
```

Result:

```text
PASS
```

---

# Known Issues

```text
No known open issues.
```

---

# Conclusions

Sprint 9 objectives successfully validated.

Validated on real hardware:

```text
INA219 Detection

Battery Voltage Monitoring

Current Measurement

Power Measurement

Battery Connected Detection

Battery Disconnect Detection

Battery Hot-Plug Validation

Battery State Monitoring

Battery SOC Estimation

Battery Telemetry CSV Logging

Battery Connected Event

Battery Disconnected Event

Battery Event Validation

USB + LiPo Dual-Power Validation

Power Architecture Validation
```

Overall Result:

```text
SPRINT 9.0 PASSED

SPRINT 9.1 PASSED

SPRINT 9.2A PASSED

SPRINT 9.2B PASSED

SPRINT 9.3 PASSED

SPRINT 9.4 PASSED

NO OPEN ISSUES
```

---

# Next Sprint

```text
Sprint 10

- MAVLink Foundation
- HEARTBEAT
- SYS_STATUS
- GPS_RAW_INT
- GLOBAL_POSITION_INT
- BATTERY_STATUS
```

Future:

```text
Sprint 11
- LoRa Telemetry

Sprint 12
- Ground Station

Sprint 13
- FreeRTOS Architecture
```