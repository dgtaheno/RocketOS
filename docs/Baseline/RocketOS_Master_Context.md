# RocketOS Master Context

Author: David Garcia-Taheño

Document status: Baseline Reconciled v4

Repository reference:

- Repository: RocketOS
- Branch: main
- Historical audited commit: 1b3023f8f2e982317c91b58bc29e1a734755e9d3
- ESP32-S3 firmware migration commit: cd093d0
- ESP32-S3 hardware identification utility commit: a648cfc
- Mk1 longitudinal PCB design commit: f234d4b
- Hardware reference photos commit: 6c75aa6
- Baseline reconciliation date: 2026-10-10


## Purpose of This Document

This document provides the concise strategic and technical context required to understand the RocketOS project.

It distinguishes explicitly between:

1. The currently implemented and validated development baseline.
2. The selected RocketOS Mk1 target architecture.
3. Functionality explicitly planned in the roadmap.
4. Conceptual ideas that have not yet been accepted into the project baseline.

Repository code, hardware design files, fabrication artifacts, and test evidence take precedence when determining implementation status.

Target-architecture documentation must not be interpreted as evidence that a subsystem has already been physically integrated or validated.


## Project Definition

RocketOS is an open-source flight-computer, telemetry, data-logging, validation, and post-flight analysis ecosystem for experimental model rockets.

The current repository contains an advanced bench prototype, an ESP32-S3 Mk1 firmware target, and released Mk1 longitudinal prototype PCB designs.

The complete RocketOS ecosystem remains under development.

No real rocket flight or recovery-deployment validation has yet been completed.


## Project Objectives

RocketOS is intended to provide:

- Reliable acquisition of flight and system data.
- Reliable onboard data logging.
- Flight-state detection.
- Position and time information through GPS.
- Battery voltage and current monitoring.
- Telemetry of flight and health information.
- Audible system-status indication.
- Repeatable validation using simulated and replayed flight profiles.
- Post-flight data analysis.
- A modular and maintainable hardware and software platform.


## Baseline Layers

### Layer 1: Current Implemented Development Baseline

The repository currently implements firmware supporting:

- ESP32-S3 Mk1 build target.
- BMP388 barometric pressure and altitude sensor.
- NEO-M9N GPS using NMEA data and TinyGPSPlus.
- MicroSD onboard data logging.
- INA219 voltage, current, and power monitoring.
- Optional DS18B20 external temperature monitoring.
- Active buzzer for audible status.
- MAVLink telemetry over USB Serial.
- Flight-state detection.
- System health monitoring.
- Runtime event monitoring.
- Buffered recovery from temporary SD write failures.
- Flight-profile simulation and serial replay tools.
- Native automated tests for flight-state behavior.
- Hardware identification and validation utilities.

The historical ESP32 DevKitC V4 implementation remains relevant as the original development and bench-validation platform.

The current Mk1 firmware build target is the ESP32-S3 N16R8 configuration.

The current development baseline is best classified as:

**Advanced bench prototype / Mk1 pre-flight integration stage.**

It is not yet a flight-qualified RocketOS Mk1 system and is not yet a validated recovery computer.


### Layer 2: RocketOS Mk1 Target Architecture

The selected direction for RocketOS Mk1 includes:

- ESP32-S3 N16R8 MCU.
- Longitudinal modular PCB architecture.
- 30 × 85.5 mm Main Deck v0.1.
- 30 × 85.5 mm Power Deck v0.1.
- Fixed 2S LiPo power source.
- Commercial off-the-shelf modules mounted on carrier or integration PCBs.
- Independent mechanical battery retention.
- Replaceable subsystems where practical.
- Ai-Thinker Ra-02 / SX1278-class LoRa radio hardware.
- Dedicated RocketOS Ground Station.
- Post-flight analysis tools.
- Recovery actuation capability.

The current Main Deck and Power Deck fabrication designs have been released for prototype manufacture.

Fabrication release does not constitute assembly, electrical bring-up, environmental validation, flight validation, or flight qualification.


