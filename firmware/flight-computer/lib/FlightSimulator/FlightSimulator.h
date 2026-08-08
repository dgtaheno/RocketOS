#ifndef FLIGHT_SIMULATOR_H
#define FLIGHT_SIMULATOR_H

#include <cstdint>

// Generates a synthetic ballistic flight profile with
// parachute recovery, used to validate the FlightStateMachine
// on real hardware without an actual launch.
//
//   powered boost -> ballistic coast -> apogee -> chute descent
//
class FlightSimulator
{
public:
    FlightSimulator();

    void reset();

    // Advance the simulation by dtMs milliseconds and
    // return the current altitude in metres.
    float step(uint32_t dtMs);

    bool isFinished() const;

    float getTime() const;
    float getVelocity() const;

private:
    static constexpr float BOOST_DURATION_S = 1.5f;
    static constexpr float BOOST_ACCEL_MS2 = 30.0f;
    static constexpr float GRAVITY_MS2 = 9.81f;
    static constexpr float CHUTE_TERMINAL_MS = -5.0f;

    float timeS;
    float altitude;
    float velocity;

    bool finished;
    bool apogeePassed;
};

#endif