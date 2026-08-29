# RocketOS Project History and Decision Record

Author: David Garcia-Taheño

Document status: Baseline Reconciled v3

Repository reference:
- Branch: main
- Audited commit: 1b3023f8f2e982317c91b58bc29e1a734755e9d3
- Latest release identified during the audit: v0.14.1

## Purpose

This document records major project decisions, the reason behind each decision, and the relationship between the current prototype and the target RocketOS Mk1 architecture.

It is not a chronological transcript of every conversation or sprint.

Its purpose is to preserve the technical and strategic know-how behind the project.

## Decision Status Convention

Each decision may be classified as:

- ACTIVE
- SUPERSEDED
- UNDER REVIEW
- CONCEPTUAL

## Project Origin

RocketOS began as a flight telemetry and data-logging project for experimental model rockets.

The project evolved from an integrated ESP32 development prototype toward a broader modular ecosystem containing:

- Flight computer.
- Sensor acquisition.
- Onboard data logging.
- Flight-state detection.
- GPS.
- Power monitoring.
- Telemetry.
- Ground Station.
- Validation and replay tools.
- Post-flight analysis.
- Future recovery actuation.

## DR-001: Modular Architecture

Status: **ACTIVE**

### Decision

RocketOS shall use a modular hardware and software architecture.

### Reason

- Easier independent validation.
- Easier replacement of failed modules.
- Lower integration risk.
- Improved maintainability.
- Improved field repairability.
- Clearer firmware responsibilities.
- Reduced impact of subsystem changes.

### Current Evidence

The repository contains separate first-party modules for:

- Battery monitoring.
- BMP388.
- Buffered logging.
- Buzzer.
- External thermometer.
- Flight simulation.
- Flight-state machine.
- GPS.
- INA219.
- MAVLink telemetry.
- Recovery servo placeholder.
- SD logging.
- Serial profile replay.
- System events.
- System health.

## DR-002: Commercial Modules First

Status: **ACTIVE**

### Decision

RocketOS shall prioritize commercial off-the-shelf modules before replacing functionality with fully custom circuits.

### Reason

- Faster validation.
- Lower development cost.
- Easier replacement.
- Reduced PCB complexity.
- Reduced risk from unvalidated custom electronics.
- Better suitability for iterative prototyping.

### Consequence

Custom PCBs should initially focus on:

- Power distribution.
- Connectors.
- Mechanical integration.
- Module interconnection.
- Compact mounting.

## DR-003: Validate Before Custom PCB Integration

Status: **ACTIVE**

### Decision

Hardware functionality should be validated before integration into a final custom PCB.

### Reason

Premature integration could commit the project to:

- Incorrect electrical assumptions.
- Incorrect pin assignments.
- Unsuitable modules.
- Mechanical incompatibilities.
- Unnecessary redesigns.
- Additional purchases.

### Consequence

Breadboard and module-level validation remain valid development stages.

Mechanical fit and functional behavior should be confirmed before finalizing integrated PCB designs.

## DR-004: Preserve the Working Prototype

Status: **ACTIVE**

### Decision

The existing ESP32 DevKitC V4 implementation shall be treated as the working development and validation prototype.

### Reason

The prototype already contains implemented and tested value:

- Sensor drivers.
- GPS.
- SD logging.
- Buffered recovery.
- Battery monitoring.
- Buzzer.
- MAVLink.
- Flight-state detection.
- Health monitoring.
- Replay tools.
- Native tests.

Migration to the target Mk1 hardware should preserve this behavior instead of restarting the firmware architecture from zero.

## DR-005: ESP32-S3 SuperMini N16R8 as Target MCU

Status: **ACTIVE TARGET DECISION**

### Decision

ESP32-S3 SuperMini N16R8 is selected as the target MCU for RocketOS Mk1.

### Reason

The recorded selection rationale includes:

- Cost/performance balance.
- 16 MB Flash.
- 8 MB PSRAM.
- Suitable ecosystem support.
- Sufficient expected capability for RocketOS.
- Compact form factor.

### Validation Status

- Decision record: present.
- Rationale: present.
- Migration validation in the audited repository: not confirmed.
- Final pin mapping: not confirmed.
- Final peripheral-allocation validation: not confirmed.

### Consequence

The current DevKitC prototype and the Mk1 target MCU must be documented separately.

The target selection must not be rewritten as already validated hardware.

## DR-006: Prototype and Mk1 Baseline Separation

Status: **ACTIVE**

### Decision

Project documentation shall distinguish between:

1. Current implemented prototype.
2. RocketOS Mk1 target architecture.
3. Planned roadmap.
4. Conceptual future ideas.

### Reason

Earlier documentation could be interpreted as if target components and planned subsystems were already implemented.

The repository audit demonstrated that this was not always the case.

### Consequence

All significant subsystems should be classified as:

- IMPLEMENTED.
- PARTIALLY IMPLEMENTED.
- PLANNED.
- CONCEPTUAL.
- DEPRECATED.

## DR-007: 54 mm Target Architecture

Status: **ACTIVE TARGET DECISION**

### Decision

RocketOS Mk1 shall target a compact architecture suitable for a 54 mm airframe class.

### Reason

The target supports a standardized compact avionics stack for the intended model-rocket platform.

### Validation Status

- Target documented.
- Complete mechanical envelope not yet defined.
- Final PCB dimensions not yet validated.
- Complete stack fit not yet validated in the reconciled baseline.

