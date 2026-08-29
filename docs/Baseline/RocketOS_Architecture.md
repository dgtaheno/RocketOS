# RocketOS Architecture

Author: David Garcia-Taheño

Document status: Baseline Reconciled v3

Repository reference:
- Branch: main
- Audited commit: 1b3023f8f2e982317c91b58bc29e1a734755e9d3

## Overview

RocketOS is a modular open-source flight-computer, telemetry, logging, validation, and post-flight analysis ecosystem for experimental model rockets.

The architecture distinguishes between:

- The current development prototype.
- The RocketOS Mk1 target architecture.
- Explicitly planned subsystems.
- Conceptual future functionality.

The current implementation and the target architecture are related but are not identical.

## Architecture Status Convention

Every subsystem in this document must be classified using one of the following states:

### IMPLEMENTED

Repository code exists and provides the described core functionality.

### PARTIALLY IMPLEMENTED

Some core functionality exists, but the target subsystem is incomplete or has not reached its intended final architecture.

### PLANNED

The subsystem is part of the documented roadmap but has no complete implementation in the audited repository.

### CONCEPTUAL

The subsystem or feature is under consideration but is not part of the confirmed baseline.

### DEPRECATED

The subsystem or implementation has been explicitly replaced or retired.

Repository code and validation evidence take precedence when determining implementation status.

The presence of a filename, empty module, roadmap item, schematic, or documentation section does not by itself prove implementation.

## Current Prototype Architecture

The current firmware uses a central Arduino/PlatformIO execution model:

```text
Hardware Drivers
    |
    +-- BMP388
    +-- NEO-M9N GPS
    +-- DS18B20
    +-- INA219
    +-- MicroSD
    +-- Active Buzzer
    |
    v
Acquisition and Derived Values
    |
    +-- Pressure
    +-- Temperature
    +-- Relative Altitude
    +-- GPS Position and Speed
    +-- Battery Voltage, Current and Power
    +-- Battery State
    +-- Climb Rate
    |
    v
Flight Processing and Health
    |
    +-- FlightStateMachine
    +-- SystemHealth
    +-- SystemEvents
    |
    +-------------------------------+
    |                               |
    v                               v
CSV Logging                  MAVLink over USB Serial
    |
    +-- Direct SD Write
    +-- RAM Buffer on Failure
    +-- Ordered Recovery
```

## RocketOS Mk1 Power Architecture

Status: **PARTIALLY IMPLEMENTED**

The RocketOS Mk1 battery architecture is fixed to a 2S LiPo:

- Nominal pack voltage: 7.4 V.
- Maximum charged voltage: 8.4 V.
- Current development battery: Tattu 2S 550 mAh LiPo.

The current firmware BatteryMonitor was migrated from its historical 4S curve to the following 2S baseline:

- Battery disconnected below 5.0 V.
- SOC curve from 6.60 V = 0% to 8.40 V = 100%.
- Warning threshold at 7.20 V.
- Critical threshold at 6.80 V.

Bench validation completed on 2026-08-28:

- LiPo disconnected while powered by USB: 4.10-4.13 V, reported `DISCONNECTED`.
- 2S LiPo connected: 7.72-7.90 V, reported `CONNECTED` / `OK` with a coherent SOC estimate.

This evidence validates the BatteryMonitor behavior on the current bench setup. The Power Deck remains under development; its KiCad files do not demonstrate that a PCB has been fabricated, assembled, brought up, or electrically validated.