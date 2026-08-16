#include "SerialProfileReplay.h"

#include <stdlib.h>
#include <string.h>

// =====================================================
// Constructor / Initialization
// =====================================================

SerialProfileReplay::SerialProfileReplay()
{
    stream = nullptr;
    reset();
}

void SerialProfileReplay::begin(
    Stream& serial)
{
    stream = &serial;
    reset();
}

void SerialProfileReplay::reset()
{
    lineLength = 0;

    head = 0;
    tail = 0;
    count = 0;

    active = false;
    ended = false;

    receivedSamples = 0;

    lineBuffer[0] = '\0';
}

// =====================================================
// State
// =====================================================

bool SerialProfileReplay::isActive() const
{
    return active;
}

bool SerialProfileReplay::hasEnded() const
{
    return ended;
}

bool SerialProfileReplay::hasSample() const
{
    return count > 0;
}

// =====================================================
// Queue Handling
// =====================================================

bool SerialProfileReplay::popSample(
    SerialReplaySample& sample)
{
    if (count == 0)
    {
        return false;
    }

    sample = queue[tail];

    tail = (tail + 1) % QUEUE_SIZE;
    count--;

    return true;
}

bool SerialProfileReplay::pushSample(
    const SerialReplaySample& sample)
{
    if (count >= QUEUE_SIZE)
    {
        // Queue full. Drop the sample safely.
        // The sender can always replay the profile again.
        return false;
    }

    queue[head] = sample;

    head = (head + 1) % QUEUE_SIZE;
    count++;

    return true;
}

// =====================================================
// Serial Processing
// =====================================================

void SerialProfileReplay::update()
{
    if (stream == nullptr)
    {
        return;
    }

    while (stream->available() > 0)
    {
        char c = (char)stream->read();

        if (c == '\r')
        {
            continue;
        }

        if (c == '\n')
        {
            lineBuffer[lineLength] = '\0';

            if (lineLength > 0)
            {
                processLine(lineBuffer);
            }

            lineLength = 0;
            lineBuffer[0] = '\0';

            continue;
        }

        if (lineLength < LINE_BUFFER_SIZE - 1)
        {
            lineBuffer[lineLength] = c;
            lineLength++;
        }
        else
        {
            // Line too long. Drop it safely and wait
            // for the next newline.
            lineLength = 0;
            lineBuffer[0] = '\0';
        }
    }
}

void SerialProfileReplay::processLine(
    const char* line)
{
    if (strcmp(line, "PROFILE_START") == 0)
    {
        handleStart();
        return;
    }

    if (strcmp(line, "PROFILE_END") == 0)
    {
        handleEnd();
        return;
    }

    if (!active)
    {
        return;
    }

    // Expected replay sample format:
    //
    // R,<time_ms>,<altitude_m>
    //
    // Example:
    // R,1250,42.37

    if (line[0] != 'R' || line[1] != ',')
    {
        return;
    }

    char workingLine[LINE_BUFFER_SIZE];
    strncpy(
        workingLine,
        line,
        sizeof(workingLine));

    workingLine[sizeof(workingLine) - 1] = '\0';

    char* firstComma =
        strchr(
            workingLine,
            ',');

    if (firstComma == nullptr)
    {
        return;
    }

    char* secondComma =
        strchr(
            firstComma + 1,
            ',');

    if (secondComma == nullptr)
    {
        return;
    }

    *secondComma = '\0';

    const char* timeText =
        firstComma + 1;

    const char* altitudeText =
        secondComma + 1;

    uint32_t timeMs =
        (uint32_t)strtoul(
            timeText,
            nullptr,
            10);

    float altitudeM =
        (float)atof(
            altitudeText);

    SerialReplaySample sample;
    sample.timeMs = timeMs;
    sample.altitudeM = altitudeM;

    if (pushSample(sample))
    {
        receivedSamples++;
    }
}

// =====================================================
// Protocol Commands
// =====================================================

void SerialProfileReplay::handleStart()
{
    active = true;
    ended = false;

    head = 0;
    tail = 0;
    count = 0;

    receivedSamples = 0;

    lineLength = 0;
    lineBuffer[0] = '\0';

    if (stream != nullptr)
    {
        stream->println("[REPLAY] START");
    }
}

void SerialProfileReplay::handleEnd()
{
    active = false;
    ended = true;

    if (stream != nullptr)
    {
        stream->print("[REPLAY] END | samples=");
        stream->println(receivedSamples);
    }
}