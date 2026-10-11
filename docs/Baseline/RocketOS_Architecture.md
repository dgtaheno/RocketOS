# RocketOS Architecture

Author: David Garcia-Taheño

Document status: Baseline Reconciled v4

Repository reference:

- Repository: RocketOS
- Branch: main
- Historical audited commit: 1b3023f8f2e982317c91b58bc29e1a734755e9d3
- ESP32-S3 firmware migration commit: cd093d0
- Mk1 longitudinal PCB design commit: f234d4b
- Baseline reconciliation date: 2026-10-10


## Overview

RocketOS is a modular open-source flight-computer, telemetry, logging, validation, and post-flight analysis ecosystem for experimental model rockets.

The architecture distinguishes between:

- The historical development prototype.
- The current RocketOS Mk1 implementation.
- Explicitly planned subsystems.
- Conceptual future functionality.

The current project is in the Mk1 pre-flight integration stage.

The ESP32-S3 firmware target and Mk1 longitudinal PCB designs now exist, but the complete Mk1 hardware has not yet been physically brought up or flight validated.


## Architecture Status Convention

Every subsystem in this document must be classified using one of the following states:

### IMPLEMENTED

Repository code or hardware-design artifacts exist and provide the described core functionality.

Implementation status does not automatically imply physical or flight validation.

### PARTIALLY IMPLEMENTED

Some core functionality exists, but the target subsystem is incomplete or has not reached its intended integrated state.

### PLANNED

The subsystem is part of the documented roadmap but has no complete implementation in the current repository.

### CONCEPTUAL

The subsystem or feature is under consideration but is not part of the confirmed baseline.

### DEPRECATED

The subsystem or implementation has been explicitly replaced or retired.

Repository code, hardware source files, fabrication artifacts, and repeatable validation evidence take precedence when determining implementation status.

The presence of a filename, empty module, roadmap item, schematic, or documentation section does not by itself prove complete implementation or validation.


## Historical Development Prototype

Status: **IMPLEMENTED / SUPERSEDED AS MK1 TARGET**

The original RocketOS development prototype used an ESP32 DevKitC V4 and provided the bench platform on which the core firmware architecture was developed and validated.

This configuration included:

- BMP388.
- NEO-M9N GPS.
- DS18B20.
- INA219.
- MicroSD.
- Active buzzer.
- USB Serial.

This prototype remains relevant as historical validation evidence but is no longer the Mk1 target hardware architecture.


## RocketOS Mk1 System Architecture

Status: **PARTIALLY IMPLEMENTED**

The Mk1 architecture is organized around three primary physical elements:

```text
2S LiPo Battery
      |
      v
+-----------------------+
|      Power Deck       |
|                       |
| Protection            |
| Main Power Switching  |
| INA219 Measurement    |
| MP1584 Regulation     |
+-----------+-----------+
            |
            | +5V / +3V3 / I2C / GND
            v
+-----------------------+
|       Main Deck       |
|                       |
| ESP32-S3 N16R8        |
| BMP388                |
| NEO-M9N GPS           |
| MicroSD               |
| GY-87                 |
| Ra-02 / SX1278        |
| Active Buzzer         |
+-----------+-----------+
            |
            v
      Flight Firmware
            |
      +-----+------+
      |            |
      v            v
  SD Logging    Telemetry
                   |
             USB MAVLink
             implemented
                   |
             LoRa transport
                planned
```

The Main Deck and Power Deck are separate longitudinal PCBs connected electrically and integrated mechanically through the Mk1 sled architecture.

The battery is mechanically retained independently of the PCBs.


## Mk1 Mechanical Architecture

Status: **PARTIALLY IMPLEMENTED**

The Mk1 electronics architecture uses longitudinal PCBs rather than the earlier circular deck concept.

Current PCB envelopes:

- Main Deck v0.1: 30 × 85.5 mm.
- Power Deck v0.1: 30 × 85.5 mm.

The two boards are intended to mount longitudinally in a mechanical sled.

The battery is not intended to use either PCB as its structural support.

The sled architecture must provide:

- PCB retention.
- Independent battery retention.
- Connector access.
- Serviceability.
- Cable management.
- Mechanical compatibility with the selected rocket airframe.

The final sled geometry and airframe retention method remain under development.

Identical PCB outer dimensions do not require identical rail geometry because edge-mounted components and connector clearances differ between the Main Deck and Power Deck.


## Mk1 Main Deck Architecture

Status: **IMPLEMENTED AS PCB DESIGN / PHYSICAL VALIDATION PENDING**

The Main Deck v0.1 integrates the flight-computer electronics around the ESP32-S3 N16R8.

Current hardware interfaces include:

- BMP388 barometric sensor.
- NEO-M9N GPS.
- MicroSD storage.
- GY-87 sensor module.
- Ai-Thinker Ra-02 / SX1278-class LoRa module.
- Active buzzer with transistor driver.
- Power Deck interconnection.

The Main Deck uses:

