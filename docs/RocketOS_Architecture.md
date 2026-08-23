# RocketOS Architecture

**Project:** RocketOS  
**Architecture Version:** 1.0  
**Author:** David Garcia-Taheño  
**Date:** August 2026

---

# Overview

RocketOS is an open-source engineering platform for model rocketry.

The objective is not simply to build a flight computer.

The objective is to provide a complete workflow that allows users to:

```text
Design
Validate
Verify
Fly
Track
Analyze
Improve
```

using a single ecosystem.

RocketOS combines:

```text
Flight Computer
Telemetry
Ground Station
Validation Framework
Flight Analysis
```

into a unified platform.

---

# Vision

Traditional hobby rocketry systems are often fragmented.

Users typically require separate tools for:

```text
Flight Simulation
Flight Computer
Telemetry
GPS Tracking
Flight Analysis
```

RocketOS aims to provide a connected workflow:

```text
OpenRocket
      ↓
Replay Validation
      ↓
Flight Computer
      ↓
Telemetry
      ↓
Ground Station
      ↓
Flight Analysis
```

The goal is to reduce complexity while increasing confidence and reliability.

---

# Core Principles

## Test Before Flight

Every critical function should be testable before a real launch.

RocketOS uses:

```text
Unit Tests
Replay Validation
Hardware Replay
Integration Testing
```

to verify system behaviour before flight.

---

## Open Architecture

RocketOS is designed to be:

```text
Open Source
Modular
Extensible
```

Users should be able to:

```text
Modify hardware
Modify software
Add sensors
Create custom modules
```

without vendor lock-in.

---

## Data Ownership

All flight data belongs to the user.

Preferred formats:

```text
CSV
JSON
MAVLink
```

No cloud services are required.

---

## Reliability First

The most important function of the system is:

```text
Safe Recovery
```

A successful recovery is more important than additional features.

---

# Ecosystem Architecture

```text
                OpenRocket
                     │
                     ▼
            Replay Validation
                     │
                     ▼
      RocketOS Flight Computer Mk1
                     │
                     ▼
              LoRa Telemetry
                     │
                     ▼
      RocketOS Ground Station
                     │
                     ▼
              QGroundControl
                     │
                     ▼
          RocketOS Analyzer
```

---

# RocketOS Flight Computer Mk1

The Flight Computer Mk1 is the airborne component of RocketOS.

Responsibilities:

```text
GPS Position
Altitude Measurement
Flight Detection
Recovery Control
Data Logging
Health Monitoring
Telemetry Generation
```

---

## Target Hardware

```text
ESP32
GPS M9N
BMP388
INA219
microSD
LoRa SX1262
Recovery Servo
USB-C Interface
```

---

## Airframe Target

Primary target:

```text
54 mm Airframes
```

Secondary compatibility:

```text
75 mm Airframes
```

Future variant:

```text
38 mm Compact Version
```

---

## Mechanical Architecture

Target dimensions:

```text
Maximum PCB Diameter:
45 mm

Maximum Assembly Length:
150 mm
```

Target power source:

```text
2S LiPo
450-500 mAh
```

Target mass:

```text
90 g or less
```

---

## Internal Layout

Conceptual arrangement:

```text
      Nose Cone
          │
          ▼

 ┌─────────────────┐
 │ GPS Antenna     │
 ├─────────────────┤
 │ Main PCB        │
 ├─────────────────┤
 │ 2S LiPo Battery │
 ├─────────────────┤
 │ Recovery Servo  │
 └─────────────────┘

          ▼

     Recovery System
```

---

# Telemetry Architecture

Initial telemetry technology:

```text
LoRa SX1262
868 MHz
```

Telemetry protocol:

```text
MAVLink
```

Data transmitted:

```text
GPS Position
Altitude
Speed
Flight State
Battery Status
System Health
Events
```

---

# Ground Station

The RocketOS Ground Station provides launch and recovery support.

Target hardware:

```text
Raspberry Pi 5
```

Responsibilities:

```text
Telemetry Reception
Map Display
Flight Monitoring
Recovery Guidance
Data Storage
```

User interface:

```text
QGroundControl
```

---

# Flight Analysis

RocketOS includes a post-flight analysis stage.

Inputs:

```text
Flight Logs
Telemetry Logs
Replay Profiles
```

Outputs:

```text
Flight Summary
Graphs
Statistics
Maps
Reports
Comparisons
```

Long-term goal:

```text
Automatic Flight Reports
```

generated from recorded telemetry.

---

# Validation Philosophy

RocketOS follows a layered validation strategy.

---

## Level 1

### Unit Tests

Purpose:

```text
Validate individual software modules.
```

Examples:

```text
Flight State Machine
Battery Monitor
Recovery Logic
```

Tools:

```text
GoogleTest
PlatformIO Native
```

---

## Level 2

### Replay Validation

Purpose:

```text
Validate behaviour using complete flight profiles.
```

Examples:

```text
Nominal Flight
Mach Dip Flight
Low Apogee Flight
```

Verification:

```text
Apogee Detection
Event Detection
Tolerance Validation
```

---

## Level 3

### Hardware Replay

Purpose:

```text
Validate behaviour on real hardware.
```

Current implementation:

```text
CSV Profile
      ↓
USB Serial
      ↓
ESP32
      ↓
Flight State Machine
```

Verification:

```text
No Sample Loss
Correct State Sequence
Matching Replay Results
```

---

## Level 4

### Integration Verification

Purpose:

```text
Verify subsystem interaction.
```

Examples:

```text
GPS + LoRa
GPS + Servo
LoRa + Logging
GPS + LoRa + Servo
```

Validation areas:

```text
Power Integrity
Signal Integrity
Subsystem Interaction
```

---

## Level 5

### Flight Verification

Purpose:

```text
Validate complete system operation.
```

Examples:

```text
Telemetry Flight
Recovery Flight
Range Validation
Full Mission Validation
```

---

# Recovery System

Initial recovery solution:

```text
Micro Servo Deployment
```

Requirements:

```text
Single Deployment
No False Deployment
Independent Operation
```

Future possibility:

```text
MOSFET Deployment Outputs
Dual Recovery
```

---

# Expansion Strategy

The architecture should support future modules without major redesign.

Candidate modules:

```text
IMU
Environmental Sensors
Camera Trigger
Additional Recovery Channels
Payload Interfaces
```

---

# Roadmap

## v0.15

```text
Recovery Servo Implementation
```

---

## v0.16

```text
LoRa Telemetry Integration
```

---

## v0.17

```text
Ground Station Prototype
```

---

## v0.18

```text
Flight Analyzer Prototype
```

---

## v1.0

```text
Complete RocketOS Mk1 Ecosystem
```

Target capabilities:

```text
Design
Validate
Verify
Fly
Track
Analyze
Improve
```

within a single platform.

---

# Success Criteria

RocketOS is considered successful when a user can:

```text
Design a Flight
        ↓
Validate Electronically
        ↓
Verify Hardware Behaviour
        ↓
Fly the Rocket
        ↓
Track the Vehicle
        ↓
Recover the Vehicle
        ↓
Analyze the Data
        ↓
Improve the Next Flight
```

without changing tools or platforms.

---

# Architecture Vision

RocketOS is not a single flight computer.

RocketOS is an open-source rocket engineering ecosystem built around:

```text
Flight
Telemetry
Validation
Analysis
Reliability
```

The long-term objective is to provide a platform that makes model rocketry safer, more accessible and more engineering-driven while remaining open, modular and community-focused.