# 🚀 Flight Telemetry & Data Logger

ESP32-based flight computer featuring GPS telemetry, altitude estimation, battery monitoring, MAVLink telemetry, ground station instrumentation and fault-tolerant flight data logging.

![Project Banner](docs/images/banner.png)

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![GPS](https://img.shields.io/badge/GPS-NEO--M9N-green)
![Sensor](https://img.shields.io/badge/Sensor-BMP388-orange)
![Power](https://img.shields.io/badge/Power-INA219-yellow)
![Battery](https://img.shields.io/badge/Battery-LiPo_4S-red)
![MAVLink](https://img.shields.io/badge/MAVLink-Telemetry-success)
![QGroundControl](https://img.shields.io/badge/QGroundControl-Validated-success)
![Status](https://img.shields.io/badge/Status-Sprint11.4-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Enabled-success)

---

# Real Hardware Prototype

Hardware platform used during validation testing.

![Flight Telemetry Prototype](docs/images/flighttelemetry-v090.png)

---

# Hardware

| Component | Model | Function |
|------------|--------|----------|
| Microcontroller | ESP32 DevKitC V4 | Flight computer |
| GNSS Receiver | u-blox NEO-M9N | Position, altitude, UTC time |
| Barometric Sensor | BMP388 | Pressure, temperature, relative altitude |
| Power Monitor | INA219 | Voltage, current, power |
| Storage | MicroSD Module | Flight data logging |
| Power Supply | MP1584 Buck Converter | Regulated 5V rail |
| Battery | Tattu LiPo 4S | System power source |

<table>
  <tr>
    <td align="center"><img src="docs/images/M9N.png" width="150"><br><b>NEO-M9N</b></td>
    <td align="center"><img src="docs/images/BMP388.png" width="150"><br><b>BMP388</b></td>
    <td align="center"><img src="docs/images/INA219.png" width="150"><br><b>INA219</b></td>
  </tr>
  <tr>
    <td align="center"><img src="docs/images/microsd.png" width="150"><br><b>MicroSD</b></td>
    <td align="center"><img src="docs/images/MP1584.png" width="150"><br><b>MP1584</b></td>
    <td align="center"><img src="docs/images/Tattu4S.png" width="150"><br><b>Tattu LiPo 4S</b></td>
  </tr>
</table>

---

# Overview

Flight Telemetry & Data Logger is a modular ESP32-based flight computer designed for telemetry acquisition, altitude estimation, power monitoring, ground station interoperability and reliable flight data recording.

The project combines:

- GPS telemetry
- Barometric altitude estimation
- Battery monitoring
- MAVLink telemetry
- Ground station instrumentation
- Runtime health monitoring
- Event-driven diagnostics
- Fault-tolerant SD logging
- Buffered telemetry recovery

All features included in this repository have been validated on real hardware.

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

✅ Climb Rate Estimation

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

## MAVLink Telemetry

✅ HEARTBEAT

✅ GPS_RAW_INT

✅ BATTERY_STATUS

✅ GLOBAL_POSITION_INT

✅ GLOBAL_POSITION_INT_COV

✅ SYS_STATUS

✅ VFR_HUD

✅ QGroundControl Integration

---

# MAVLink Telemetry Stack

Current MAVLink implementation validated on real hardware:

| Message | ID | Purpose | Status |
|----------|-----|---------|---------|
| HEARTBEAT | 0 | Vehicle presence and link status | ✅ |
| SYS_STATUS | 1 | System and battery status | ✅ |
| GPS_RAW_INT | 24 | Raw GNSS position and fix type | ✅ |
| GLOBAL_POSITION_INT | 33 | Position and relative altitude | ✅ |
| GLOBAL_POSITION_INT_COV | 63 | Position with covariance | ✅ |
| VFR_HUD | 74 | Ground speed, altitude, climb rate | ✅ |
| BATTERY_STATUS | 147 | Battery voltage, current and SOC | ✅ |

Runtime validation confirmed:

```text
Vehicle Detected by QGroundControl

Battery Connected

GPS Fix Acquired

Relative Altitude Reported

Ground Speed Reported

Climb Rate Reported

System Healthy = YES

Fault Flags = 0x00

No Brownout Detected

No Watchdog Resets

No System Regression
```

---

# QGroundControl Validation

Sprint 11 introduced Ground Control Station compatibility through the MAVLink protocol.

The Flight Telemetry & Data Logger successfully establishes a live connection with QGroundControl running on a ground station PC.

Validated functionality:

✅ Vehicle Detection

✅ MAVLink Communication

✅ GPS Telemetry Reception

✅ Battery Status Reception

✅ System Status Reception

✅ Relative Altitude Instrumentation

✅ Ground Speed Instrumentation

✅ Climb Rate Instrumentation

✅ Real-Time Telemetry Streaming

![QGroundControl Validation](docs/images/qgroundcontrol-validation.png)

*QGroundControl establishing a live MAVLink connection with the ESP32 flight computer.*

![QGroundControl Flight Instruments](docs/images/qgc-flight-instruments.png)

*Flight instrumentation reported through GLOBAL_POSITION_INT and VFR_HUD: relative altitude, climb rate and ground speed. Residual values are consistent with BMP388 sensor noise under stationary conditions.*

| Instrument | Source Message | Field | Reported |
|-------------|----------------|--------|----------|
| Relative Altitude | GLOBAL_POSITION_INT | relative_alt | 1.3 m |
| Climb Rate | VFR_HUD | climb | 0.0 m/s |
| Ground Speed | VFR_HUD | groundspeed | 0.0 m/s |
| Distance to Home | HOME_POSITION | not implemented | --.-- m |

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

# Generated Telemetry

```text
System Status

GPS Position

GPS Altitude

Flight Altitude

Relative Altitude

Ground Speed

Climb Rate

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

✅ MAVLink GLOBAL_POSITION_INT Validation

✅ MAVLink GLOBAL_POSITION_INT_COV Validation

✅ MAVLink SYS_STATUS Validation

✅ MAVLink VFR_HUD Validation

✅ Relative Altitude Validation

✅ Climb Rate Validation

✅ Ground Station Instrument Validation

✅ QGroundControl Connection Validation

✅ MAVLink End-to-End Telemetry Validation

✅ No System Regression

---

# Current Status

```text
Current Development Stage

Sprint 11.4

Flight Instrumentation

Validated on Real Hardware
```

Current capabilities:

```text
GPS Telemetry

Barometric Altitude Estimation

Climb Rate Estimation

Battery Monitoring

System Health Monitoring

Fault-Tolerant SD Logging

MAVLink Telemetry

Ground Station Instrumentation

QGroundControl Connectivity
```

---

# Documentation

```text
docs/TestReport_Sprint8.md

docs/TestReport_Sprint9.md

docs/TestReport_Sprint10.md

docs/TestReport_Sprint11.md

docs/schematics/

docs/images/
```

---

# Next Milestones

```text
HOME_POSITION

Flight Event Detection

Launch & Apogee Algorithms

FreeRTOS Architecture

Long-Range Telemetry

PX4 Interoperability

ArduPilot Interoperability
```

---

# License

MIT License
