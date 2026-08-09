#include "Buzzer.h"

// =====================================================
// Flight event patterns
// =====================================================

// STARTUP — accelerating countdown then confirmation.
// pi . pi . pi. pi.PIIII  (beeps speeding up + long tone)
static const BuzzerNote PATTERN_STARTUP[] = {
    {60, 200}, {60, 150}, {60, 100}, {60, 60}, {350, 0}
};

// GPS LOCK — two quick beeps.
static const BuzzerNote PATTERN_GPS_LOCK[] = {
    {50, 60}, {50, 0}
};

// IGNITION — one long powerful tone.
static const BuzzerNote PATTERN_IGNITION[] = {
    {600, 0}
};

// APOGEE — three short accelerating + one long.
static const BuzzerNote PATTERN_APOGEE[] = {
    {60, 60}, {60, 60}, {60, 60}, {400, 0}
};

// LOCATOR — double beep beacon, repeated.
// pi-pi ...... pi-pi ......
static const BuzzerNote PATTERN_LOCATOR[] = {
    {100, 120}, {100, 1400}
};

// ALARM — triple beep, repeated.
static const BuzzerNote PATTERN_ALARM[] = {
    {80, 80}, {80, 80}, {80, 400}
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