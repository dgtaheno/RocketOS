#include <gtest/gtest.h>
#include "FlightStateMachine.h"
#include <cmath>

// --------------------------------------------------
// Synthetic flight profile with parachute recovery.
//
// Models a model-rocketry ballistic flight:
//   powered boost -> ballistic coast -> apogee -> chute descent.
//
// The parachute caps descent at a low terminal velocity,
// as real recovery systems do.
// --------------------------------------------------
struct FlightProfile
{
    float t = 0, alt = 0, vel = 0;
    bool finished = false;
    bool apogeePassed = false;

    static constexpr float BOOST_S = 1.5f;
    static constexpr float BOOST_A = 30.0f;
    static constexpr float G = 9.81f;
    static constexpr float CHUTE_TERMINAL = -5.0f;

    float step(float dt)
    {
        if (finished) return alt;

        if (t < BOOST_S)
        {
            // Powered ascent.
            vel += (BOOST_A - G) * dt;
        }
        else if (!apogeePassed)
        {
            // Ballistic coast up to apogee.
            vel += (-G) * dt;
            if (vel <= 0.0f) apogeePassed = true;
        }
        else
        {
            // Parachute descent: ease velocity toward terminal.
            vel += (CHUTE_TERMINAL - vel) * 0.5f;
        }

        alt += vel * dt;
        t += dt;

        if (alt <= 0.0f && apogeePassed)
        {
            alt = 0;
            vel = 0;
            finished = true;
        }
        return alt;
    }
};

// --------------------------------------------------
// Helper: run a full flight followed by a
// resting-on-ground phase.
//
// After touchdown the vehicle rests on the ground while
// the flight computer keeps logging, so altitude = 0
// samples are fed until LANDED confirms.
// --------------------------------------------------
static void runFullFlight(FlightStateMachine& fsm, bool visited[6])
{
    FlightProfile p;
    uint32_t ms = 0;
    const uint32_t stepMs = 100;

    // Flight phase.
    for (int i = 0; i < 300 && !p.finished; ++i)
    {
        float alt = p.step(stepMs / 1000.0f);
        ms += stepMs;
        fsm.update(alt, ms);
        visited[fsm.getState()] = true;
    }

    // Resting-on-ground phase (2 s of altitude = 0).
    for (int i = 0; i < 20; ++i)
    {
        ms += stepMs;
        fsm.update(0.0f, ms);
        visited[fsm.getState()] = true;
    }
}

// --------------------------------------------------
// Test cases
// --------------------------------------------------

// The full synthetic flight must walk through every state
// and finish LANDED, with a physically sensible apogee.
TEST(FlightStateMachine, FullFlightSequence)
{
    FlightStateMachine fsm;
    bool visited[6] = {false, false, false, false, false, false};

    runFullFlight(fsm, visited);

    EXPECT_TRUE(visited[FLIGHT_BOOST]);
    EXPECT_TRUE(visited[FLIGHT_COAST]);
    EXPECT_TRUE(visited[FLIGHT_APOGEE]);
    EXPECT_TRUE(visited[FLIGHT_DESCENT]);
    EXPECT_EQ(fsm.getState(), FLIGHT_LANDED);
    EXPECT_GT(fsm.getApogeeAltitude(), 30.0f);
}

// Barometric noise while stationary must not trigger a
// false launch.
TEST(FlightStateMachine, IgnoresBarometricNoiseAtIdle)
{
    FlightStateMachine fsm;
    uint32_t ms = 0;
    float noise[] = {0.1f, -0.2f, 0.3f, -0.1f, 0.2f,
                     -0.3f, 0.1f, 0.0f, -0.2f, 0.2f};

    for (int i = 0; i < 50; ++i)
    {
        ms += 100;
        fsm.update(noise[i % 10], ms);
    }

    EXPECT_EQ(fsm.getState(), FLIGHT_IDLE);
}

// A slow rise (e.g. lifting the board by hand) must not
// be interpreted as a launch.
TEST(FlightStateMachine, DoesNotLaunchOnSlowRise)
{
    FlightStateMachine fsm;
    uint32_t ms = 0;
    float alt = 0;

    // 1 m/s rise, below the 5 m/s launch threshold.
    for (int i = 0; i < 100; ++i)
    {
        ms += 100;
        alt += 0.1f;
        fsm.update(alt, ms);
    }

    EXPECT_EQ(fsm.getState(), FLIGHT_IDLE);
}

// A fast vertical rise must leave IDLE (launch detected).
TEST(FlightStateMachine, LaunchesOnFastRise)
{
    FlightStateMachine fsm;
    uint32_t ms = 0;
    float alt = 0;

    // 15 m/s rise, above the launch threshold.
    for (int i = 0; i < 20; ++i)
    {
        ms += 100;
        alt += 1.5f;
        fsm.update(alt, ms);
    }

    EXPECT_NE(fsm.getState(), FLIGHT_IDLE);
}

// The recorded apogee altitude must match the peak altitude.
TEST(FlightStateMachine, ApogeeAltitudeMatchesPeak)
{
    FlightStateMachine fsm;
    bool visited[6] = {false, false, false, false, false, false};

    runFullFlight(fsm, visited);

    EXPECT_NEAR(fsm.getApogeeAltitude(), fsm.getMaxAltitude(), 2.0f);
}

// reset() must return the machine to a clean IDLE state.
TEST(FlightStateMachine, ResetReturnsToIdle)
{
    FlightStateMachine fsm;
    bool visited[6] = {false, false, false, false, false, false};

    runFullFlight(fsm, visited);
    fsm.reset();

    EXPECT_EQ(fsm.getState(), FLIGHT_IDLE);
    EXPECT_FLOAT_EQ(fsm.getMaxAltitude(), 0.0f);
}

// --------------------------------------------------
// Test entry point
// --------------------------------------------------

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}