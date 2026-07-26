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

Validate the Sprint 9 power architecture, INA219 integration, battery monitoring subsystem and power telemetry functionality under real hardware conditions.

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
v0.9.1
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

---

# Test Cases

---

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
Observe runtime telemetry
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
Battery V : 15.10 V

Battery V : 15.11 V

Battery V : 15.12 V
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
Observe current telemetry
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
Current : 47.1 mA

Current : 49.5 mA

Current : 51.6 mA
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
Observe power telemetry
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
Power : 712 mW

Power : 744 mW

Power : 780 mW
```

### Conclusion

```text
Real-time power monitoring
operates correctly.
```

---

## T94 – Battery Connected Detection

### Procedure

```text
Connect LiPo battery
Observe BatteryMonitor
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

Power system using USB only
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
Battery V   : 4.13 V

Battery Conn: NO

Battery SOC : 0.0 %

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
Start system on USB power only

Connect LiPo while system remains running
```

### Expected

```text
Battery state transitions from
DISCONNECTED to CONNECTED
without system restart.
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
Battery hot-plug operation validated.

No reboot required.
No instability observed.
```

---

## T97 – Battery State Monitoring

### Procedure

```text
Operate system using LiPo battery
Observe reported battery state
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
Operate system with LiPo battery

Observe calculated SOC
```

### Expected

```text
SOC value calculated from
4-cell LiPo voltage curve
```

### Result

```text
PASS
```

Observed:

```text
Battery V   : 15.10 V

Battery SOC : 57.6 %
```

```text
Battery V   : 15.12 V

Battery SOC : 57.9 %
```

### Conclusion

```text
SOC calculation based on
LiPo 4S voltage curve
operates correctly.
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

# Power Monitoring Validation

Observed runtime telemetry:

```text
Battery V   : 15.10 - 15.12 V

Current     : 47 - 52 mA

Power       : 712 - 780 mW

Battery SOC : 57.6 - 57.9 %

Battery St. : OK
```

Result:

```text
PASS
```

---

# Architecture Validation

Validated Hardware:

```text
Tattu LiPo 4S

INA219 Current Sensor

MP1584 Buck Converter

ESP32 DevKitC V4

BMP388

u-blox NEO-M9N

MicroSD Storage
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

Validated in real hardware:

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

USB + LiPo Dual-Power Validation

Power Architecture Validation
```

Overall Result:

```text
SPRINT 9.0 PASSED

SPRINT 9.1 PASSED

SPRINT 9.2A PASSED

SPRINT 9.2B PASSED

NO OPEN ISSUES
```

---

# Next Sprint

```text
Sprint 9.3

- Battery Telemetry CSV Logging
- Battery Voltage Storage
- Current Logging
- Power Logging
- Battery SOC Logging
```

Future:

```text
Sprint 9.4
- Battery Events
- Battery Warnings
- Battery Critical State

Sprint 9.5
- MAVLink Battery Telemetry

Sprint 10
- LoRa Telemetry

Sprint 11
- Flight Analytics

Sprint 12
- FreeRTOS Architecture
```