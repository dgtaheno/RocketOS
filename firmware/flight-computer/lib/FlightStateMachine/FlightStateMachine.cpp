#include "FlightStateMachine.h"
#include <cmath>

FlightStateMachine::FlightStateMachine()
{
    reset();
}

void FlightStateMachine::reset()
{
    state = FLIGHT_IDLE;
    stateChanged = false;

    initialized = false;

    lastAltitude = 0.0f;
    lastTimeMs = 0;

    climbRate = 0.0f;
    peakClimbRate = 0.0f;

    maxAltitude = 0.0f;
    apogeeAltitude = 0.0f;

    launchCounter = 0;
    burnoutCounter = 0;
    apogeeCounter = 0;
    descentCounter = 0;
    landedCounter = 0;
}

void FlightStateMachine::transitionTo(
    FlightState newState)
{
    state = newState;
    stateChanged = true;
}

void FlightStateMachine::update(
    float altitudeM,
    uint32_t nowMs)
{
    stateChanged = false;

    // First sample only initializes the reference.
    if (!initialized)
    {
        lastAltitude = altitudeM;
        lastTimeMs = nowMs;
        initialized = true;

        return;
    }

    float dt = (nowMs - lastTimeMs) / 1000.0f;

    if (dt <= 0.0f)
    {
        return;
    }

    // Climb rate from successive altitude samples,
    // smoothed with a low-pass filter to reject noise.
    float rawClimb =
        (altitudeM - lastAltitude) / dt;

    climbRate =
        (CLIMB_FILTER_ALPHA * rawClimb) +
        ((1.0f - CLIMB_FILTER_ALPHA) * climbRate);

    lastAltitude = altitudeM;
    lastTimeMs = nowMs;

    if (altitudeM > maxAltitude)
    {
        maxAltitude = altitudeM;
    }

    // --------------------------------------------------
    // State transitions
    // --------------------------------------------------

    switch (state)
    {
        case FLIGHT_IDLE:

            if (climbRate > LAUNCH_CLIMB_MS &&
                altitudeM > LAUNCH_ALTITUDE_M)
            {
                launchCounter++;
            }
            else
            {
                launchCounter = 0;
            }

            if (launchCounter >= LAUNCH_CONFIRM)
            {
                peakClimbRate = climbRate;
                transitionTo(FLIGHT_BOOST);
            }

            break;

        case FLIGHT_BOOST:

            // Motor burnout is detected when the climb rate
            // stops increasing (starts decelerating).
            if (climbRate > peakClimbRate)
            {
                peakClimbRate = climbRate;
                burnoutCounter = 0;
            }
            else
            {
                burnoutCounter++;
            }

            if (burnoutCounter >= BURNOUT_CONFIRM)
            {
                transitionTo(FLIGHT_COAST);
            }

            break;

        case FLIGHT_COAST:

            // Apogee: vertical speed crosses zero at altitude.
            if (climbRate <= 0.0f &&
                altitudeM > APOGEE_MIN_ALTITUDE_M)
            {
                apogeeCounter++;
            }
            else
            {
                apogeeCounter = 0;
            }

            if (apogeeCounter >= APOGEE_CONFIRM)
            {
                apogeeAltitude = maxAltitude;
                transitionTo(FLIGHT_APOGEE);
            }

            break;

        case FLIGHT_APOGEE:

            // Apogee is momentary; move to descent once
            // a sustained negative climb rate is confirmed.
            if (climbRate < DESCENT_CLIMB_MS)
            {
                descentCounter++;
            }
            else
            {
                descentCounter = 0;
            }

            if (descentCounter >= DESCENT_CONFIRM)
            {
                transitionTo(FLIGHT_DESCENT);
            }

            break;

        case FLIGHT_DESCENT:

            if (altitudeM < LANDED_ALTITUDE_M &&
                fabs(climbRate) < LANDED_CLIMB_MS)
            {
                landedCounter++;
            }
            else
            {
                landedCounter = 0;
            }

            if (landedCounter >= LANDED_CONFIRM)
            {
                transitionTo(FLIGHT_LANDED);
            }

            break;

        case FLIGHT_LANDED:

            // Terminal state.
            break;
    }
}

FlightState FlightStateMachine::getState() const
{
    return state;
}

const char* FlightStateMachine::getStateString() const
{
    switch (state)
    {
        case FLIGHT_IDLE:    return "IDLE";
        case FLIGHT_BOOST:   return "BOOST";
        case FLIGHT_COAST:   return "COAST";
        case FLIGHT_APOGEE:  return "APOGEE";
        case FLIGHT_DESCENT: return "DESCENT";
        case FLIGHT_LANDED:  return "LANDED";
        default:             return "UNKNOWN";
    }
}

bool FlightStateMachine::hasStateChanged() const
{
    return stateChanged;
}

float FlightStateMachine::getClimbRate() const
{
    return climbRate;
}

float FlightStateMachine::getMaxAltitude() const
{
    return maxAltitude;
}

float FlightStateMachine::getApogeeAltitude() const
{
    return apogeeAltitude;
}