## DR-008: Fixed 2S LiPo Power Source

Status: **ACTIVE TARGET DECISION**

### Decision

RocketOS Mk1 shall operate from a fixed 2S LiPo battery architecture.

The Tattu 2S 550 mAh LiPo is the current Mk1 development battery. This records the development pack in use without preventing a later capacity change within the fixed 2S architecture if power-budget or endurance testing requires it.

### Context

The current or historical prototype documentation references:

- Tattu 4S LiPo.
- MP1584 regulator.

The historical 4S prototype arrangement is superseded for Mk1 battery monitoring and must not be treated as the current battery baseline.

### Validation Status

- 2S battery architecture selected and fixed for Mk1.
- Current development battery recorded as Tattu 2S 550 mAh.
- BatteryMonitor migrated from 4S to 2S and bench validated on 2026-08-28.
- USB-only with LiPo disconnected measured 4.10-4.13 V and reported `DISCONNECTED`.
- Connected 2S battery measured 7.72-7.90 V and reported `CONNECTED` / `OK` with a coherent SOC estimate.
- Disconnected threshold: below 5.0 V.
- SOC endpoints: 6.60 V = 0% and 8.40 V = 100%.
- Warning threshold: 7.20 V.
- Critical threshold: 6.80 V.
- Final regulators not baselined here.
- Final rail architecture not baselined here.
- Final power budget not documented here.
- Complete Mk1 electrical validation pending.

The 2026-08-28 evidence validates BatteryMonitor behavior on the current bench prototype only. It does not claim that the Power Deck PCB has been fabricated, assembled, brought up, or electrically validated.

## DR-009: Power Deck Strategy

Status: **ACTIVE**

### Decision

A dedicated Power Deck shall be used to develop the target power distribution and integration architecture.

### Reason

- Separate power development from the core flight-computer logic.
- Improve modularity.
- Improve connector reliability.
- Support compact integration.
- Allow the power architecture to evolve without redesigning every subsystem.

### Current Evidence

The repository contains Power Deck v0.1 KiCad files.

### Validation Status

The available repository evidence does not yet confirm:

- Fabrication release.
- Final BOM.
- Assembly.
- Board bring-up.
- Electrical validation.
- Flight validation.

## DR-010: Onboard Logging Independent of Telemetry

Status: **ACTIVE AND IMPLEMENTED FOR CURRENT SERIAL TELEMETRY**

### Decision

Temporary loss of telemetry shall not stop onboard logging.

### Reason

- The radio or external link may fail during flight.
- Onboard data remains necessary for post-flight analysis.
- Logging and telemetry have different reliability dependencies.

### Current Implementation

- MicroSD CSV logging.
- Direct write attempts.
- RAM FIFO on failed writes.
- Ordered recovery after storage returns.

### Future Validation

Behavior under future LoRa link loss remains to be validated after LoRa integration.

## DR-011: Buffered Logging Recovery

Status: **ACTIVE AND IMPLEMENTED**

### Decision

RocketOS shall buffer records in RAM during temporary SD write failures.

### Reason

- Temporary card problems should not immediately discard current data.
- Ordered recovery improves completeness of the recorded flight.

### Current Implementation

- 100-record FIFO.
- Ordered buffer flush.
- Current default logging interval reported as one second.

### Open Points

- Required minimum protected duration.
- Full-buffer behavior.
- Power-loss behavior with buffered records.
- Long-term repeated-failure behavior.

## DR-012: Flight-State Machine

Status: **ACTIVE AND IMPLEMENTED**

### Decision

RocketOS shall use an explicit flight-state machine.

### Current Implemented States

- IDLE.
- BOOST.
- COAST.
- APOGEE.
- DESCENT.
- LANDED.

### Reason

- Provide deterministic flight-event interpretation.
- Support flight logging.
- Support audible events.
- Support telemetry state reporting.
- Provide a foundation for future recovery logic.

### Validation Status

- Native tests reported.
- Profile replay reported.
- Physical ESP32 replay reported.
- Real-flight validation pending.

## DR-013: Validation Through Flight-Profile Replay

Status: **ACTIVE AND IMPLEMENTED**

### Decision

RocketOS shall support repeatable testing using flight profiles.

### Reason

- Test state transitions without requiring repeated launches.
- Reproduce failure cases.
- Compare algorithms consistently.
- Validate low-apogee behavior.
- Validate response to Mach-related pressure disturbances.
- Reuse real or externally supplied profiles in the future.

### Current Implementation

- Native GoogleTest.
- CSV flight profiles.
- Metadata with expected values.
- OpenRocket and generic CSV importer.
- Serial profile sender.
- Replay into physical ESP32.

### Known Limitations

Current replay does not comprehensively reproduce:

- Vibration.
- Acceleration.
- Electrical disturbances.
- GPS loss.
- Real sensor noise.
- Timing stalls.
- Full barometric dynamics.
- Real flight environment.

## DR-014: MAVLink as Current Telemetry Message Layer

Status: **ACTIVE AND IMPLEMENTED**

### Decision

The current prototype shall use MAVLink messages for telemetry reporting.

### Reason

- Structured telemetry messages.
- Compatibility with QGroundControl.
- Reuse of established message definitions.
- Suitable foundation for later transport experimentation.

### Current Transport