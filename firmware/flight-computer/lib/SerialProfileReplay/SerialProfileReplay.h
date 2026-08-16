#pragma once

#include <Arduino.h>

struct SerialReplaySample
{
    uint32_t timeMs;
    float altitudeM;
};

class SerialProfileReplay
{
public:
    SerialProfileReplay();

    void begin(Stream& serial);
    void update();

    bool isActive() const;
    bool hasEnded() const;

    bool hasSample() const;
    bool popSample(SerialReplaySample& sample);

    void reset();

private:
    static constexpr size_t LINE_BUFFER_SIZE = 96;
    static constexpr size_t QUEUE_SIZE = 16;

    Stream* stream;

    char lineBuffer[LINE_BUFFER_SIZE];
    size_t lineLength;

    SerialReplaySample queue[QUEUE_SIZE];
    size_t head;
    size_t tail;
    size_t count;

    bool active;
    bool ended;

    uint32_t receivedSamples;

    void processLine(const char* line);
    bool pushSample(const SerialReplaySample& sample);

    void handleStart();
    void handleEnd();
};