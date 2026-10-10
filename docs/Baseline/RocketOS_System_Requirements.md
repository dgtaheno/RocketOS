# RocketOS System Requirements

Author: David Garcia-Taheño

Document status: Baseline Reconciled v4

Repository reference:

- Repository: RocketOS
- Branch: main
- Historical audited commit: 1b3023f8f2e982317c91b58bc29e1a734755e9d3
- ESP32-S3 firmware migration commit: cd093d0
- Mk1 longitudinal PCB design commit: f234d4b
- Baseline reconciliation date: 2026-10-10


## Purpose

This document defines the target requirements for RocketOS Mk1.

A documented requirement does not imply implementation or validation.

Implementation status must be determined from:

- Repository code.
- Hardware design files.
- Released fabrication artifacts.
- Test reports.
- Validation records.
- Approved decision records.

Hardware-design completion does not imply successful physical hardware validation.


---

# System Mission

RocketOS is an open-source flight-computer, telemetry, data-logging, validation, and post-flight analysis ecosystem for experimental model rockets.

Its primary goals are:

- Reliable flight data acquisition.
- Reliable onboard logging.
- GPS tracking.
- Flight-state detection.
- Power monitoring.
- Telemetry transmission.
- Post-flight analysis.
- Hardware and software modularity.


---

# Target Platform

## Airframe

Target architecture:

- 54 mm-class rocket airframe.

Status:

- Target defined.
- Mechanical sled validation pending.


---

## MCU

Target MCU:

- ESP32-S3 N16R8.

Status:

- Selected for Mk1.
- Mk1 firmware build target implemented.
- ESP32-S3 build validated on 2026-10-10.
- Physical development module identified as 16 MB flash and approximately 8 MiB PSRAM.
- Integrated Mk1 PCB validation pending.

Historical development prototype:

- ESP32 DevKitC V4.


---

## PCB Architecture

Target architecture:

- Longitudinal modular PCB architecture.
- Separate Main Deck and Power Deck.
- Main Deck v0.1: 30 × 85.5 mm.
- Power Deck v0.1: 30 × 85.5 mm.
- PCB retention through the mechanical sled architecture.
- Battery mechanically retained independently of the PCBs.

Status:

- Main Deck v0.1 design complete and released for prototype fabrication.
- Power Deck v0.1 design complete and released for prototype fabrication.
- Physical PCB bring-up pending.
- Mechanical sled validation pending.

The identical PCB envelopes do not require identical rail geometry.


---

## Power Source

Target power source:

- Fixed 2S LiPo.
- 7.4 V nominal.
- 8.4 V maximum.
- Tattu 2S 550 mAh as the current development battery.

Target Power Deck architecture:

- XT30 battery input.
- 2 A resettable protection fuse.
- Main power switching.
- INA219 power monitoring.
- MP1584 regulation.
- 5 V supply to Main Deck.
- 3.3 V logic supply from Main Deck for INA219.
- I2C connection between Main Deck and INA219.

Status:

- Selected and baselined for Mk1.
- BatteryMonitor bench validated on 2026-08-28.
- Power Deck v0.1 design released for prototype fabrication.
- Complete Mk1 power subsystem validation pending.

Historical prototype power configurations must not be confused with the Mk1 architecture.


---

## Radio Hardware

Target radio:

- Ai-Thinker Ra-02.
- SX1278-class LoRa radio.

Target interface:

- Shared SPI bus with MicroSD.
- Independent LoRa NSS.
- RESET.
- DIO0.

Status:

- Hardware selected and baselined for Mk1.
- Main Deck hardware interface implemented.
- Flight-firmware LoRa transport not implemented.
- Radio-link validation pending.


---

# Functional Requirements

## REQ-F-001 Sensor Acquisition

RocketOS shall acquire data from all enabled baseline sensors.

Current firmware support:

- BMP388.
- INA219.
- DS18B20.
- NEO-M9N GPS.

Current Mk1 configuration:

- BMP388 enabled.
- NEO-M9N GPS enabled.
- INA219 firmware support implemented but currently disabled pending Mk1 hardware bring-up.
- DS18B20 firmware support implemented but disabled in the current Mk1 configuration.

Planned firmware integration:

- GY-87.

Status:

- Partially Implemented.


---

## REQ-F-002 Relative Altitude

