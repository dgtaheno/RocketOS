#ifndef EXTERNAL_THERMOMETER_H
#define EXTERNAL_THERMOMETER_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 external temperature sensor (1-Wire).
class ExternalThermometer
{
public:
    ExternalThermometer();

    bool begin(uint8_t pin);
    bool update();               // request and read a new sample

    float getTemperature() const; // last valid reading, in degrees C
    bool isConnected() const;

private:
    OneWire oneWire;
    DallasTemperature sensors;

    float temperature;
    bool connected;
    bool initialized;
};

#endif