#include "Buzzer.h"

// =====================================================
// RocketOS Sound Identity
// Active Buzzer Edition
// =====================================================

// BOOT
// ti-ti-ti ... tiiii ... TIIIIIIII
static const BuzzerNote PATTERN_STARTUP[] = {
    {80, 80},
    {80, 80},
    {80, 200},
    {250, 150},
    {600, 0}
};

// GPS LOCK
// ti ... ti
static const BuzzerNote PATTERN_GPS_LOCK[] = {
    {100, 150},
    {100, 0}
};

// IGNITION
// TIIIIIIIIIIII
static const BuzzerNote PATTERN_IGNITION[] = {
    {700, 0}
};

// APOGEE
// ti-ti-ti-TIIII
static const BuzzerNote PATTERN_APOGEE[] = {
    {60, 60},
    {60, 60},
    {60, 60},
    {300, 0}
};

// LOCATOR
// ti-ti ..... (repeat)
static const BuzzerNote PATTERN_LOCATOR[] = {
    {150, 120},
    {150, 2000}
};

// ALARM
// TIIII TIIII TIIII
static const BuzzerNote PATTERN_ALARM[] = {
    {300, 100},
    {300, 100},
    {300, 800}
};

// =====================================================
// Constructor
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

// =====================================================
// Begin
// =====================================================

void Buzzer::begin(uint8_t pin)
{
    buzzerPin = pin;

    pinMode(buzzerPin, OUTPUT);

    digitalWrite(buzzerPin, LOW);
}

// =====================================================
// Load Pattern
// =====================================================

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

    digitalWrite(buzzerPin, HIGH);
}

// =====================================================
// Play Pattern
// =====================================================

void Buzzer::play(BuzzerPattern pattern)
{
    switch (pattern)
    {
        case BUZZER_STARTUP:

            loadPattern(
                PATTERN_STARTUP,
                sizeof(PATTERN_STARTUP) / sizeof(BuzzerNote),
                false);

            break;

        case BUZZER_GPS_LOCK:

            loadPattern(
                PATTERN_GPS_LOCK,
                sizeof(PATTERN_GPS_LOCK) / sizeof(BuzzerNote),
                false);

            break;

        case BUZZER_IGNITION:

            loadPattern(
                PATTERN_IGNITION,
                sizeof(PATTERN_IGNITION) / sizeof(BuzzerNote),
                false);

            break;

        case BUZZER_APOGEE:

            loadPattern(
                PATTERN_APOGEE,
                sizeof(PATTERN_APOGEE) / sizeof(BuzzerNote),
                false);

            break;

        case BUZZER_ALARM:

            loadPattern(
                PATTERN_ALARM,
                sizeof(PATTERN_ALARM) / sizeof(BuzzerNote),
                false);

            break;

        case BUZZER_LOCATOR:

            startLocator();

            break;

        case BUZZER_NONE:
        default:

            stop();

            break;
    }
}

// =====================================================
// Locator
// =====================================================

void Buzzer::startLocator()
{
    loadPattern(
        PATTERN_LOCATOR,
        sizeof(PATTERN_LOCATOR) / sizeof(BuzzerNote),
        true);
}

// =====================================================
// Stop
// =====================================================

void Buzzer::stop()
{
    playing = false;

    currentPattern = nullptr;

    digitalWrite(buzzerPin, LOW);
}

// =====================================================
// State
// =====================================================

bool Buzzer::isPlaying() const
{
    return playing;
}

// =====================================================
// Update
// Non-blocking state machine.
// Must be called regularly from loop().
// =====================================================

void Buzzer::update()
{
    if (!playing || currentPattern == nullptr)
    {
        return;
    }

    uint32_t now = millis();

    const BuzzerNote& note =
        currentPattern[currentNote];

    if (noteActive)
    {
        if (now - phaseStart >= note.onMs)
        {
            digitalWrite(buzzerPin, LOW);

            noteActive = false;

            phaseStart = now;

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