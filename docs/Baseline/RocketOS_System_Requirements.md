# RocketOS System Requirements

Author: David Garcia-Taheño

Document status: Baseline Reconciled v3

## Purpose

This document defines the target requirements for RocketOS Mk1.

A documented requirement does not imply implementation.

Implementation status must be determined from:

- Repository code
- Test reports
- Validation records
- Approved decision records

---

# System Mission

RocketOS is an open-source flight computer, telemetry and data logging ecosystem for experimental rockets.

Its primary goals are:

- Reliable flight data acquisition
- Reliable onboard logging
- GPS tracking
- Flight-state detection
- Power monitoring
- Telemetry transmission
- Post-flight analysis
- Hardware and software modularity

---

# Target Platform

## Airframe

Target architecture:

- 54 mm rocket airframe

Status:

- Target defined
- Mechanical validation pending

---

## MCU

Target MCU:

- ESP32-S3 SuperMini N16R8

Status:

- Selected for Mk1
- Migration not yet validated

Current prototype:

- ESP32 DevKitC V4

---

## Power Source

Target power source:

- Fixed 2S LiPo
- 7.4 V nominal
- 8.4 V maximum
- Tattu 2S 550 mAh as the current development battery

Status:

- Selected and baselined for Mk1
- BatteryMonitor bench validated on 2026-08-28
- Complete Mk1 power subsystem validation pending

Historical prototype configurations must not be confused with the Mk1 architecture.

---

# Functional Requirements

## REQ-F-001 Sensor Acquisition

RocketOS shall acquire data from all active baseline sensors.

Current implemented sensors:

- BMP388
- INA219
- DS18B20
- NEO-M9N GPS

Planned:

- GY-87 IMU

Status:

- Partially Implemented

---

## REQ-F-002 Relative Altitude

RocketOS shall calculate relative altitude using barometric data.

Current implementation:

- BMP388
- Boot-relative reference

Status:

- Implemented
- Replay validated

---

## REQ-F-003 Flight-State Detection

RocketOS shall determine the current flight state.

Current implemented states:

- IDLE
- BOOST
- COAST
- APOGEE
- DESCENT
- LANDED

Status:

- Implemented
- Native test validated
- Replay validated

Real flight validation remains pending.

---

## REQ-F-004 Independent Logging

RocketOS shall continue onboard logging even when telemetry is unavailable.

Status:

- Implemented for current MAVLink serial telemetry
- Future LoRa validation pending

---

## REQ-F-005 SD Failure Recovery

RocketOS shall temporarily buffer data when SD storage becomes unavailable.

Current implementation:

- FIFO RAM buffer
- 100 records
- Ordered recovery

Status:

- Implemented

---

## REQ-F-006 Flight Data Recording

RocketOS shall store flight and system data on onboard storage.

Current implementation:

- CSV logging
- Automatic file creation
- GPS timestamp support

Status:

- Implemented

---

## REQ-F-007 Battery Monitoring

RocketOS shall monitor:

- Voltage
- Current
- Power

Current implementation:

- INA219
- Battery disconnected below 5.0 V
- 2S SOC curve: 6.60 V = 0% to 8.40 V = 100%
- Warning threshold: 7.20 V
- Critical threshold: 6.80 V

Status:

- Implemented
- Bench validated on 2026-08-28 after migration from 4S to 2S
- USB-only, LiPo disconnected: 4.10-4.13 V, `DISCONNECTED`
- 2S LiPo connected: 7.72-7.90 V, `CONNECTED` / `OK`

This validation applies to BatteryMonitor behavior on the current bench prototype. It does not establish that the Power Deck PCB has been fabricated or electrically validated.

---

## REQ-F-008 GPS Data

RocketOS shall acquire GPS information when valid GPS data is available.

Current implementation:

- Position
- Altitude
- Speed
- Time and date
- Home reference

Status:

- Implemented

---

## REQ-F-009 Audible Status

RocketOS shall provide audible status information.

Current implementation:

- Active buzzer

Patterns:

- Startup
- GPS lock
- Launch
- Apogee
- Landing
- Critical battery

Status:

- Implemented

---

## REQ-F-010 System Health Monitoring

RocketOS shall monitor subsystem health.

Current implementation:

- Power-on checks
- Health counters
- Event reporting

Status:

- Partially Implemented

---

## REQ-F-011 Telemetry Generation

RocketOS shall generate telemetry messages.

Current implementation:

- MAVLink over USB Serial

Messages:

- HEARTBEAT
- SYS_STATUS
- GPS_RAW_INT
- GLOBAL_POSITION_INT
- GLOBAL_POSITION_INT_COV
- VFR_HUD
- BATTERY_STATUS
- HOME_POSITION
- STATUSTEXT

Status:

- Implemented

---

## REQ-F-012 LoRa Telemetry

RocketOS Mk1 shall support radio telemetry.

Status:

- Planned

Not implemented.

Radio module selection remains open.

---

## REQ-F-013 Ground Station

RocketOS shall provide a dedicated Ground Station.

Status:

- Planned

Not implemented.

---

## REQ-F-014 Flight Analyzer

RocketOS shall support post-flight analysis.

Current support:

- CSV logs
- Replay tools

Dedicated Flight Analyzer:

- Planned

---

## REQ-F-015 Recovery Actuation

RocketOS Mk1 shall support recovery actuation.

Status:

- Planned

Current repository state:

- Placeholder files only

No implementation exists.

---

# Validation Requirements

## REQ-V-001 Native Automated Tests

The flight-state machine shall be testable through native automated tests.

Status:

- Implemented

Repository reports:

- 9 / 9 tests passing

---

## REQ-V-002 Flight Replay Validation

RocketOS shall support replay validation.

Current profiles:

- estes_c6
- low_apogee
- midpower_machdip

Status:

- Implemented

---

## REQ-V-003 Hardware Replay

RocketOS shall support replay execution on physical ESP32 hardware.

Status:

- Implemented

---

## REQ-V-004 Real Flight Validation

RocketOS shall be validated during real rocket flights before being considered flight validated.

Status:

- Pending

---

## REQ-V-005 Validation Before PCB Integration

Subsystems shall be validated before integration into custom PCBs.

Status:

- Active development rule

---

# Non-Functional Requirements

## Reliability

The system shall prioritize reliability over unnecessary complexity.

---

## Modularity

Subsystems should remain replaceable where practical.

---

## Maintainability

The system should remain understandable and maintainable by a hobbyist developer.

---

## Field Repairability

Modules and connections should allow field replacement whenever practical.

---

## Cost Awareness

Avoid unnecessary purchases and unnecessary custom hardware.

---

# Implemented Today

- Sensor acquisition
- Flight-state machine
- GPS
- MAVLink telemetry over USB
- SD logging
- Buffered logging recovery
- Battery monitoring
- Buzzer
- Replay validation
- Native automated tests

# Planned

- LoRa telemetry
- GY-87 integration
- Recovery servo
- Ground Station
- Flight Analyzer
- Real flight campaign

# Open Decisions

- Final LoRa module
- Final telemetry transport architecture
- Final power architecture
- Final recovery architecture
- Flight readiness criteria