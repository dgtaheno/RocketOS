#pragma once

#include <Arduino.h>
#include <Adafruit_INA219.h>

class INA219Sensor
{
public:
    INA219Sensor();

    bool begin();
    bool update();

    bool isHealthy() const;

    float getBusVoltage_V() const;
    float getCurrent_mA() const;
    float getPower_mW() const;

private:
    Adafruit_INA219 ina219;

    bool healthy;

    float busVoltage_V;
    float current_mA;
    float power_mW;
};