#pragma once

#include <Arduino.h>

extern "C"
{
    #include "../MAVLink/common/mavlink.h"
}

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

private:
    uint8_t systemId;
    uint8_t componentId;
};