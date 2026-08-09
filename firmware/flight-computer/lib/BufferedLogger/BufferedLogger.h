#pragma once

#include <Arduino.h>

#ifndef BUFFERED_LOGGER_CAPACITY
#define BUFFERED_LOGGER_CAPACITY 100
#endif

struct TelemetryRecord
{
    uint32_t timestamp;
    float temperature;
    float pressure;
    float bmpAltitude;
    float extTemperature;
    uint8_t flightState;
    bool gpsFix;
    double latitude;
    double longitude;
    float gpsAltitude;
    float flightAltitude;
    float speed;
    float batteryVoltage;
    float current_mA;
    float power_mW;
    float batterySoc;
};

class BufferedLogger
{
public:
    BufferedLogger();

    bool push(
        const TelemetryRecord& record);

    bool pop(
        TelemetryRecord& record);

    bool peek(
        TelemetryRecord& record) const;

    void clear();

    bool isEmpty() const;
    bool isFull() const;

    size_t size() const;
    size_t capacity() const;
    size_t freeSpace() const;

    uint32_t getDroppedRecords() const;
    uint32_t getTotalBuffered() const;

private:
    TelemetryRecord buffer[BUFFERED_LOGGER_CAPACITY];

    size_t head;
    size_t tail;
    size_t count;

    uint32_t droppedRecords;
    uint32_t totalBuffered;
};