### Layer 3: Explicitly Planned Functionality

The following functionality remains planned:

- Recovery servo implementation.
- LoRa firmware integration and radio-link validation.
- Dedicated RocketOS Ground Station.
- Flight Analyzer.
- GY-87 / IMU firmware integration.
- Physical bring-up and validation of the Mk1 prototype PCBs.
- Integrated Mk1 system testing.
- Real-flight validation campaign.
- Complete RocketOS Mk1 ecosystem.

These items are part of the stated development direction but must not be described as implemented unless repository code or validation evidence confirms their status.


### Layer 4: Conceptual Functionality

The following ideas may appear in roadmap or design material but are not part of the confirmed Mk1 baseline unless promoted through a documented decision:

- Pyrotechnic deployment outputs.
- MOSFET deployment outputs.
- Dual recovery.
- A compact 38 mm variant.
- Additional sensors.
- Camera trigger.
- Additional payload interfaces.
- Additional recovery channels.

Conceptual items must not be treated as requirements or committed roadmap items without an explicit project decision.


## Design Philosophy

RocketOS follows these principles:

- Prefer commercial off-the-shelf modules whenever practical.
- Validate hardware and software concepts before custom PCB integration.
- Prioritize reliability over unnecessary complexity.
- Prioritize modularity.
- Prioritize maintainability.
- Prioritize field repairability.
- Minimize unnecessary cost.
- Avoid unnecessary hardware purchases.
- Preserve working, validated functionality during migration or redesign.
- Keep core modules replaceable where practical.
- Prefer proven solutions over experimental complexity.
- Separate implemented functionality from planned and conceptual functionality.
- Use repository code and repeatable test evidence as the technical source of truth.


## Integration Philosophy

The project prioritizes commercial modules mounted on carrier or deck PCBs.

Custom PCBs are primarily intended for:

- Power distribution.
- Connectors.
- Mechanical integration.
- Reliable module interconnection.
- Compact packaging.

A custom PCB should not replace a validated commercial module unless there is a documented technical reason.

Mechanical fit and functional behavior should be validated before committing to later integrated PCB revisions.

The Mk1 v0.1 longitudinal PCBs are prototype integration hardware intended to validate this architecture physically.


## Development Preferences

When supporting this project:

- Provide complete files by default when code is requested.
- Do not provide partial snippets unless explicitly requested.
- Preserve existing behavior and backward compatibility where practical.
- Do not propose redesigns without identifying the problem being solved.
- Do not propose hardware purchases unless strongly justified.
- Prioritize validation before optimization.
- Start with the simplest viable solution.
- Clearly justify increases in complexity.
- Distinguish documented facts from assumptions.
- Do not invent missing pin assignments, interfaces, requirements, or validation results.
- Prefer updating existing documentation over creating unnecessary new documents.
- Keep documentation concise and maintainable.


## Historical Development Prototype Hardware

The historical development and bench-validation configuration references:

- ESP32 DevKitC V4.
- BMP388.
- NEO-M9N GPS.
- MicroSD module.
- INA219.
- DS18B20.
- Active buzzer.
- USB Serial connection to the host or ground software.

Earlier repository documentation also references an MP1584 regulator and a 4S LiPo in the prototype or bench configuration.

This historical prototype power arrangement must not be confused with the current Mk1 architecture.


## RocketOS Mk1 Hardware Baseline

The current Mk1 hardware baseline includes:

- ESP32-S3 N16R8.
- BMP388.
- NEO-M9N GPS breakout.
- MicroSD module.
- INA219 power monitor.
- GY-87 sensor module.
- Ai-Thinker Ra-02 / SX1278-class LoRa module.
- Active buzzer.
- Fixed 2S LiPo architecture, 7.4 V nominal and 8.4 V maximum.
- Tattu 2S 550 mAh LiPo as the current Mk1 development battery.
- Longitudinal Main Deck.
- Longitudinal Power Deck.
- Modular mechanical sled architecture.