RocketOS shall calculate relative altitude using barometric data.

Current implementation:

- BMP388.
- Boot-relative reference.

Status:

- Implemented.
- Replay validated.


---

## REQ-F-003 Flight-State Detection

RocketOS shall determine the current flight state.

Current implemented states:

- IDLE.
- BOOST.
- COAST.
- APOGEE.
- DESCENT.
- LANDED.

Status:

- Implemented.
- Native-test validated.
- Replay validated.
- Real-flight validation pending.


---

## REQ-F-004 Independent Logging

RocketOS shall continue onboard logging when telemetry is unavailable.

Status:

- Implemented for the current firmware architecture.
- LoRa transport validation pending.

Loss of telemetry shall not prevent normal onboard flight-data recording.


---

## REQ-F-005 SD Failure Recovery

RocketOS shall temporarily buffer data when SD storage becomes unavailable.

Current implementation:

- FIFO RAM buffer.
- 100 records.
- Ordered recovery.

Status:

- Implemented.


---

## REQ-F-006 Flight Data Recording

RocketOS shall store flight and system data on onboard storage.

Current implementation:

- CSV logging.
- Automatic file creation.
- GPS timestamp support.
- Boot-number filename fallback.
- Runtime event logging.

Status:

- Implemented.


---

## REQ-F-007 Battery Monitoring

RocketOS shall monitor:

- Voltage.
- Current.
- Power.

Current implementation:

- INA219 support.
- Battery state estimation.

Mk1 BatteryMonitor baseline:

- Battery disconnected below 5.0 V.
- 6.60 V = 0%.
- 6.80 V = 10%.
- 7.00 V = 25%.
- 7.20 V = 40%.
- 7.40 V = 50%.
- 7.60 V = 60%.
- 7.80 V = 70%.
- 8.00 V = 80%.
- 8.20 V = 90%.
- 8.40 V = 100%.
- Warning threshold: 7.20 V.
- Critical threshold: 6.80 V.

Status:

- Firmware implementation complete.
- Bench validated on 2026-08-28 after migration from 4S to 2S.
- INA219 access controlled by `INA219_ENABLED`.
- Mk1 Power Deck physical validation pending.

Bench evidence:

- USB-only, LiPo disconnected: approximately 4.10-4.13 V, reported `DISCONNECTED`.
- 2S LiPo connected: approximately 7.72-7.90 V, reported `CONNECTED` / `OK`.

This evidence validates BatteryMonitor behavior on the bench configuration.

It does not establish complete Mk1 Power Deck electrical validation.


---

## REQ-F-008 GPS Data

RocketOS shall acquire GPS information when valid GPS data is available.

Current implementation:

- Position.
- Altitude.
- Speed.
- Time and date.
- Home reference.

Status:

- Implemented.
- Mk1 ESP32-S3 outdoor-fix validation pending.


---

## REQ-F-009 Audible Status

RocketOS shall provide audible status information.

Current implementation:

- Active buzzer.

Implemented patterns include:

- Startup.
- GPS lock.
- Launch.
- Apogee.
- Landing.
- Critical battery.

Status:

- Implemented.
- Integrated Mk1 hardware validation pending.


---

## REQ-F-010 System Health Monitoring

RocketOS shall monitor subsystem health and report detected failures where practical.

Current implementation:

- Power-on checks.
- Health counters.
- Event reporting.
- SD failure detection.
- Critical initialization failure handling.

Status:

- Partially Implemented.


---

## REQ-F-011 Telemetry Generation

RocketOS shall generate telemetry messages.

Current implementation:

- MAVLink over USB Serial.

Implemented messages include:

- HEARTBEAT.
- SYS_STATUS.
- GPS_RAW_INT.
- GLOBAL_POSITION_INT.
- GLOBAL_POSITION_INT_COV.
- VFR_HUD.
- BATTERY_STATUS.
- HOME_POSITION.
- STATUSTEXT.

Status:

- Implemented.


---

## REQ-F-012 LoRa Telemetry

RocketOS Mk1 shall support radio telemetry using the Mk1 LoRa hardware interface.

Target hardware:

- Ai-Thinker Ra-02.
- SX1278-class radio.

Status:

- Hardware selected.
- Main Deck hardware interface implemented.
- Flight-firmware transport planned.
- Bench radio-link validation pending.

