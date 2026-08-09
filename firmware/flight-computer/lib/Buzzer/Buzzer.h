#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

// A single note in a pattern: sound for onMs, then silence for offMs.
struct BuzzerNote
{
    uint16_t onMs;
    uint16_t offMs;
};

// Flight event sounds.
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

// Non-blocking active buzzer driver.
// Call update() every loop; patterns play in the background.
class Buzzer
{
public:
    Buzzer();

    void begin(uint8_t pin);
    void update();                    // call every loop, non-blocking

    void play(BuzzerPattern pattern); // play a one-shot pattern
    void startLocator();              // repeating locator beacon
    void stop();

    bool isPlaying() const;

private:
    void loadPattern(const BuzzerNote* notes, uint8_t count, bool repeat);

    uint8_t buzzerPin;

    const BuzzerNote* currentPattern;
    uint8_t patternLength;
    uint8_t currentNote;
    bool repeating;

    bool noteActive;      // true = in the "on" phase, false = "off" phase
    uint32_t phaseStart;  // millis() when the current phase began
    bool playing;
};

#endif
