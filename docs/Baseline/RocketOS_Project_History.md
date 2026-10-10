# RocketOS Project History and Decision Record

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


## Purpose

This document records major project decisions, the reason behind each decision, and the evolution from the original development prototype toward the RocketOS Mk1 architecture.

It is not a chronological transcript of every conversation or sprint.

Its purpose is to preserve the technical and strategic reasoning behind the project.

Later validation evidence may update the status of an earlier decision without rewriting the historical decision itself.


## Decision Status Convention

Each decision may be classified as:

- ACTIVE.
- ACTIVE TARGET DECISION.
- ACTIVE AND IMPLEMENTED.
- SUPERSEDED.
- UNDER REVIEW.
- CONCEPTUAL.

Implementation and validation are separate concepts.

A design may be implemented in firmware or PCB files without yet being physically or flight validated.


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

The repository contains separate first-party firmware modules for:

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

The Mk1 hardware architecture also separates the Main Deck, Power Deck, battery retention, and mechanical sled responsibilities.


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

The Mk1 v0.1 Main Deck and Power Deck follow this strategy by integrating commercial modules rather than replacing them unnecessarily with custom circuitry.


## DR-003: Validate Before Final Custom PCB Integration

Status: **ACTIVE**

### Decision

Hardware functionality should be validated before integration into final custom hardware where practical.

Prototype carrier PCBs may themselves be used as validation hardware when complete module-level integration cannot reasonably be validated beforehand.

### Reason

Premature final integration could commit the project to:

- Incorrect electrical assumptions.
- Incorrect pin assignments.
- Unsuitable modules.
- Mechanical incompatibilities.
- Unnecessary redesigns.
- Additional purchases.

### Consequence

Breadboard, module-level, carrier-PCB, and bench validation remain valid development stages.

The Mk1 v0.1 PCBs are prototype integration hardware and are not considered final flight-qualified electronics.


## DR-004: Preserve the Working Prototype

Status: **ACTIVE**

### Decision

The ESP32 DevKitC V4 implementation shall remain the historical working development and validation prototype.

Migration to Mk1 hardware shall preserve validated behavior rather than restart the firmware architecture from zero.

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

### Current Status

The Mk1 firmware has now been migrated to an ESP32-S3 build target while retaining the existing firmware architecture.

The DevKitC remains relevant as historical validation evidence but is no longer the Mk1 target MCU.


## DR-005: ESP32-S3 N16R8 as Target MCU

Status: **ACTIVE AND IMPLEMENTED AS MK1 FIRMWARE TARGET**

### Decision

ESP32-S3 N16R8 is selected as the target MCU for RocketOS Mk1.

### Reason

The recorded selection rationale includes:

- Cost/performance balance.
- 16 MB flash.
- 8 MB PSRAM.
- Suitable ecosystem support.
- Sufficient expected capability for RocketOS.
- Compact form factor.

### Historical Validation Status

At the v3 baseline:

- Target decision was present.
- Migration validation was not yet confirmed.
- Final pin mapping was not yet confirmed.
- Peripheral allocation was not yet confirmed.

### Updated Validation Status — 2026-10-10

Physical hardware identification confirmed:

- ESP32-S3.
- 240 MHz CPU.
- 16 MB flash.
- Approximately 8 MiB PSRAM.

The flight firmware now contains an `esp32s3_n16r8` target.

Validated build result:

- Build: SUCCESS.
- RAM: 29,452 / 327,680 bytes, 9.0%.
- Firmware flash: 394,633 / 3,342,336 bytes, 11.8%.

Native regression tests after migration:

- 9 / 9 succeeded.
- Three flight-profile tests.
- Six flight-state-machine tests.

The Mk1 GPIO allocation is now represented consistently in firmware and the Main Deck v0.1 design.

### Consequence

The S3 migration is no longer merely a target decision.

Firmware build compatibility and regression behavior have been demonstrated.

Complete assembled Mk1 hardware validation remains pending.


## DR-006: Prototype and Mk1 Baseline Separation

Status: **ACTIVE**

### Decision

Project documentation shall distinguish between:

1. Historical implemented prototype.
2. Current RocketOS Mk1 implementation and target architecture.
3. Planned roadmap.
4. Conceptual future ideas.

### Reason

Earlier documentation could be interpreted as if target components and planned subsystems were already implemented.

The repository audit demonstrated that this was not always the case.

### Consequence

Subsystem status shall distinguish implementation, design release, physical validation, and flight validation where relevant.


