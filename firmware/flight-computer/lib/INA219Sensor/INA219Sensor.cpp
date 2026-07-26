#include "INA219Sensor.h"

INA219Sensor::INA219Sensor()
    : healthy(false),
      busVoltage_V(0.0f),
      current_mA(0.0f),
      power_mW(0.0f)
{
}

bool INA219Sensor::begin()
{
    healthy = ina219.begin();

    if (!healthy)
    {
        Serial.println("[FAIL] INA219 sensor");
        return false;
    }

    Serial.println("[PASS] INA219 sensor");
    return true;
}

bool INA219Sensor::update()
{
    if (!healthy)
    {
        return false;
    }

    busVoltage_V = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();

    return true;
}

bool INA219Sensor::isHealthy() const
{
    return healthy;
}

float INA219Sensor::getBusVoltage_V() const
{
    return busVoltage_V;
}

float INA219Sensor::getCurrent_mA() const
{
    return current_mA;
}

float INA219Sensor::getPower_mW() const
{
    return power_mW;
}