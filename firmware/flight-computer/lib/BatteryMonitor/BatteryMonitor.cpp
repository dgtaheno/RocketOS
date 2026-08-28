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

    // RocketOS Mk1 uses a 2S LiPo battery.
    //
    // With USB power only and the LiPo disconnected,
    // the current hardware has been measured at approximately
    // 4.12V - 4.13V by the INA219.
    //
    // 5.0V therefore provides a clear separation between:
    //   - USB-only / battery disconnected
    //   - a physically connected 2S LiPo
    if (batteryVoltage < 5.0f)
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

    // Basic 2S LiPo health thresholds.
    //
    // Equivalent per-cell thresholds:
    //   3.60V/cell -> WARNING
    //   3.40V/cell -> CRITICAL
    //
    // These preserve the same per-cell thresholds
    // previously used by the 4S implementation.
    if (batteryVoltage <= 6.8f)
    {
        batteryState = BATTERY_CRITICAL;
    }
    else if (batteryVoltage <= 7.2f)
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
    // Approximate resting-voltage SOC curve for a 2S LiPo pack.
    //
    // This preserves the same per-cell SOC curve previously
    // used by the validated 4S implementation:
    //
    // 4.20V/cell = 8.40V pack
    // 4.10V/cell = 8.20V pack
    // 4.00V/cell = 8.00V pack
    // 3.90V/cell = 7.80V pack
    // 3.80V/cell = 7.60V pack
    // 3.70V/cell = 7.40V pack
    // 3.60V/cell = 7.20V pack
    // 3.50V/cell = 7.00V pack
    // 3.40V/cell = 6.80V pack
    // 3.30V/cell = 6.60V pack

    static const SocPoint curve[] =
    {
        {8.40f, 100.0f},
        {8.20f,  90.0f},
        {8.00f,  80.0f},
        {7.80f,  70.0f},
        {7.60f,  60.0f},
        {7.40f,  50.0f},
        {7.20f,  40.0f},
        {7.00f,  25.0f},
        {6.80f,  10.0f},
        {6.60f,   0.0f}
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