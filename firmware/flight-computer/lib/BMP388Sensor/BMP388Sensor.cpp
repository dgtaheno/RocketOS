#include "BMP388Sensor.h"

#include "../../include/Config.h"

#include <Arduino.h>
#include <Wire.h>

#define SEALEVELPRESSURE_HPA 1013.25

bool BMP388Sensor::begin()
{
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!bmp.begin_I2C(BMP388_I2C_ADDRESS))
    {
        return false;
    }

    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

    delay(3000);

    for (int i = 0; i < 5; i++)
    {
        bmp.performReading();
        delay(200);
    }

    altitudeOffset = bmp.readAltitude(SEALEVELPRESSURE_HPA);

    update();

    return true;
}

bool BMP388Sensor::selfTest()
{
    if (!bmp.performReading())
    {
        return false;
    }

    float temperature = bmp.temperature;
    float pressure = bmp.pressure / 100.0f;

    if (temperature < -40.0f || temperature > 85.0f)
    {
        return false;
    }

    if (pressure < 300.0f || pressure > 1200.0f)
    {
        return false;
    }

    return true;
}

bool BMP388Sensor::update()
{
    if (!bmp.performReading())
    {
        return false;
    }

    temperature = bmp.temperature;

    pressure = bmp.pressure / 100.0f;

    relativeAltitude =
        bmp.readAltitude(SEALEVELPRESSURE_HPA)
        - altitudeOffset;

    return true;
}

float BMP388Sensor::getTemperature()
{
    return temperature;
}

float BMP388Sensor::getPressure()
{
    return pressure;
}

float BMP388Sensor::getRelativeAltitude()
{
    return relativeAltitude;
}