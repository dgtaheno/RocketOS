# RocketOS Master Context

Author: David Garcia-Taheño

Document status: Baseline Reconciled v3

Repository reference:
- Repository: RocketOS
- Branch: main
- Audited commit: 1b3023f8f2e982317c91b58bc29e1a734755e9d3
- Latest release identified during the audit: v0.14.1

## Purpose of This Document

This document provides the concise strategic and technical context required to understand the RocketOS project.

It distinguishes explicitly between:

1. The currently implemented development prototype.
2. The selected RocketOS Mk1 target architecture.
3. Functionality explicitly planned in the roadmap.
4. Conceptual ideas that have not yet been accepted into the project baseline.

Repository code and test evidence take precedence when determining whether functionality is currently implemented.

Target-architecture documentation must not be interpreted as evidence that a subsystem has already been implemented, integrated, or validated.

## Project Definition

RocketOS is an open-source flight-computer, telemetry, data-logging, validation, and post-flight analysis ecosystem for experimental model rockets.

The current repository contains an advanced bench prototype and pre-flight integration platform.

The complete RocketOS ecosystem remains under development.

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

### Layer 1: Current Implemented Prototype

The repository currently implements an ESP32-based development prototype with:

- ESP32 DevKitC V4 build target.
- BMP388 barometric pressure and altitude sensor.
- NEO-M9N GPS using NMEA data and TinyGPSPlus.
- MicroSD onboard data logging.
- INA219 voltage, current, and power monitoring.
- DS18B20 external temperature monitoring.
- Active buzzer for audible status.
- MAVLink telemetry over USB Serial.
- Flight-state detection.
- System health monitoring.
- Runtime event monitoring.
- Buffered recovery from temporary SD write failures.
- Flight-profile simulation and serial replay tools.
- Native automated tests for flight-state behavior.
- Hardware replay validation using an ESP32.

The current prototype is best classified as:

**Advanced bench prototype / pre-flight integration stage.**

It is not yet a complete RocketOS Mk1 ecosystem and is not yet a flight-ready recovery computer.

### Layer 2: RocketOS Mk1 Target Architecture

The selected direction for RocketOS Mk1 includes:

- ESP32-S3 SuperMini N16R8 as target MCU.
- Compact architecture intended for a 54 mm airframe.
- Fixed 2S LiPo power source.
- Modular carrier/deck PCB architecture.
- Commercial off-the-shelf modules mounted on carrier or integration PCBs.
- Replaceable subsystems where practical.
- Dedicated radio telemetry.
- Dedicated RocketOS Ground Station.
- Post-flight analysis tools.
- Recovery actuation capability.

Target-architecture items must not be described as implemented unless repository code, hardware files, or validation evidence confirms their status.

### Layer 3: Explicitly Planned Functionality

The repository audit identified the following planned functionality:

- Recovery servo implementation.
- LoRa radio integration.
- Dedicated RocketOS Ground Station.
- Flight Analyzer.
- GY-87 / IMU integration.
- Real-flight validation campaign.
- Complete RocketOS Mk1 ecosystem.

These items are part of the stated development direction but are not implemented in the audited repository unless explicitly stated otherwise.

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
- Use repository code and test evidence as the technical source of truth.

## Integration Philosophy

The project currently prioritizes commercial modules mounted on carrier or deck PCBs.

Custom PCBs are primarily intended for:

- Power distribution.
- Connectors.
- Mechanical integration.
- Reliable module interconnection.
- Compact packaging.

A custom PCB should not replace a validated commercial module unless there is a documented technical reason.

Mechanical fit and functional behavior should be validated before committing to a final integrated PCB design.

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

## Current Prototype Hardware

The current repository implementation references:

- ESP32 DevKitC V4.
- BMP388.
- NEO-M9N GPS.
- MicroSD module.
- INA219.
- DS18B20.
- Active buzzer.
- USB Serial connection to the host or ground software.

The current repository documentation also references an MP1584 regulator and a 4S LiPo in the prototype or bench configuration.

This historical prototype power arrangement must not be confused with the Mk1 target architecture.

## RocketOS Mk1 Target Hardware

The current target direction includes:

- ESP32-S3 SuperMini N16R8.
- Fixed 2S LiPo architecture (7.4 V nominal, 8.4 V maximum).
- Tattu 2S 550 mAh LiPo as the current Mk1 development battery.
- Compact modular deck or carrier architecture.
- Power Deck.
- Radio telemetry module.
- Recovery actuator.
- 54 mm integration envelope.

The exact LoRa radio module still requires baseline confirmation.

Earlier documentation mentions an SX1278, while more recent architecture or roadmap material identified during repository analysis refers to an SX1262.

Until the selection is confirmed through a project decision, the authoritative wording is:

- LoRa radio module: target device not yet baselined.

## Current Implemented Firmware Capabilities

The audited repository contains:

