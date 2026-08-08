#include "FlightSimulator.h"

FlightSimulator::FlightSimulator()
{
    reset();
}

void FlightSimulator::reset()
{
    timeS = 0.0f;
    altitude = 0.0f;
    velocity = 0.0f;
    finished = false;
    apogeePassed = false;
}

float FlightSimulator::step(uint32_t dtMs)
{
    if (finished)
    {
        return altitude;
    }

    float dt = dtMs / 1000.0f;

    if (timeS < BOOST_DURATION_S)
    {
        // Powered ascent.
        velocity += (BOOST_ACCEL_MS2 - GRAVITY_MS2) * dt;
    }
    else if (!apogeePassed)
    {
        // Ballistic coast up to apogee.
        velocity += (-GRAVITY_MS2) * dt;

        if (velocity <= 0.0f)
        {
            apogeePassed = true;
        }
    }
    else
    {
        // Parachute descent: ease velocity toward terminal.
        velocity += (CHUTE_TERMINAL_MS - velocity) * 0.5f;
    }

    altitude += velocity * dt;
    timeS += dt;

    if (altitude <= 0.0f && apogeePassed)
    {
        altitude = 0.0f;
        velocity = 0.0f;
        finished = true;
    }

    return altitude;
}

bool FlightSimulator::isFinished() const
{
    return finished;
}

float FlightSimulator::getTime() const
{
    return timeS;
}

float FlightSimulator::getVelocity() const
{
    return velocity;
}