- I2C for compatible sensor interfaces.
- Shared SPI for MicroSD and LoRa.
- Independent chip-select signals for SPI devices.
- UART for GPS.

Current Mk1 pin baseline:

- GPS RX: GPIO17.
- GPS TX: GPIO18.
- I2C SDA: GPIO8.
- I2C SCL: GPIO9.
- SPI MOSI: GPIO11.
- SPI MISO: GPIO13.
- SPI clock: GPIO12.
- MicroSD CS: GPIO5.
- LoRa NSS: GPIO10.
- LoRa RESET: GPIO14.
- LoRa DIO0: GPIO15.
- Buzzer control: GPIO21.

DS18B20 support remains available in firmware but is disabled in the current Mk1 configuration.

The Main Deck v0.1 schematic has passed ERC without violations.

The PCB has no unresolved routing connections.

Fabrication data has been generated and released for prototype manufacture.

Physical assembly, electrical bring-up, integrated bus validation, and flight validation remain pending.


## Mk1 Power Architecture

Status: **IMPLEMENTED AS PCB DESIGN / PHYSICAL VALIDATION PENDING**

The RocketOS Mk1 battery architecture is fixed to a 2S LiPo:

- Nominal pack voltage: 7.4 V.
- Maximum charged voltage: 8.4 V.
- Current development battery: Tattu 2S 550 mAh LiPo.

The Power Deck v0.1 architecture is:

```text
2S LiPo
   |
   v
 XT30
   |
   v
2 A PPTC
   |
   v
Main Switch
   |
   v
 INA219
   |
   v
 MP1584
   |
   +----> +5V to Main Deck

Main Deck +3V3
   |
   +----> INA219 logic supply

Main Deck I2C
   |
   +----> INA219 SDA/SCL
```

The Power Deck provides:

- Battery input.
- Resettable overcurrent protection.
- Main switching interface.
- Battery voltage/current monitoring.
- 5 V regulation.
- Main Deck power connection.
- INA219 logic and I2C connection.
- Bulk and local decoupling.

The Power Deck v0.1 final design check reported:

- 0 DRC violations.
- 0 unconnected items.
- 0 schematic-parity errors.

Fabrication data has been generated and released for prototype manufacture.

The PCB has not yet been physically assembled, electrically brought up, or flight validated.


## Battery Monitoring Architecture

Status: **IMPLEMENTED IN FIRMWARE / MK1 HARDWARE VALIDATION PENDING**

The BatteryMonitor uses the Mk1 2S baseline:

- Battery disconnected below 5.0 V.
- 6.60 V = 0%.
- 6.80 V = 10% and critical threshold.
- 7.00 V = 25%.
- 7.20 V = 40% and warning threshold.
- 7.40 V = 50%.
- 7.60 V = 60%.
- 7.80 V = 70%.
- 8.00 V = 80%.
- 8.20 V = 90%.
- 8.40 V = 100%.

Bench validation completed on 2026-08-28:

- LiPo disconnected while powered by USB: approximately 4.10-4.13 V, reported `DISCONNECTED`.
- 2S LiPo connected: approximately 7.72-7.90 V, reported `CONNECTED` / `OK` with a coherent SOC estimate.

This evidence validates BatteryMonitor behavior on the bench configuration.

It does not validate the complete Mk1 Power Deck.

INA219 initialization and runtime acquisition are now controlled by `INA219_ENABLED`.


## Mk1 Firmware Architecture

Status: **IMPLEMENTED / INTEGRATION VALIDATION ONGOING**

The flight firmware uses a central Arduino/PlatformIO execution model:

```text
Hardware Interfaces
    |
    +-- BMP388
    +-- NEO-M9N GPS
    +-- INA219
    +-- MicroSD
    +-- Active Buzzer
    +-- DS18B20 [optional / disabled in current Mk1]
    +-- GY-87 [hardware interface present / firmware pending]
    +-- Ra-02 [hardware interface present / firmware pending]
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

The current Mk1 firmware has been migrated to the ESP32-S3 target.

On 2026-10-10:

- `esp32s3_n16r8` build: SUCCESS.
- RAM usage: 29,452 / 327,680 bytes, 9.0%.
- Firmware flash usage: 394,633 / 3,342,336 bytes, 11.8%.
- Native regression tests: 9/9 passed.

Physical module identification separately confirmed:

- ESP32-S3.
- 240 MHz CPU.
- 16 MB flash.
- Approximately 8 MiB PSRAM.

A successful build and native tests validate firmware compatibility and regression behavior.

They do not establish complete Mk1 hardware operation.


## Flight-State Architecture

Status: **IMPLEMENTED**

The current flight-state sequence is:

```text
IDLE
  |
  v
BOOST
  |
  v
COAST
  |
  v
APOGEE
  |
  v
DESCENT
  |
  v