MAVLink message generation and LoRa physical transport shall be treated as separate architectural layers.


---

## REQ-F-013 Ground Station

RocketOS shall provide a dedicated Ground Station capable of receiving and presenting RocketOS telemetry.

Status:

- Planned.
- Not implemented.


---

## REQ-F-014 Flight Analyzer

RocketOS shall support post-flight analysis.

Current supporting functionality:

- CSV logs.
- Flight-profile replay tools.

Dedicated Flight Analyzer:

- Planned.

Status:

- Partially Implemented at data/tooling level.
- Dedicated analyzer not implemented.


---

## REQ-F-015 Recovery Actuation

RocketOS Mk1 shall support recovery actuation.

Status:

- Planned.

Current repository state:

- Recovery source structure exists.
- No operational recovery actuation implementation exists.

Flight-state detection shall not be interpreted as recovery-deployment capability.


---

## REQ-F-016 Main Deck Integration

The RocketOS Mk1 Main Deck shall provide the electrical interfaces required for the Mk1 flight-computer subsystems.

Baseline interfaces:

- ESP32-S3 N16R8.
- BMP388.
- NEO-M9N GPS.
- MicroSD.
- GY-87.
- Ai-Thinker Ra-02 / SX1278.
- Active buzzer.
- Power Deck interconnection.

Status:

- Schematic implemented.
- PCB implemented.
- Fabrication package released.
- Physical bring-up pending.


---

## REQ-F-017 Power Deck Integration

The RocketOS Mk1 Power Deck shall provide protected battery input, power monitoring, regulation, and electrical connection to the Main Deck.

Baseline functions:

- 2S LiPo input.
- Resettable overcurrent protection.
- Main power switching.
- INA219 measurement.
- MP1584 5 V regulation.
- Main Deck interconnection.

Status:

- Schematic implemented.
- PCB implemented.
- Fabrication package released.
- Physical bring-up pending.


---

# Interface Requirements

## REQ-I-001 Main Deck / Power Deck Interface

The Main Deck and Power Deck shall exchange:

- +5 V.
- +3.3 V.
- I2C SCL.
- I2C SDA.
- GND.

Current connector baseline:

- 5-position interconnection.

Status:

- Defined in Mk1 v0.1 hardware.
- Physical validation pending.


---

## REQ-I-002 Shared SPI Bus

MicroSD and LoRa shall use the Mk1 shared SPI bus with independent chip-select control.

Current pin baseline:

- MOSI: GPIO11.
- MISO: GPIO13.
- SCK: GPIO12.
- MicroSD CS: GPIO5.
- LoRa NSS: GPIO10.

Status:

- MicroSD firmware implemented.
- LoRa hardware interface implemented.
- Combined MicroSD/LoRa operation not yet validated.


---

## REQ-I-003 I2C Bus

Mk1 I2C devices shall use:

- SDA: GPIO8.
- SCL: GPIO9.

Current hardware users include:

- BMP388.
- GY-87.
- INA219 through the Power Deck.

Status:

- Interface defined.
- BMP388 firmware implemented.
- Complete integrated Mk1 bus validation pending.


---

# Validation Requirements

## REQ-V-001 Native Automated Tests

The flight-state machine shall be testable through native automated tests.

Status:

- Implemented.

Latest validated result on 2026-10-10:

- 9 / 9 tests passing.
- Three flight-profile tests.
- Six flight-state-machine tests.


---

## REQ-V-002 Flight Replay Validation

RocketOS shall support replay validation.

Current profiles include:

- estes_c6.
- low_apogee.
- midpower_machdip.

Status:

- Implemented.


---

## REQ-V-003 Hardware Replay

RocketOS shall support replay execution on physical ESP32 hardware.

Status:

- Implemented on the development prototype.

Mk1 ESP32-S3 integrated hardware replay remains to be validated after PCB bring-up.


---

## REQ-V-004 Real Flight Validation

RocketOS shall be validated during real rocket flights before being considered flight validated.

Status:

- Pending.

No RocketOS Mk1 configuration shall be described as flight validated solely from bench, simulation, replay, firmware-build, or PCB-design evidence.


---

## REQ-V-005 Validation Before PCB Integration

Subsystem behavior should be validated before replacing known working prototype behavior with custom integrated hardware.

Status:

