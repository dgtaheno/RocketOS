#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

// =====================================================
// Buzzer Note
// =====================================================

struct BuzzerNote
{
    uint16_t onMs;
    uint16_t offMs;
};

// =====================================================
// Buzzer Patterns
// =====================================================

enum BuzzerPattern
{
    BUZZER_NONE = 0,

    BUZZER_STARTUP,
    BUZZER_GPS_LOCK,

    BUZZER_IGNITION,
    BUZZER_APOGEE,

    BUZZER_LOCATOR,
    BUZZER_ALARM
};

// =====================================================
// Buzzer
// =====================================================

class Buzzer
{
public:
    Buzzer();

    void begin(uint8_t pin);

    void update();

    void play(BuzzerPattern pattern);

    void startLocator();

    void stop();

    bool isPlaying() const;

private:
    void loadPattern(
        const BuzzerNote* notes,
        uint8_t count,
        bool repeat);

    uint8_t buzzerPin;

    const BuzzerNote* currentPattern;

    uint8_t patternLength;
    uint8_t currentNote;

    bool repeating;
    bool noteActive;

    uint32_t phaseStart;

    bool playing;
};

#endif