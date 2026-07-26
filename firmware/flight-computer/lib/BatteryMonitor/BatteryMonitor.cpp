#include "BatteryMonitor.h"

BatteryMonitor::BatteryMonitor()
    : batteryVoltage(0.0f),
      batteryPercentage(0.0f),
      batteryConnected(false),
      batteryState(BATTERY_DISCONNECTED)
{
}

void BatteryMonitor::update(float voltage)
{
    batteryVoltage = voltage;

    if (batteryVoltage < 10.0f)
    {
        batteryConnected = false;
        batteryPercentage = 0.0f;
        batteryState = BATTERY_DISCONNECTED;
        return;
    }

    batteryConnected = true;

    float minVoltage = 13.2f;
    float maxVoltage = 16.8f;

    batteryPercentage =
        ((batteryVoltage - minVoltage) /
         (maxVoltage - minVoltage)) * 100.0f;

    if (batteryPercentage < 0.0f)
    {
        batteryPercentage = 0.0f;
    }

    if (batteryPercentage > 100.0f)
    {
        batteryPercentage = 100.0f;
    }

    if (batteryVoltage < 13.6f)
    {
        batteryState = BATTERY_CRITICAL;
    }
    else if (batteryVoltage < 14.0f)
    {
        batteryState = BATTERY_WARNING;
    }
    else
    {
        batteryState = BATTERY_OK;
    }
}

bool BatteryMonitor::isConnected() const
{
    return batteryConnected;
}

float BatteryMonitor::getVoltage() const
{
    return batteryVoltage;
}

float BatteryMonitor::getPercentage() const
{
    return batteryPercentage;
}

BatteryState BatteryMonitor::getState() const
{
    return batteryState;
}

const char* BatteryMonitor::getStateString() const
{
    switch (batteryState)
    {
        case BATTERY_OK:
            return "OK";

        case BATTERY_WARNING:
            return "WARNING";

        case BATTERY_CRITICAL:
            return "CRITICAL";

        default:
            return "DISCONNECTED";
    }
}