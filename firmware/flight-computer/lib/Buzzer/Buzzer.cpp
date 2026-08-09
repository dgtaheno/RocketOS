#include "Buzzer.h"

// =====================================================
// Flight event patterns
// Original rhythmic patterns. Active buzzer: single tone,
// character comes from timing (staccato bursts).
// =====================================================

// STARTUP — "power-up chime": quick ascending chatter
// then a confirming tone. Signals the system is awake.
static const BuzzerNote PATTERN_STARTUP[] = {
    {40, 50}, {40, 40}, {30, 40}, {30, 30}, {60, 40}, {250, 250}
};

// GPS LOCK — "ready chirp": two quick excited beeps.
// Confirms GPS fix acquired.
static const BuzzerNote PATTERN_GPS_LOCK[] = {
    {40, 40}, {40, 40}, {90, 250}
};

// IGNITION — "launch tone": one long, strong beep.
// Marks liftoff detection.
static const BuzzerNote PATTERN_IGNITION[] = {
    {600, 300}
};

// APOGEE — "peak alert": rapid rising burst then a hold.
// Marks apogee detection. Highest priority sound.
static const BuzzerNote PATTERN_APOGEE[] = {
    {40, 40}, {40, 40}, {40, 40}, {40, 40}, {300, 300}
};

// LOCATOR — "recovery beacon": strong double beep, repeated.
// Kept deliberately clear and loud to find the vehicle
// on the ground. Not decorative.
static const BuzzerNote PATTERN_LOCATOR[] = {
    {150, 120}, {150, 1500}
};

// ALARM — "warning stutter": urgent triple burst, repeated.
// Used for critical battery or fault conditions.
static const BuzzerNote PATTERN_ALARM[] = {
    {40, 40}, {40, 40}, {40, 250}
};

// =====================================================

Buzzer::Buzzer()
{
    buzzerPin = 255;
    currentPattern = nullptr;
    patternLength = 0;
    currentNote = 0;
    repeating = false;
    noteActive = false;
    phaseStart = 0;
    playing = false;
}

void Buzzer::begin(uint8_t pin)
{
    buzzerPin = pin;
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
}

void Buzzer::loadPattern(
    const BuzzerNote* notes,
    uint8_t count,
    bool repeat)
{
    currentPattern = notes;
    patternLength = count;
    repeating = repeat;
    currentNote = 0;
    noteActive = true;
    phaseStart = millis();
    playing = true;

    // Start the first note immediately.
    digitalWrite(buzzerPin, HIGH);
}

void Buzzer::play(BuzzerPattern pattern)
{
    switch (pattern)
    {
        case BUZZER_STARTUP:
            loadPattern(PATTERN_STARTUP,
                sizeof(PATTERN_STARTUP) / sizeof(BuzzerNote), false);
            break;

        case BUZZER_GPS_LOCK:
            loadPattern(PATTERN_GPS_LOCK,
                sizeof(PATTERN_GPS_LOCK) / sizeof(BuzzerNote), false);
            break;

        case BUZZER_IGNITION:
            loadPattern(PATTERN_IGNITION,
                sizeof(PATTERN_IGNITION) / sizeof(BuzzerNote), false);
            break;

        case BUZZER_APOGEE:
            loadPattern(PATTERN_APOGEE,
                sizeof(PATTERN_APOGEE) / sizeof(BuzzerNote), false);
            break;

        case BUZZER_ALARM:
            loadPattern(PATTERN_ALARM,
                sizeof(PATTERN_ALARM) / sizeof(BuzzerNote), false);
            break;

        case BUZZER_LOCATOR:
            startLocator();
            break;

        default:
            stop();
            break;
    }
}

void Buzzer::startLocator()
{
    loadPattern(PATTERN_LOCATOR,
        sizeof(PATTERN_LOCATOR) / sizeof(BuzzerNote), true);
}

void Buzzer::stop()
{
    playing = false;
    currentPattern = nullptr;
    digitalWrite(buzzerPin, LOW);
}

bool Buzzer::isPlaying() const
{
    return playing;
}

void Buzzer::update()
{
    if (!playing || currentPattern == nullptr)
    {
        return;
    }

    uint32_t now = millis();
    const BuzzerNote& note = currentPattern[currentNote];

    if (noteActive)
    {
        // Currently sounding. Has the "on" time elapsed?
        if (now - phaseStart >= note.onMs)
        {
            digitalWrite(buzzerPin, LOW);
            noteActive = false;
            phaseStart = now;

            // If there is no off time, skip straight to next note.
            if (note.offMs == 0)
            {
                noteActive = true;
                currentNote++;

                if (currentNote >= patternLength)
                {
                    if (repeating)
                    {
                        currentNote = 0;
                    }
                    else
                    {
                        stop();
                        return;
                    }
                }

                digitalWrite(buzzerPin, HIGH);
                phaseStart = now;
            }
        }
    }
    else
    {
        // Currently silent. Has the "off" time elapsed?
        if (now - phaseStart >= note.offMs)
        {
            currentNote++;

            if (currentNote >= patternLength)
            {
                if (repeating)
                {
                    currentNote = 0;
                }
                else
                {
                    stop();
                    return;
                }
            }

            noteActive = true;
            digitalWrite(buzzerPin, HIGH);
            phaseStart = now;
        }
    }
}