## DR-007: 54 mm-Class Target Architecture

Status: **ACTIVE TARGET DECISION**

### Decision

RocketOS Mk1 shall target a compact architecture suitable for a 54 mm-class airframe.

### Reason

The target supports a standardized compact avionics system for the intended model-rocket platform while retaining practical module-level serviceability.

### Historical Validation Status

At the v3 baseline:

- Target documented.
- Complete mechanical envelope not yet defined.
- Final PCB dimensions not yet validated.
- Complete stack fit not yet validated.

### Updated Status — 2026-10-10

The Mk1 electronics architecture has evolved to longitudinal PCBs.

Current v0.1 PCB envelopes are:

- Main Deck: 30 × 85.5 mm.
- Power Deck: 30 × 85.5 mm.

Both PCB envelopes are now represented in released fabrication data.

The final mechanical sled geometry and complete airframe fit remain to be validated.

### Consequence

The PCB envelope is no longer an open v0.1 design decision.

Mechanical sled and airframe integration remain open validation work.


## DR-008: Fixed 2S LiPo Power Source

Status: **ACTIVE TARGET DECISION**

### Decision

RocketOS Mk1 shall operate from a fixed 2S LiPo battery architecture.

The Tattu 2S 550 mAh LiPo is the current Mk1 development battery.

This records the development pack in use without preventing a later capacity change within the fixed 2S architecture if power-budget or endurance testing requires it.

### Context

Historical prototype documentation references:

- Tattu 4S LiPo.
- MP1584 regulator.

The historical 4S arrangement is superseded for the Mk1 battery baseline.

### Validation Status

- 2S battery architecture selected and fixed for Mk1.
- Current development battery: Tattu 2S 550 mAh.
- BatteryMonitor migrated from 4S to 2S.
- Bench validated on 2026-08-28.
- USB-only with LiPo disconnected measured approximately 4.10-4.13 V and reported `DISCONNECTED`.
- Connected 2S battery measured approximately 7.72-7.90 V and reported `CONNECTED` / `OK`.
- Disconnected threshold: below 5.0 V.
- SOC endpoints: 6.60 V = 0% and 8.40 V = 100%.
- Warning threshold: 7.20 V.
- Critical threshold: 6.80 V.

The current Mk1 Power Deck architecture now defines the regulator and rail topology.

Complete physical Mk1 electrical validation remains pending.

The 2026-08-28 evidence validates BatteryMonitor behavior on the bench configuration only.


## DR-009: Power Deck Strategy

Status: **ACTIVE AND IMPLEMENTED AS PROTOTYPE PCB DESIGN**

### Decision

A dedicated Power Deck shall provide the Mk1 power-distribution and power-monitoring architecture.

### Reason

- Separate power development from core flight-computer logic.
- Improve modularity.
- Improve connector reliability.
- Support compact integration.
- Allow power architecture development without redesigning every subsystem.

### Historical Evidence

The earlier repository contained Power Deck v0.1 KiCad development files.

At the v3 baseline, fabrication release, assembly, bring-up, and electrical validation had not been demonstrated.

### Updated Architecture

The active longitudinal Power Deck v0.1 provides:

- XT30 battery input.
- 2 A PPTC protection.
- Main power switching interface.
- INA219 measurement.
- MP1584 regulation.
- +5 V output to Main Deck.
- +3.3 V logic supply from Main Deck for INA219.
- I2C connection.
- Bulk and local decoupling.

PCB envelope:

- 30 × 85.5 mm.

### Updated Validation Status — 2026-10-10

Final design checks reported:

- 0 DRC violations.
- 0 unconnected items.
- 0 schematic-parity errors.

Gerber and drill data were generated and inspected.

An exact manufacturing package was created and committed.

The Power Deck v0.1 was released for prototype fabrication.

Still pending:

- Physical inspection.
- Assembly.
- Continuity testing.
- Board bring-up.
- Electrical integration validation.
- Flight validation.


## DR-010: Onboard Logging Independent of Telemetry

Status: **ACTIVE AND IMPLEMENTED**

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

Behavior during actual LoRa link loss remains to be validated after LoRa integration.


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

### Open Points

- Required minimum protected duration.
- Full-buffer behavior.
- Power-loss behavior with buffered records.
- Long-term repeated-failure behavior.

Logging frequency shall be determined from the current firmware configuration rather than historical documentation.


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

