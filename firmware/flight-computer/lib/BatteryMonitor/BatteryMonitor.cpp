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

    // The system uses a 4S LiPo battery.
    // When only USB is connected, the INA219 may report around 4V.
    // This must be treated as "battery disconnected".
    if (batteryVoltage < 10.0f)
    {
        batteryConnected = false;
        batteryPercentage = 0.0f;
        batteryState = BATTERY_DISCONNECTED;
        return;
    }

    batteryConnected = true;

    batteryPercentage =
        calculateSocFromVoltage(
            batteryVoltage);

    // Basic 4S LiPo health thresholds.
    // These values are intentionally conservative for flight telemetry use.
    if (batteryVoltage <= 13.6f)
    {
        batteryState = BATTERY_CRITICAL;
    }
    else if (batteryVoltage <= 14.4f)
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

        case BATTERY_DISCONNECTED:
        default:
            return "DISCONNECTED";
    }
}

float BatteryMonitor::calculateSocFromVoltage(
    float voltage) const
{
    // Approximate resting-voltage SOC curve for a 4S LiPo pack.
    // This is more realistic than a purely linear 13.2V-16.8V mapping.
    //
    // 4S LiPo:
    // 4.20V/cell = 16.8V
    // 3.30V/cell = 13.2V

    static const SocPoint curve[] =
    {
        {16.80f, 100.0f},
        {16.40f,  90.0f},
        {16.00f,  80.0f},
        {15.60f,  70.0f},
        {15.20f,  60.0f},
        {14.80f,  50.0f},
        {14.40f,  40.0f},
        {14.00f,  25.0f},
        {13.60f,  10.0f},
        {13.20f,   0.0f}
    };

    const size_t pointCount =
        sizeof(curve) / sizeof(curve[0]);

    if (voltage >= curve[0].voltage)
    {
        return 100.0f;
    }

    if (voltage <= curve[pointCount - 1].voltage)
    {
        return 0.0f;
    }

    for (size_t i = 0; i < pointCount - 1; i++)
    {
        float highVoltage = curve[i].voltage;
        float lowVoltage = curve[i + 1].voltage;

        float highSoc = curve[i].percentage;
        float lowSoc = curve[i + 1].percentage;

        if (voltage <= highVoltage &&
            voltage >= lowVoltage)
        {
            float rangeVoltage =
                highVoltage - lowVoltage;

            float position =
                (voltage - lowVoltage) /
                rangeVoltage;

            float soc =
                lowSoc +
                position * (highSoc - lowSoc);

            if (soc < 0.0f)
            {
                soc = 0.0f;
            }

            if (soc > 100.0f)
            {
                soc = 100.0f;
            }

            return soc;
        }
    }

    return 0.0f;
}