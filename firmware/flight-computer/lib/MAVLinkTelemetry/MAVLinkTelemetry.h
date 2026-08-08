#ifndef MAVLINK_TELEMETRY_H
#define MAVLINK_TELEMETRY_H

#include <Arduino.h>
#include "Config.h"
#include "common/mavlink.h"

class MAVLinkTelemetry
{
public:
    MAVLinkTelemetry();

    void begin();

    void sendHeartbeat();

    void sendGPSRawInt(
        double latitude,
        double longitude,
        float altitude,
        float speedKmh,
        bool gpsFix);

    void sendBatteryStatus(
        float voltageV,
        float currentmA,
        float remainingPercent,
        bool connected);

    void sendGlobalPositionIntCov(
        double latitude,
        double longitude,
        float gpsAltitude,
        float relativeAltitude,
        float speedKmh);

    void sendSysStatus(
        float voltageV,
        float currentmA,
        float remainingPercent,
        bool connected);

    void sendVFRHUD(
        float groundSpeedKmh,
        float altitude,
        float heading);

    void sendGlobalPositionInt(
        double latitude,
        double longitude,
        float gpsAltitude,
        float relativeAltitude,
        float speedKmh);

    void sendHomePosition(
        double latitude,
        double longitude,
        float altitude);

private:
    uint8_t systemId;
    uint8_t componentId;

    // Climb rate estimation state
    float lastAltitude;
    uint32_t lastAltitudeTime;
    bool climbRateInitialized;
};

#endif