LANDED
```

The state machine currently uses barometric altitude and derived climb rate.

This subsystem detects flight events.

It does not currently command recovery deployment.


## Logging Architecture

Status: **IMPLEMENTED**

RocketOS currently provides:

- CSV flight logging.
- Automatic log creation.
- GPS-based timestamp filenames when available.
- Boot-number fallback filenames.
- Sensor and flight-state records.
- SD startup read/write self-test.
- Runtime SD failure detection.
- Temporary RAM buffering.
- Ordered recovery of buffered records.
- Separate runtime event logging.

The configured recovery buffer contains 100 records.


## Telemetry Architecture

### MAVLink Message Layer

Status: **IMPLEMENTED**

The current firmware generates MAVLink telemetry over USB Serial.

Implemented message types include:

- HEARTBEAT.
- SYS_STATUS.
- GPS_RAW_INT.
- GLOBAL_POSITION_INT.
- GLOBAL_POSITION_INT_COV.
- VFR_HUD.
- BATTERY_STATUS.
- HOME_POSITION.
- STATUSTEXT.

### LoRa Physical Transport

Status: **PLANNED IN FIRMWARE / HARDWARE INTERFACE DEFINED**

The Mk1 hardware baseline uses:

- Ai-Thinker Ra-02.
- SX1278-class LoRa radio.
- Shared SPI bus with MicroSD.
- Independent LoRa NSS.
- RESET and DIO0 connections.

The radio hardware interface is represented in the Main Deck v0.1 design.

LoRa transport is not implemented or validated in the current flight firmware.

MAVLink and LoRa must therefore be treated as separate architectural layers.


## GY-87 / IMU Architecture

Status: **PLANNED IN FIRMWARE / HARDWARE INTERFACE DEFINED**

The GY-87 is represented in the Main Deck v0.1 hardware design and shares the Mk1 I2C bus.

Firmware acquisition and use of GY-87 data are not currently part of the implemented flight-computer baseline.

Mk1 shared-I2C operation has been bench validated.

The GY-87 used for Mk1 contains an MPU-6050 at address 0x68 and an onboard BMP180 at address 0x77. This conflicts with the external BMP388 default address of 0x77.

The Mk1 architecture therefore fixes the external BMP388 at address 0x76 by tying its SDO pin to GND. Bench validation confirmed simultaneous I2C operation of INA219 at 0x40, MPU-6050 at 0x68, external BMP388 at 0x76, and GY-87 BMP180 at 0x77.

The Main Deck v0.1 fabrication package was released before this address conflict was discovered and leaves the external BMP388 SDO pin unconnected. Fabricated Main Deck v0.1 boards therefore require an SDO-to-GND rework when the BMP388 is installed. The current KiCad source includes the corrected SDO-to-GND connection.

This validation establishes electrical coexistence on the shared I2C bus; GY-87 firmware acquisition remains unimplemented.


## Recovery Architecture

Status: **PLANNED**

Recovery actuation remains part of the RocketOS roadmap.

Current firmware flight-state detection does not constitute recovery deployment control.

Servo-based recovery remains planned.

Pyrotechnic deployment, MOSFET deployment, dual recovery, or additional recovery channels remain conceptual unless promoted through a separate documented decision.


## Ground Station Architecture

Status: **PLANNED**

A dedicated RocketOS Ground Station remains part of the target ecosystem.

The current USB MAVLink interface provides development and validation telemetry but does not constitute the final dedicated RocketOS Ground Station.


## Flight Analyzer Architecture

Status: **PLANNED**

Post-flight analysis remains part of the RocketOS target ecosystem.

Existing CSV logging and replay tools provide the data and validation foundation, but the dedicated Flight Analyzer is not yet implemented.


## Mk1 Validation Boundary

The current architecture has validated evidence for:

- Core flight firmware behavior.
- Flight-state-machine regression behavior.
- Flight-profile replay.
- Historical ESP32 bench operation.
- 2S BatteryMonitor behavior.
- ESP32-S3 physical module identification.
- ESP32-S3 firmware compilation.
- Native regression tests.
- Main Deck v0.1 design completion and fabrication release.
- Power Deck v0.1 design completion and fabrication release.

The following remain outside the validated boundary:

- Physical Mk1 PCB assembly.
- PCB continuity and power-up testing.
- Complete Power Deck electrical bring-up.
- Main Deck integrated bring-up.
- Shared SPI operation with MicroSD and LoRa.
- Integrated Mk1 I2C bus operation.
- LoRa communication.
- GY-87 firmware operation.
- Recovery actuation.
- Mechanical sled validation.
- Environmental testing.
- Real rocket flight.
- Recovery deployment in flight.


## Architecture Authority

When architecture documentation conflicts with implementation evidence, use the following priority:

1. Current repository code at a named commit.
2. Repeatable test and validation evidence.
3. Current hardware source files and released fabrication artifacts.
4. Approved decision records.
5. Architecture and requirements documents.
6. Roadmap material.
7. Historical documentation.
8. Conceptual discussion.

Hardware-design completion must not be interpreted as physical hardware validation.

Firmware build success must not be interpreted as complete system validation.