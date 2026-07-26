# 🚀 Flight Telemetry & Data Logger

ESP32-based flight computer featuring GPS telemetry, altitude estimation, battery monitoring, MAVLink integration and fault-tolerant flight data logging.

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
![MAVLink](https://img.shields.io/badge/MAVLink-Integrated-success)
![Status](https://img.shields.io/badge/Status-Sprint10.0-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Enabled-success)

---

# Overview

Flight Telemetry & Data Logger is a modular ESP32-based flight computer designed for telemetry acquisition, altitude estimation, power monitoring and reliable flight data recording.

The project combines:

- GPS telemetry
- Barometric altitude estimation
- Battery monitoring
- MAVLink integration
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

## MAVLink Foundation

✅ MAVLink Library Integration

✅ MAVLinkTelemetry Module

✅ HEARTBEAT Message Generation

✅ Runtime Validation Successful

✅ No Regression Detected

✅ Stable Integration With Existing Subsystems

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

[EVENT] BATTERY_DISCONNECTED

[EVENT] SD_REMOVED

[EVENT] SD_RECOVERED
```

---

# MAVLink Foundation

Current MAVLink implementation:

```text
MAVLink Library Integrated

MAVLinkTelemetry Module

HEARTBEAT Message

System Validation Successful
```

Validated runtime output:

```text
[MAVLINK] Initialized

[MAVLINK] HEARTBEAT sent
```

Future MAVLink messages:

```text
GPS_RAW_INT

BATTERY_STATUS

GLOBAL_POSITION_INT

SYS_STATUS
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

✅ MAVLink Foundation Validation

✅ HEARTBEAT Validation

✅ No System Regression

---

# Current Status

```text
Latest Stable Release

v0.9.2
Battery Telemetry & Event System
```

Current Development:

```text
Sprint 10.0

MAVLink Foundation

HEARTBEAT Validated

Project Stable
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
Sprint 10.1

GPS_RAW_INT

Latitude
Longitude
GPS Altitude
GPS Fix
```

```text
Sprint 10.2

BATTERY_STATUS

Battery Voltage
Current
Remaining %
```

```text
Sprint 10.3

GLOBAL_POSITION_INT

Flight Altitude
Ground Speed
```

```text
Sprint 10.4

SYS_STATUS
```

---

# License

MIT License