- Modular C++ libraries.
- Central Arduino `setup()` and `loop()` control flow.
- Timed execution based on `millis()`.
- BMP388 pressure, temperature, and relative-altitude acquisition.
- GPS parsing and validity handling.
- INA219 voltage, current, and power acquisition.
- Battery state estimation and battery warnings.
- DS18B20 external-temperature acquisition.
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
- Serial profile replay into physical ESP32 hardware.
- OpenRocket and generic CSV profile import tooling.

## Implemented Flight-State Sequence

The audited firmware implements:

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

The current default logging interval reported by the repository audit is one second.

## Implemented Telemetry Behavior

The current firmware implements MAVLink over USB Serial.

The audited MAVLink wrapper sends:

- HEARTBEAT.
- SYS_STATUS.
- GPS_RAW_INT.
- GLOBAL_POSITION_INT.
- GLOBAL_POSITION_INT_COV.
- VFR_HUD.
- BATTERY_STATUS.
- HOME_POSITION.
- STATUSTEXT.

LoRa transport is not implemented in the audited repository.

MAVLink and LoRa must be treated as separate layers:

- MAVLink message layer: implemented over USB Serial.
- LoRa physical transport: planned.

## Current Validation Status

Repository-reported validation includes:

- Native state-machine tests.
- Full flight-profile replay tests.
- Serial replay into physical ESP32 hardware.
- Full state-sequence observation during replay.
- Validation of low-apogee behavior.
- Validation of a synthetic mid-power Mach-dip profile.
- Repository-reported QGroundControl compatibility.
- Repository-reported INA219 hot-plug behavior.
- BatteryMonitor bench validation on 2026-08-28 after migration from 4S to 2S:
  - USB-powered hardware with the LiPo disconnected measured 4.10-4.13 V and reported `DISCONNECTED`.
  - Connected 2S battery measured 7.72-7.90 V and reported `CONNECTED` / `OK` with a coherent SOC estimate.

The validated Mk1 BatteryMonitor baseline is:

- Battery disconnected below 5.0 V.
- SOC curve from 6.60 V = 0% to 8.40 V = 100%.
- Warning threshold at 7.20 V.
- Critical threshold at 6.80 V.

This validates the BatteryMonitor behavior on the current bench prototype. It does not validate the Power Deck PCB or the complete Mk1 power subsystem.

The repository audit did not independently rerun the tests.

No real rocket flight or deployment test was evidenced in the audited repository.

## Current Hardware Development

The repository contains KiCad work for:

- Power Deck v0.1.

The existence of KiCad files confirms hardware-design activity.

It does not by itself confirm:

- Manufacturing readiness.
- Completed schematic review.
- Completed PCB layout review.
- Released fabrication package.
- Completed BOM.
- Assembly.
- Board bring-up.
- Electrical validation.
- Flight validation.

## Current Development Status

- Strategic project definition: established.
- High-level target architecture: established.
- Prototype firmware: substantially implemented for bench testing.
- Modular firmware structure: implemented.
- BMP388 integration: implemented.
- GPS integration: implemented.
- MicroSD logging: implemented.
- Buffered logging recovery: implemented.
- INA219 integration: implemented in code.
- DS18B20 integration: implemented.
- Buzzer integration: implemented.
- MAVLink over USB Serial: implemented.
- Flight-state detection: implemented.
- Profile replay: implemented.
- Native automated tests: implemented.
- Power subsystem: under refinement.
- Power Deck PCB: under development.
- ESP32-S3 Mk1 migration: not yet confirmed as validated.
- Recovery actuation: not implemented.
- LoRa radio link: not implemented.
- GY-87 / IMU integration: not implemented.
- Dedicated Ground Station: not implemented.
- Flight Analyzer: not implemented.
- Real-flight validation: not completed.

## Known Baseline Issues

The repository audit identified the following items requiring attention:

- `SYSTEM_VERSION` is synchronized with the repository/software release version.
- `INA219_ENABLED` is false, but the INA219 is still initialized and read.
- Multiple feature flags may not consistently enable or disable functionality.
- MAVLink binary data and human-readable diagnostic output share the USB Serial interface.
- `RecoveryServo` source files are placeholders.
- `FlightSimulator` appears implemented but effectively disconnected from the main execution path.
- The detailed Mk1 power-distribution architecture is not settled.
- The final LoRa radio selection is not settled.
- PCB manufacturing readiness is not demonstrated.
- Real-flight validation is pending.

## Authority Rules

When determining project truth, use the following priority:

1. Current repository code at a named commit.
2. Current repository test reports and repeatable test evidence.
3. Approved decision records.
4. Current architecture and requirements documents.
5. Roadmap documentation.
6. Historical sprint material.
7. Unapproved concepts and discussion.

A component name or roadmap entry alone does not prove implementation.

A test report documents reported validation, but independent test evidence should be distinguished from author-reported results.

## Confidentiality and Project Separation

RocketOS is a personal open-source project.

Corporate information, files, emails, requirements, source code, or confidential material from the user's employer must not be introduced into the RocketOS project unless explicitly authorized and relevant.
