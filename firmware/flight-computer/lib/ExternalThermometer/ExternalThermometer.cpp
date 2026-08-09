#include "ExternalThermometer.h"

// DS18B20 returns this when disconnected or failing to read.
static const float DS18B20_ERROR_TEMP = -127.0f;

ExternalThermometer::ExternalThermometer()
    : oneWire(), sensors(&oneWire)
{
    temperature = 0.0f;
    connected = false;
    initialized = false;
}

bool ExternalThermometer::begin(uint8_t pin)
{
    oneWire.begin(pin);
    sensors.begin();

    // Check whether a device is present on the bus.
    uint8_t count = sensors.getDeviceCount();

    if (count == 0)
    {
        connected = false;
        initialized = false;
        return false;
    }

    // Use 12-bit resolution (0.0625 C), the default and most precise.
    sensors.setResolution(12);

    // Non-blocking mode: we request, then read on the next update.
    sensors.setWaitForConversion(true);

    initialized = true;
    connected = true;

    return true;
}

bool ExternalThermometer::update()
{
    if (!initialized)
    {
        connected = false;
        return false;
    }

    sensors.requestTemperatures();

    float t = sensors.getTempCByIndex(0);

    if (t <= DS18B20_ERROR_TEMP)
    {
        connected = false;
        return false;
    }

    temperature = t;
    connected = true;

    return true;
}

float ExternalThermometer::getTemperature() const
{
    return temperature;
}

bool ExternalThermometer::isConnected() const
{
    return connected;
}