DS18B20 support remains available in firmware but is disabled in the current Mk1 configuration.

The Main Deck v0.1 and Power Deck v0.1 both use a 30 × 85.5 mm PCB envelope.

The boards are intended to mount longitudinally in the mechanical sled architecture.

The battery is mechanically retained independently of the PCBs.


## Mk1 Main Deck v0.1

The current Main Deck v0.1 design integrates or provides interfaces for:

- ESP32-S3 N16R8.
- BMP388.
- NEO-M9N GPS.
- MicroSD.
- GY-87.
- Ai-Thinker Ra-02 / SX1278-class LoRa radio.
- Active buzzer and transistor driver.
- Power Deck interconnection.

The Main Deck design uses shared buses where appropriate:

- I2C for compatible sensor modules.
- Shared SPI for MicroSD and LoRa with independent chip-select signals.

The Main Deck schematic has passed ERC without violations.

The PCB has no unresolved routing connections.

Fabrication data has been generated and released for prototype manufacture.

Accepted KiCad footprint-library mismatch warnings and intentional courtyard exclusions are documented design-tool conditions and are not equivalent to electrical validation.


## Mk1 Power Deck v0.1

The current Power Deck v0.1 provides the prototype power architecture based on:

- XT30 battery input.
- 2 A resettable protection fuse.
- Main power switch interface.
- INA219 current and voltage measurement.
- MP1584 regulator module.
- 5 V output to the Main Deck.
- 3.3 V logic supply return from the Main Deck for INA219 communication.
- I2C interconnection.
- Bulk and local decoupling.

The Power Deck schematic and PCB have passed the final KiCad design checks used for the prototype release:

- 0 DRC violations.
- 0 unconnected items.
- 0 schematic-parity errors.

Fabrication data has been generated and released for prototype manufacture.

The Power Deck PCB has not yet been physically assembled, electrically brought up, or flight validated.


## Current Implemented Firmware Capabilities

The current repository contains:

- Modular C++ libraries.
- Central Arduino `setup()` and `loop()` control flow.
- Timed execution based on `millis()`.
- ESP32-S3 build configuration.
- BMP388 pressure, temperature, and relative-altitude acquisition.
- GPS parsing and validity handling.
- Optional INA219 voltage, current, and power acquisition.
- Battery state estimation and battery warnings.
- Optional DS18B20 external-temperature acquisition.
- MicroSD CSV logging.
- RAM buffering during temporary SD failures.
- Recovery and ordered flush of buffered records.
- Runtime event logging.
- System health monitoring.
- Audible buzzer patterns.
- Flight-state detection.
- MAVLink message generation over USB Serial.
- Native GoogleTest validation.
- Flight-profile replay.
- Serial profile replay.
- OpenRocket and generic CSV profile import tooling.

The INA219 initialization and runtime update paths are controlled by `INA219_ENABLED`.

The current Mk1 configuration disables INA219 firmware access until the corresponding hardware integration is ready for bring-up.

DS18B20 is also disabled in the current Mk1 configuration.


## Current ESP32-S3 Pin Baseline

The current Mk1 firmware configuration uses:

- GPS RX: GPIO17.
- GPS TX: GPIO18.
- I2C SDA: GPIO8.
- I2C SCL: GPIO9.
- Shared SPI MOSI: GPIO11.
- Shared SPI MISO: GPIO13.
- Shared SPI clock: GPIO12.
- MicroSD CS: GPIO5.
- LoRa NSS: GPIO10.
- LoRa RESET: GPIO14.
- LoRa DIO0: GPIO15.
- Buzzer control: GPIO21.

DS18B20 support is currently disabled.

GPIO4 is therefore not part of the active Mk1 sensor configuration.


## Implemented Flight-State Sequence

The firmware implements:

- IDLE
- BOOST
- COAST
- APOGEE
- DESCENT
- LANDED

The current implementation uses barometric altitude and derived climb rate.

This functionality is flight-event detection.

It is not guidance, navigation, closed-loop flight control, or recovery deployment control.