- Active development rule.

The Mk1 v0.1 PCBs are prototype integration hardware intended to validate the selected longitudinal architecture.


---

## REQ-V-006 ESP32-S3 Firmware Build Validation

The RocketOS Mk1 flight firmware shall compile successfully for the selected ESP32-S3 target.

Target:

- ESP32-S3 N16R8.

Validated on 2026-10-10:

- Build result: SUCCESS.
- RAM: 29,452 / 327,680 bytes, 9.0%.
- Firmware flash: 394,633 / 3,342,336 bytes, 11.8%.

Status:

- Implemented and validated.


---

## REQ-V-007 Main Deck Design Validation

The Main Deck PCB design shall pass the project design checks required before prototype fabrication release.

Current v0.1 result:

- ERC passed without violations.
- No unresolved routing connections.
- Fabrication files generated.
- Prototype fabrication package released.

Status:

- Design validation complete.
- Physical validation pending.

Accepted footprint-library mismatch warnings and intentional courtyard exclusions do not constitute electrical validation failures.


---

## REQ-V-008 Power Deck Design Validation

The Power Deck PCB design shall pass the project design checks required before prototype fabrication release.

Current v0.1 result:

- 0 DRC violations.
- 0 unconnected items.
- 0 schematic-parity errors.
- Fabrication files generated.
- Prototype fabrication package released.

Status:

- Design validation complete.
- Physical validation pending.


---

## REQ-V-009 PCB Bring-Up

The Mk1 prototype PCBs shall undergo controlled physical bring-up before integrated flight testing.

Bring-up shall include, at minimum:

- Visual inspection.
- Continuity and short-circuit checks.
- Power-rail verification.
- Regulator output verification before Main Deck connection.
- Main Deck power-up.
- Required subsystem communication checks.

Status:

- Pending.


---

## REQ-V-010 Integrated Mk1 Validation

The assembled Mk1 electronics shall be validated as an integrated system before real-flight validation.

Validation shall include the enabled flight-critical logging and acquisition functions of the Mk1 configuration.

Status:

- Pending.


---

# Non-Functional Requirements

## Reliability

The system shall prioritize reliability over unnecessary complexity.


---

## Modularity

Subsystems should remain replaceable where practical.

Commercial off-the-shelf modules should be preferred where they provide adequate performance and reliability.


---

## Maintainability

The system should remain understandable and maintainable by a hobbyist developer.


---

## Field Repairability

Modules and connections should allow field replacement whenever practical.


---

## Cost Awareness

The project shall avoid unnecessary purchases and unnecessary custom hardware.


---

## Mechanical Serviceability

The Mk1 mechanical architecture should provide practical access to:

- PCBs.
- Battery.
- Inter-board connection.
- Serviceable connectors.

The battery shall be retained independently of the PCBs.


---

# Current Implementation Summary

Implemented or design-released today:

- ESP32-S3 Mk1 firmware build target.
- Sensor-acquisition framework.
- BMP388 acquisition.
- GPS acquisition.
- INA219 firmware support.
- DS18B20 firmware support.
- Flight-state machine.
- MAVLink telemetry over USB.
- SD logging.
- Buffered logging recovery.
- Battery monitoring.
- Buzzer support.
- Replay validation.
- Native automated tests.
- Main Deck v0.1 PCB design.
- Power Deck v0.1 PCB design.
- Main and Power Deck prototype fabrication packages.


# Planned

- LoRa firmware transport.
- GY-87 firmware integration.
- Recovery actuation.
- Ground Station.
- Flight Analyzer.
- Mk1 PCB bring-up.
- Integrated Mk1 bench validation.
- Mechanical sled validation.
- Real-flight campaign.


# Open Decisions

The following decisions remain open before later implementation stages:

- Final recovery actuation architecture.
- Final mechanical sled geometry and airframe retention method.
- Detailed flight-readiness acceptance criteria.

The following are no longer open baseline decisions:

- Mk1 MCU: ESP32-S3 N16R8.
- Mk1 battery architecture: fixed 2S LiPo.
- Mk1 PCB architecture: longitudinal Main Deck + Power Deck.
- Mk1 PCB envelope: 30 × 85.5 mm for both v0.1 decks.
- Mk1 LoRa hardware: Ai-Thinker Ra-02 / SX1278-class.