- Native tests validated.
- Profile replay validated.
- Physical ESP32 replay previously demonstrated.
- Native regression tests passed 9/9 after the ESP32-S3 migration.
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

RocketOS shall use MAVLink as the current structured telemetry message layer.

### Reason

- Structured telemetry messages.
- Compatibility with established ground software.
- Reuse of established message definitions.
- Separation of telemetry messages from the physical transport layer.

### Current Transport

The implemented transport is:

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

### Consequence

MAVLink message generation and the physical telemetry transport shall be treated as separate architectural layers.

LoRa does not replace the MAVLink message-layer decision merely by providing a future radio transport.


## DR-015: Longitudinal Mk1 PCB Architecture

Status: **ACTIVE AND IMPLEMENTED AS PROTOTYPE PCB DESIGN**

### Decision

RocketOS Mk1 shall use separate longitudinal Main Deck and Power Deck PCBs integrated through a mechanical sled.

The earlier circular Power Deck concept is superseded.

### Reason

The longitudinal architecture provides:

- Better compatibility with compact rocket airframes.
- More practical use of commercial modules.
- Improved service access.
- Independent Main and Power Deck development.
- Practical battery integration.
- Reduced pressure to compress modules into an unnecessarily small circular PCB.

### Baseline

Main Deck v0.1:

- 30 × 85.5 mm.

Power Deck v0.1:

- 30 × 85.5 mm.

Both boards use the same outer envelope.

Identical outer dimensions do not imply identical mechanical rail geometry.

### Mechanical Principle

The PCBs are intended to be retained by the sled architecture rather than using PCB mounting holes as the primary sled attachment method.

The battery shall be mechanically retained independently of the PCBs.

### Validation Status

- Main Deck design complete.
- Power Deck design complete.
- Fabrication packages generated.
- Prototype fabrication release completed.
- Physical sled validation pending.
- Complete 54 mm-class airframe fit validation pending.


## DR-016: Mk1 Main Deck Strategy

Status: **ACTIVE AND IMPLEMENTED AS PROTOTYPE PCB DESIGN**

### Decision

The Main Deck shall integrate the Mk1 flight-computer modules around the ESP32-S3 N16R8.

### Baseline Interfaces

The Main Deck v0.1 provides interfaces for:

- ESP32-S3 N16R8.
- BMP388.
- NEO-M9N GPS.
- MicroSD.
- GY-87.
- Ai-Thinker Ra-02 / SX1278-class LoRa.
- Active buzzer and transistor driver.
- Power Deck interconnection.

### Bus Architecture

I2C:

- SDA GPIO8.
- SCL GPIO9.

Shared SPI:

- MOSI GPIO11.
- MISO GPIO13.
- SCK GPIO12.
- MicroSD CS GPIO5.
- LoRa NSS GPIO10.

Additional LoRa signals:

- RESET GPIO14.
- DIO0 GPIO15.

GPS UART:

- RX GPIO17.
- TX GPIO18.

Buzzer:

- GPIO21.

### Validation Status — 2026-10-10

- Schematic ERC passed without violations.
- PCB routing completed.
- No unresolved routing connections.
- Fabrication data generated.
- Prototype fabrication package released.

Accepted footprint-library mismatch warnings and intentional courtyard exclusions remain documented KiCad conditions.

Physical bring-up and integrated electrical validation remain pending.


## DR-017: Ai-Thinker Ra-02 / SX1278 as Mk1 LoRa Hardware

Status: **ACTIVE TARGET DECISION**

### Decision

RocketOS Mk1 shall use the Ai-Thinker Ra-02 / SX1278-class module as its current LoRa radio hardware baseline.

### Reason

The selected module provides:

- Commercial off-the-shelf availability.
- Compact module format.
- SPI interface.
- Compatibility with the current Mk1 integration strategy.
- Adequate basis for prototype radio-link development.

### Consequence

Earlier uncertainty between SX1278 and alternative LoRa hardware is closed for the Mk1 v0.1 baseline.

The Main Deck provides the required Ra-02 interface.

### Validation Status

- Hardware selected.
- Main Deck schematic integration complete.
- Main Deck PCB integration complete.
- Flight-firmware LoRa transport not implemented.
- Shared SPI behavior with MicroSD not yet validated.
- Radio-link bench validation pending.
- Flight telemetry validation pending.


## DR-018: Mk1 Prototype Fabrication Release

Status: **ACTIVE MILESTONE**

### Decision