## Implemented Logging Behavior

The current firmware includes:

- Automatic CSV log creation.
- GPS-based timestamp filenames when GPS time is available.
- Boot-number filename fallback.
- Sensor and state data records.
- SD startup read/write self-test.
- SD failure detection.
- Temporary RAM buffering.
- Ordered recovery of buffered records.
- Separate runtime event log.

The configured buffer contains 100 records.

Logging frequency remains defined by the current firmware configuration and must not be inferred from historical documentation when evaluating performance.


## Implemented Telemetry Behavior

The current firmware implements MAVLink over USB Serial.

The MAVLink wrapper provides messages including:

- HEARTBEAT.
- SYS_STATUS.
- GPS_RAW_INT.
- GLOBAL_POSITION_INT.
- GLOBAL_POSITION_INT_COV.
- VFR_HUD.
- BATTERY_STATUS.
- HOME_POSITION.
- STATUSTEXT.

LoRa hardware is now selected and represented in the Mk1 hardware design.

LoRa transport is not yet implemented or validated in the current flight firmware.

MAVLink and LoRa must therefore continue to be treated as separate layers:

- MAVLink message layer: implemented over USB Serial.
- LoRa hardware interface: defined in the Mk1 hardware baseline.
- LoRa physical transport in flight firmware: planned.


## Current Validation Status

Repository and bench validation includes:

- Native state-machine tests.
- Full flight-profile replay tests.
- Serial replay into physical ESP32 hardware.
- Full state-sequence observation during replay.
- Validation of low-apogee behavior.
- Validation of a synthetic mid-power Mach-dip profile.
- Repository-reported QGroundControl compatibility.
- Repository-reported INA219 hot-plug behavior.
- BatteryMonitor bench validation after migration from 4S to 2S.
- Physical identification of the ESP32-S3 N16R8 development module.
- Successful compilation of the Mk1 flight firmware for ESP32-S3.
- Native regression-test execution after the ESP32-S3 migration.

Physical ESP32-S3 identification confirmed:

- ESP32-S3.
- 240 MHz CPU.
- 16 MB flash.
- Approximately 8 MiB PSRAM.

On 2026-10-10, the `esp32s3_n16r8` flight-firmware environment built successfully.

Reported build usage:

- RAM: 29,452 bytes / 327,680 bytes, 9.0%.
- Firmware flash usage: 394,633 bytes / 3,342,336 bytes, 11.8%.

The native regression suite executed after the migration with:

**9 test cases / 9 succeeded.**

The tests covered three flight-profile cases and six flight-state-machine cases.

The validated Mk1 BatteryMonitor baseline remains:

- Battery disconnected below 5.0 V.
- SOC curve from 6.60 V = 0% to 8.40 V = 100%.
- Warning threshold at 7.20 V.
- Critical threshold at 6.80 V.

Previous bench validation reported:

- USB-powered hardware with the LiPo disconnected measured approximately 4.10-4.13 V and reported `DISCONNECTED`.
- Connected 2S battery measured approximately 7.72-7.90 V and reported `CONNECTED` / `OK` with a coherent SOC estimate.

This validates BatteryMonitor behavior on the bench configuration.

It does not validate the complete Mk1 Power Deck hardware.

No real rocket flight or recovery-deployment test has yet been completed.


## Current Hardware Development

The repository now contains the active Mk1 longitudinal KiCad designs for:

- Main Deck v0.1.
- Power Deck v0.1.

The previous Power Deck design has been moved to the `superseded` hardware area.

The active Main Deck and Power Deck designs each use a 30 × 85.5 mm PCB envelope.

The repository includes:

- KiCad schematic sources.
- KiCad PCB sources.
- Required custom symbols and footprints.
- Gerber fabrication layers.
- PTH and NPTH drill files.
- Gerber job files.
- Exact manufacturing ZIP packages.

The v0.1 fabrication packages have been released for prototype manufacture.

This establishes prototype PCB manufacturing readiness for these specific v0.1 designs.

It does not establish:

