#pragma once

#include <Arduino.h>

enum BatteryState
{
    BATTERY_DISCONNECTED,
    BATTERY_OK,
    BATTERY_WARNING,
    BATTERY_CRITICAL
};

class BatteryMonitor
{
public:
    BatteryMonitor();

    void update(float voltage);

    bool isConnected() const;

    float getVoltage() const;

    float getPercentage() const;

    BatteryState getState() const;

    const char* getStateString() const;

private:
    float batteryVoltage;
    float batteryPercentage;

    bool batteryConnected;

    BatteryState batteryState;
};