The Main Deck v0.1 and Power Deck v0.1 designs were considered sufficiently complete to release as prototype fabrication hardware.

This release is explicitly a validation step and not a flight-qualification milestone.

### Released PCB Baseline

Main Deck:

- 30 × 85.5 mm.
- 2-layer PCB.

Power Deck:

- 30 × 85.5 mm.
- 2-layer PCB.

Prototype manufacturing baseline:

- FR4.
- 1.6 mm thickness.
- 1 oz copper.
- Green solder mask.
- White silkscreen.
- Lead-free HASL.
- Tented vias.
- No assembly service.
- No stencil.

### Fabrication Artifacts

The repository contains the exact manufacturing packages for both boards, including:

- Gerber layers.
- Drill files.
- Gerber job files.
- Manufacturing ZIP packages.

### Manufacturing Order

A prototype order was placed for:

- 5 × Main Deck v0.1.
- 5 × Power Deck v0.1.

Manufacturer:

- JLCPCB.

### Validation Boundary

Fabrication release demonstrates that the v0.1 PCB designs reached prototype manufacturing readiness.

It does not demonstrate:

- Correct physical manufacture.
- Assembly.
- Continuity.
- Correct regulator output.
- Successful Main Deck power-up.
- Sensor communication.
- LoRa communication.
- Mechanical sled fit.
- Environmental robustness.
- Flight readiness.
- Flight qualification.

These validations must occur after the prototype boards are available.


## DR-019: ESP32-S3 Mk1 Firmware Migration

Status: **ACTIVE AND IMPLEMENTED**

### Decision

The existing RocketOS flight firmware shall be migrated to the Mk1 ESP32-S3 N16R8 target while preserving the validated architecture and behavior of the historical ESP32 prototype.

### Implementation

The migration includes:

- ESP32-S3 PlatformIO build environment.
- Mk1 GPIO assignments.
- Explicit I2C initialization.
- Explicit MicroSD SPI initialization.
- Controlled INA219 feature enablement.
- USB CDC startup handling.
- Critical initialization halt diagnostics.

### Validation — 2026-10-10

ESP32-S3 build:

- SUCCESS.
- RAM: 29,452 / 327,680 bytes, 9.0%.
- Firmware flash: 394,633 / 3,342,336 bytes, 11.8%.

Native regression suite:

- 9 / 9 tests passed.

### Consequence

The ESP32-S3 migration is now part of the implemented Mk1 firmware baseline.

This does not establish complete operation on the assembled Main Deck.


# Current Project Milestone

As of 2026-10-10, RocketOS has progressed from the historical ESP32 development prototype to the Mk1 prototype-integration stage.

The current baseline includes:

- ESP32-S3 Mk1 firmware target.
- Successful ESP32-S3 firmware build.
- 9/9 native regression tests passing.
- Main Deck v0.1 longitudinal PCB design.
- Power Deck v0.1 longitudinal PCB design.
- Exact prototype fabrication packages.
- Prototype PCB manufacturing release.
- Fixed 2S LiPo architecture.
- Ai-Thinker Ra-02 / SX1278 LoRa hardware baseline.

The next project phase is physical Mk1 integration and validation.

This includes:

- PCB receipt and inspection.
- Continuity testing.
- Power Deck bring-up.
- Main Deck bring-up.
- Integrated bus validation.
- Mechanical sled development and validation.
- LoRa firmware integration and radio-link validation.
- Integrated Mk1 bench testing.

Real-flight validation remains a later milestone.


# Superseded Baselines

The following previous project elements are retained for historical traceability but are no longer the active Mk1 baseline:

- ESP32 DevKitC V4 as the Mk1 target MCU.
- Historical 4S battery baseline.
- Earlier circular Power Deck mechanical concept.
- Unresolved SX1278 versus alternative LoRa-module selection.

These historical elements must not be confused with the current Mk1 configuration.


# Open Decisions

The following decisions remain open:

- Final recovery-actuation architecture.
- Final mechanical sled geometry.
- Final sled-to-airframe retention method.
- Detailed flight-readiness acceptance criteria.

The following decisions are now closed for Mk1 v0.1:

- MCU: ESP32-S3 N16R8.
- Battery architecture: fixed 2S LiPo.
- Development battery: Tattu 2S 550 mAh.
- PCB architecture: longitudinal Main Deck + Power Deck.
- PCB envelope: 30 × 85.5 mm for both v0.1 decks.
- LoRa hardware: Ai-Thinker Ra-02 / SX1278-class.