- Correct physical manufacture.
- Assembly quality.
- Board bring-up.
- Electrical integration validation.
- Mechanical sled validation.
- Environmental validation.
- Flight validation.


## Current Development Status

- Strategic project definition: established.
- High-level Mk1 architecture: established.
- Historical ESP32 bench prototype: implemented and validated for development use.
- ESP32-S3 Mk1 firmware migration: implemented and build validated.
- ESP32-S3 physical module identification: validated.
- Modular firmware structure: implemented.
- BMP388 integration: implemented.
- GPS integration: implemented.
- MicroSD logging: implemented.
- Buffered logging recovery: implemented.
- INA219 firmware integration: implemented and feature-gated.
- DS18B20 firmware integration: implemented but disabled for current Mk1.
- Buzzer integration: implemented.
- MAVLink over USB Serial: implemented.
- Flight-state detection: implemented.
- Profile replay: implemented.
- Native automated tests: implemented and passing 9/9.
- Main Deck v0.1 PCB design: released for prototype fabrication.
- Power Deck v0.1 PCB design: released for prototype fabrication.
- Mk1 PCB physical bring-up: pending.
- Mk1 mechanical sled: not yet finalized.
- Recovery actuation: not implemented.
- LoRa hardware selection: established as Ai-Thinker Ra-02 / SX1278-class.
- LoRa firmware radio link: not implemented.
- GY-87 hardware interface: present in Mk1 design; shared-I2C electrical coexistence bench validated.
- GY-87 firmware integration: not implemented.
- Dedicated Ground Station: not implemented.
- Flight Analyzer: not implemented.
- Integrated Mk1 bench validation: partially completed; shared-I2C sensor-bus operation and BMP388/GPS/MicroSD operation have been bench validated, while complete Mk1 integration remains pending.
- Real-flight validation: not completed.


## Known Baseline Issues and Open Validation Items

The following items remain open:

- MAVLink binary data and human-readable diagnostic output share the USB Serial interface.
- `RecoveryServo` source files remain placeholders.
- `FlightSimulator` integration with the main execution path requires confirmation before being treated as active runtime functionality.
- Main Deck and Power Deck v0.1 require physical inspection after manufacture.
- The released Main Deck v0.1 fabrication package leaves the external BMP388 SDO pin unconnected. The fabricated v0.1 boards require an SDO-to-GND rework to select BMP388 address 0x76 and avoid the GY-87 BMP180 address at 0x77. The current KiCad source contains this correction.
- Main Deck and Power Deck require continuity and power-up validation.
- MP1584 output must be verified before connection to the Main Deck during hardware bring-up.
- USB plus external 5 V power behavior requires validation before simultaneous use.
- GPS outdoor fix behavior on the Mk1 ESP32-S3 configuration remains to be validated.
- Shared SPI operation between MicroSD and LoRa requires firmware implementation and bench validation.
- Shared I2C operation of the Mk1 sensor set requires integrated bench validation.
- LoRa radio transport is not implemented.
- Recovery actuation is not implemented.
- Mechanical sled geometry and retention remain to be finalized and validated.
- Real-flight validation is pending.


## Authority Rules

When determining project truth, use the following priority:

1. Current repository code at a named commit.
2. Current repository test reports and repeatable validation evidence.
3. Released hardware source files and exact fabrication artifacts for hardware-design state.
4. Approved decision records.
5. Current architecture and requirements documents.
6. Roadmap documentation.
7. Historical sprint material.
8. Unapproved concepts and discussion.

A component name or roadmap entry alone does not prove implementation.

A PCB design or fabrication package proves a hardware-design and manufacturing-release state, not successful physical validation.

A successful firmware build proves build compatibility, not complete hardware operation.

A test report documents the behavior covered by that test and must not be generalized beyond its validation scope.


## Confidentiality and Project Separation

RocketOS is a personal open-source project.

Corporate information, files, emails, requirements, source code, or confidential material from the user's employer must not be introduced into the RocketOS project unless explicitly authorized and relevant.