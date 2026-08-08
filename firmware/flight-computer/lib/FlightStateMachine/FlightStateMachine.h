#ifndef FLIGHT_STATE_MACHINE_H
#define FLIGHT_STATE_MACHINE_H

#include <cstdint>
// --------------------------------------------------
// FlightStateMachine
//
// Models a BALLISTIC flight profile as used in model
// rocketry: powered boost, unpowered coast, a single
// apogee, and parachute recovery.
//
// Detection relies on barometric altitude and derived
// vertical speed. It assumes an irreversible trajectory
// (the vehicle goes up once and comes down once).
//
// NOT intended for powered controlled flight such as
// multirotors or fixed-wing aircraft, where altitude
// changes are arbitrary and repeatable.
// --------------------------------------------------

// --------------------------------------------------
// Flight States
// --------------------------------------------------

enum FlightState
{
    FLIGHT_IDLE = 0,
    FLIGHT_BOOST,
    FLIGHT_COAST,
    FLIGHT_APOGEE,
    FLIGHT_DESCENT,
    FLIGHT_LANDED
};

class FlightStateMachine
{
public:
    FlightStateMachine();

    void reset();

    // Feed one altitude sample (relative altitude, in metres)
    // together with the current time in milliseconds.
    void update(
        float altitudeM,
        uint32_t nowMs);

    FlightState getState() const;

    const char* getStateString() const;

    // True only during the single update where the state changed.
    bool hasStateChanged() const;

    float getClimbRate() const;

    float getMaxAltitude() const;

    float getApogeeAltitude() const;

private:
    void transitionTo(FlightState newState);

    // --------------------------------------------------
    // Detection thresholds
    // (fixed for the initial implementation)
    // --------------------------------------------------

    static constexpr float LAUNCH_CLIMB_MS = 5.0f;
    static constexpr float LAUNCH_ALTITUDE_M = 3.0f;

    static constexpr float APOGEE_MIN_ALTITUDE_M = 5.0f;

    static constexpr float DESCENT_CLIMB_MS = -2.0f;

    static constexpr float LANDED_ALTITUDE_M = 2.0f;
    static constexpr float LANDED_CLIMB_MS = 0.5f;

    // Consecutive samples required to confirm a transition.
    static constexpr uint8_t LAUNCH_CONFIRM = 3;
    static constexpr uint8_t BURNOUT_CONFIRM = 3;
    static constexpr uint8_t APOGEE_CONFIRM = 2;
    static constexpr uint8_t DESCENT_CONFIRM = 3;
    static constexpr uint8_t LANDED_CONFIRM = 5;

    // Climb rate low-pass filter coefficient (0..1).
    static constexpr float CLIMB_FILTER_ALPHA = 0.3f;

    // --------------------------------------------------
    // State
    // --------------------------------------------------

    FlightState state;
    bool stateChanged;

    bool initialized;

    float lastAltitude;
    uint32_t lastTimeMs;

    float climbRate;
    float peakClimbRate;

    float maxAltitude;
    float apogeeAltitude;

    // --------------------------------------------------
    // Confirmation counters
    // --------------------------------------------------

    uint8_t launchCounter;
    uint8_t burnoutCounter;
    uint8_t apogeeCounter;
    uint8_t descentCounter;
    uint8_t landedCounter;
};

#endif