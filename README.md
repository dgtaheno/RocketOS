# 🚀 Flight Telemetry & Data Logger

ESP32-based flight computer featuring GPS telemetry, altitude estimation, battery monitoring, MAVLink telemetry and fault-tolerant flight data logging.

![Project Banner](docs/images/banner.png)

---

# Real Hardware Prototype

Hardware platform used during validation testing.

- ESP32 DevKitC V4
- u-blox NEO-M9N GPS
- BMP388 Barometric Sensor
- INA219 Power Monitor
- MicroSD Storage
- MP1584 Buck Converter
- Tattu LiPo 4S Battery

![Flight Telemetry Prototype](docs/images/flighttelemetry-v090.png)

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![GPS](https://img.shields.io/badge/GPS-NEO--M9N-green)
![Sensor](https://img.shields.io/badge/Sensor-BMP388-orange)
![Power](https://img.shields.io/badge/Power-INA219-yellow)
![Battery](https://img.shields.io/badge/Battery-LiPo_4S-red)
![MAVLink](https://img.shields.io/badge/MAVLink-Telemetry-success)
![Status](https://img.shields.io/badge/Status-Sprint10.4-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Enabled-success)

---

# Overview

Flight Telemetry & Data Logger is a modular ESP32-based flight computer designed for telemetry acquisition, altitude estimation, power monitoring and reliable flight data recording.

The project combines:

- GPS telemetry
- Barometric altitude estimation
- Battery monitoring
- MAVLink telemetry
- Runtime health monitoring
- Event-driven diagnostics
- Fault-tolerant SD logging
- Buffered telemetry recovery

All features included in this repository have been validated on real hardware.

---

# Hardware

```text
ESP32 DevKitC V4

u-blox NEO-M9N GPS

BMP388 Barometric Sensor

INA219 Power Monitor

MicroSD Storage Module

MP1584 Buck Converter

Tattu LiPo 4S Battery
```

---

# Features

## Telemetry

✅ GPS Position

✅ GPS Altitude

✅ Ground Speed

✅ UTC Time Synchronization

✅ GPS Timestamped Logging

✅ GPS Fix Monitoring

---

## Altitude System

✅ BMP388 Pressure Measurement

✅ Temperature Measurement

✅ Relative Altitude Estimation

✅ Flight Altitude Calculation

✅ GPS Speed Deadband Filtering

---

## Power Monitoring

✅ INA219 Integration

✅ Battery Voltage Monitoring

✅ Current Measurement

✅ Power Consumption Monitoring

✅ Battery SOC Estimation

✅ Battery Connection Detection

✅ Battery State Monitoring

✅ Battery Event System

✅ Battery Hot-Plug Detection

---

## Logging

✅ Automatic CSV Logging

✅ GPS Timestamped Filenames

✅ Buffered Logging

✅ Automatic Buffer Flush

✅ FIFO Preservation

✅ SD Recovery Logging

✅ Fault-Tolerant Telemetry Storage

---

## Reliability

✅ Power-On Self Test (POST)

✅ Runtime Health Monitoring

✅ Fault Flags

✅ Error Counters

✅ Runtime Event System

✅ SD State Machine

✅ SD Removal Detection

✅ SD Recovery Detection

---

# MAVLink Telemetry Stack

Current MAVLink implementation validated on hardware:

✅ HEARTBEAT

✅ GPS_RAW_INT

✅ BATTERY_STATUS

✅ GLOBAL_POSITION_INT_COV

✅ SYS_STATUS

Runtime validation confirmed:

```text
Battery Connected

GPS Fix Acquired

System Healthy = YES

Fault Flags = 0x00

No Brownout Detected

No Watchdog Resets

No System Regression
```

---

# System Architecture

```text
ESP32 DevKitC V4
│
├── BMP388Sensor
├── GPSSensor
├── INA219Sensor
├── BatteryMonitor
├── SDLogger
├── BufferedLogger
├── SystemHealth
├── SystemEvents
├── MAVLinkTelemetry
└── Flight Logger
```

---

# Runtime Event System

Supported events:

```text
SYSTEM_START
SYSTEM_READY

GPS_DETECTED
GPS_FIX_ACQUIRED
GPS_FIX_LOST

SD_DETECTED
SD_REMOVED
SD_INSERTED
SD_RECOVERED

BATTERY_CONNECTED
BATTERY_DISCONNECTED

BATTERY_WARNING
BATTERY_CRITICAL

BUFFER_FLUSH_COMPLETED
```

Example:

```text
[EVENT] BATTERY_CONNECTED

[EVENT] GPS_FIX_ACQUIRED

[EVENT] SD_RECOVERED
```

---

# MAVLink Messages

Implemented and validated:

```text
HEARTBEAT

GPS_RAW_INT

BATTERY_STATUS

GLOBAL_POSITION_INT_COV

SYS_STATUS
```

Generated telemetry includes:

```text
System Status

GPS Position

GPS Altitude

Flight Altitude

Battery Voltage

Battery Current

Battery Remaining %

System Health State
```

---

# CSV Format

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

---

# Validation Status

Validated on real hardware:

✅ GPS Validation

✅ BMP388 Validation

✅ SD Logging Validation

✅ Buffered Logging Validation

✅ FIFO Recovery Validation

✅ SD Recovery Validation

✅ Runtime Health Monitoring Validation

✅ Event System Validation

✅ INA219 Validation

✅ Battery Voltage Validation

✅ Current Measurement Validation

✅ Power Monitoring Validation

✅ Battery SOC Validation

✅ Battery Event Validation

✅ Battery Hot-Plug Validation

✅ CSV Battery Telemetry Validation

✅ MAVLink HEARTBEAT Validation

✅ MAVLink GPS_RAW_INT Validation

✅ MAVLink BATTERY_STATUS Validation

✅ MAVLink GLOBAL_POSITION_INT_COV Validation

✅ MAVLink SYS_STATUS Validation

✅ No System Regression

---

# Current Status

```text
Current Development Stage

Sprint 10.4

MAVLink Telemetry Pack

Validated on Real Hardware
```

Current capabilities:

```text
GPS Telemetry

Barometric Altitude Estimation

Battery Monitoring

System Health Monitoring

Fault-Tolerant SD Logging

MAVLink Telemetry Stack
```

---

# Documentation

```text
docs/TestReport_Sprint8.md

docs/TestReport_Sprint9.md

docs/TestReport_Sprint10.md

docs/schematics/

docs/images/
```

---

# Next Milestones

```text
Ground Station Integration

Mission Planner Validation

QGroundControl Validation

PX4 Interoperability

ArduPilot Interoperability

Telemetry Radio Integration
```

---

# License

MIT License