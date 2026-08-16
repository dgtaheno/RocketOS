#include <gtest/gtest.h>
#include "FlightStateMachine.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// =====================================================
// Flight Profile Replay Testing
//
// Data-driven test: every profile in test_profiles/ is
// replayed through the FlightStateMachine and its detected
// apogee is checked against the expected value in the .meta.
//
// To add a new test, just drop a <name>.csv + <name>.meta
// into test_profiles/. No code changes required.
// =====================================================

// Folder holding the profiles, relative to the project root.
// PlatformIO runs native tests from the project root.
#ifndef PROFILE_DIR
#define PROFILE_DIR "test_profiles/"
#endif

struct FlightProfile
{
    std::string name;
    std::string description;
    std::string source;
    float expectedApogee = 0.0f;
    float tolerance = 5.0f;
    std::vector<std::pair<float, float>> samples;  // (time_s, altitude_m)
};

// --- Minimal CSV loader: expects header "time_s,altitude_m" ---
static bool loadCsv(const std::string& path, FlightProfile& p)
{
    FILE* f = fopen(path.c_str(), "r");
    if (!f) return false;

    char line[256];
    if (!fgets(line, sizeof(line), f))  // skip header
    {
        fclose(f);
        return false;
    }

    while (fgets(line, sizeof(line), f))
    {
        float t, a;
        if (sscanf(line, "%f,%f", &t, &a) == 2)
        {
            p.samples.push_back({t, a});
        }
    }
    fclose(f);
    return !p.samples.empty();
}

// --- Minimal .meta loader: "key: value" lines ---
static bool loadMeta(const std::string& path, FlightProfile& p)
{
    FILE* f = fopen(path.c_str(), "r");
    if (!f) return false;

    char line[256];
    while (fgets(line, sizeof(line), f))
    {
        char key[64], value[192];
        if (sscanf(line, "%63[^:]: %191[^\n]", key, value) == 2)
        {
            std::string k(key);
            if (k == "name")               p.description = value;
            else if (k == "source")        p.source = value;
            else if (k == "expected_apogee_m") p.expectedApogee = atof(value);
            else if (k == "tolerance_m")   p.tolerance = atof(value);
        }
    }
    fclose(f);
    return true;
}

// --- Discover all profiles listed in the manifest ---
// A simple manifest keeps discovery portable across platforms
// (no directory-listing needed). One profile name per line.
static std::vector<FlightProfile> loadAllProfiles()
{
    std::vector<FlightProfile> profiles;

    const std::string dir = PROFILE_DIR;
    FILE* mf = fopen((dir + "manifest.txt").c_str(), "r");
    if (!mf)
    {
        return profiles;  // no manifest -> no tests
    }

    char name[128];
    while (fgets(name, sizeof(name), mf))
    {
        // strip newline / whitespace
        std::string n(name);
        while (!n.empty() && (n.back() == '\n' || n.back() == '\r' || n.back() == ' '))
            n.pop_back();
        if (n.empty() || n[0] == '#') continue;

        FlightProfile p;
        p.name = n;
        if (loadCsv(dir + n + ".csv", p) && loadMeta(dir + n + ".meta", p))
        {
            profiles.push_back(std::move(p));
        }
    }
    fclose(mf);
    return profiles;
}

// =====================================================
// Parameterized test
// =====================================================

class FlightProfileTest
    : public ::testing::TestWithParam<FlightProfile>
{
};

// Replays the profile and checks the detected apogee and sequence.
TEST_P(FlightProfileTest, DetectsApogeeWithinTolerance)
{
    const FlightProfile& p = GetParam();

    FlightStateMachine fsm;
    bool visited[6] = {false, false, false, false, false, false};

    for (const auto& s : p.samples)
    {
        uint32_t ms = static_cast<uint32_t>(s.first * 1000.0f);
        fsm.update(s.second, ms);
        visited[fsm.getState()] = true;
    }

    // The full flight sequence must have occurred.
    EXPECT_TRUE(visited[FLIGHT_BOOST])   << p.name << ": never reached BOOST";
    EXPECT_TRUE(visited[FLIGHT_COAST])   << p.name << ": never reached COAST";
    EXPECT_TRUE(visited[FLIGHT_APOGEE])  << p.name << ": never reached APOGEE";
    EXPECT_TRUE(visited[FLIGHT_DESCENT]) << p.name << ": never reached DESCENT";

    // Detected apogee must match the expected value within tolerance.
    EXPECT_NEAR(fsm.getApogeeAltitude(), p.expectedApogee, p.tolerance)
        << p.name << ": apogee out of tolerance";
}

// Human-readable test names in the output.
static std::string profileName(
    const ::testing::TestParamInfo<FlightProfile>& info)
{
    return info.param.name;
}

INSTANTIATE_TEST_SUITE_P(
    RealFlightProfiles,
    FlightProfileTest,
    ::testing::ValuesIn(loadAllProfiles()),
    profileName);

// --- Test entry point (self-provided main for portability) ---
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
