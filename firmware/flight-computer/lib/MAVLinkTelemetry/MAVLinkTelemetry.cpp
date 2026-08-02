#include "MAVLinkTelemetry.h"

MAVLinkTelemetry::MAVLinkTelemetry()
{
    systemId = 1;
    componentId = MAV_COMP_ID_AUTOPILOT1;

    lastAltitude = 0.0f;
    lastAltitudeTime = 0;
    climbRateInitialized = false;
}

void MAVLinkTelemetry::begin()
{
    Serial.println("[MAVLINK] Initialized");
}

void MAVLinkTelemetry::sendHeartbeat()
{
    mavlink_message_t msg;

    mavlink_msg_heartbeat_pack(
        systemId,
        componentId,
        &msg,
        MAV_TYPE_GENERIC,
        MAV_AUTOPILOT_GENERIC,
        0,
        0,
        MAV_STATE_ACTIVE);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    uint16_t len =
        mavlink_msg_to_send_buffer(
            buffer,
            &msg);

    Serial.write(
        buffer,
        len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println(
        "[MAVLINK] HEARTBEAT sent");

#endif
}

void MAVLinkTelemetry::sendGPSRawInt(
    double latitude,
    double longitude,
    float altitude,
    float speedKmh,
    bool gpsFix)
{
    mavlink_message_t msg;

    uint64_t timeUsec =
        millis() * 1000ULL;

    uint8_t fixType =
        gpsFix ? 3 : 1;

    int32_t lat =
        (int32_t)(latitude * 10000000.0);

    int32_t lon =
        (int32_t)(longitude * 10000000.0);

    int32_t alt =
        (int32_t)(altitude * 1000.0);

    uint16_t vel =
        (uint16_t)((speedKmh / 3.6f) * 100.0f);

    mavlink_msg_gps_raw_int_pack(
        systemId,
        componentId,
        &msg,
        timeUsec,
        fixType,
        lat,
        lon,
        alt,
        UINT16_MAX,
        UINT16_MAX,
        vel,
        UINT16_MAX,
        UINT8_MAX,
        alt,
        0,
        0,
        0,
        0,
        0);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    uint16_t len =
        mavlink_msg_to_send_buffer(
            buffer,
            &msg);

    Serial.write(
        buffer,
        len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println();
    Serial.println("===== MAVLINK GPS_RAW_INT =====");

    Serial.print("Lat : ");
    Serial.println(latitude, 6);

    Serial.print("Lon : ");
    Serial.println(longitude, 6);

    Serial.print("Alt : ");
    Serial.println(altitude, 1);

    Serial.print("Fix : ");
    Serial.println(gpsFix ? "YES" : "NO");

    Serial.println("==============================");
    Serial.println();

#endif
}

void MAVLinkTelemetry::sendBatteryStatus(
    float voltageV,
    float currentmA,
    float remainingPercent,
    bool connected)
{
    mavlink_message_t msg;

    // MAVLink BATTERY_STATUS.voltages[10]
    // Cells 1 to 10, in mV.
    // Unused cells MUST be UINT16_MAX.
    uint16_t voltages[10];

    for (int i = 0; i < 10; i++)
    {
        voltages[i] = UINT16_MAX;
    }

    if (connected)
    {
        voltages[0] =
            (uint16_t)(voltageV * 1000.0f);
    }

    // MAVLink uses centi-amps.
    // 1 cA = 10 mA.
    int16_t batteryCurrent =
        connected
            ? (int16_t)(currentmA / 10.0f)
            : -1;

    int8_t batteryRemaining =
        connected
            ? (int8_t)remainingPercent
            : -1;

    // MAVLink BATTERY_STATUS.voltages_ext[4]
    // Cells 11 to 14, in mV.
    // Unused cells MUST be 0, not UINT16_MAX.
    // Using UINT16_MAX here makes ground stations add
    // 4 x 65535 mV = 262.14 V to the reported pack voltage.
    uint16_t voltagesExt[4];

    for (int i = 0; i < 4; i++)
    {
        voltagesExt[i] = 0;
    }

    uint8_t chargeState =
        connected
            ? MAV_BATTERY_CHARGE_STATE_OK
            : MAV_BATTERY_CHARGE_STATE_UNDEFINED;

    mavlink_msg_battery_status_pack(
        systemId,
        componentId,
        &msg,
        0,
        MAV_BATTERY_FUNCTION_ALL,
        MAV_BATTERY_TYPE_LIPO,
        INT16_MAX,
        voltages,
        batteryCurrent,
        -1,
        -1,
        batteryRemaining,
        0,
        chargeState,
        voltagesExt,
        0,
        0);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    uint16_t len =
        mavlink_msg_to_send_buffer(
            buffer,
            &msg);

    Serial.write(
        buffer,
        len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println();
    Serial.println("===== MAVLINK BATTERY_STATUS =====");

    Serial.print("Voltage : ");
    Serial.print(voltageV, 2);
    Serial.println(" V");

    Serial.print("Current : ");
    Serial.print(currentmA, 2);
    Serial.println(" mA");

    Serial.print("SOC     : ");
    Serial.print(remainingPercent, 1);
    Serial.println(" %");

    Serial.print("Conn    : ");
    Serial.println(connected ? "YES" : "NO");

    Serial.println("==================================");
    Serial.println();

#endif
}

void MAVLinkTelemetry::sendGlobalPositionInt(
    double latitude,
    double longitude,
    float gpsAltitude,
    float relativeAltitude,
    float speedKmh)
{
    mavlink_message_t msg;

    uint32_t timeBootMs = millis();

    int32_t lat =
        (int32_t)(latitude * 10000000.0);

    int32_t lon =
        (int32_t)(longitude * 10000000.0);

    // Altitudes are expressed in millimetres.
    int32_t alt =
        (int32_t)(gpsAltitude * 1000.0f);

    // MAVLink GLOBAL_POSITION_INT.relative_alt is the
    // altitude above the home position, not above sea level.
    int32_t relAlt =
        (int32_t)(relativeAltitude * 1000.0f);

    // Velocity components are expressed in cm/s.
    int16_t vx =
        (int16_t)((speedKmh / 3.6f) * 100.0f);

    int16_t vy = 0;
    int16_t vz = 0;

    // Heading is expressed in centi-degrees.
    // UINT16_MAX indicates unknown heading.
    uint16_t hdg = UINT16_MAX;

    mavlink_msg_global_position_int_pack(
        systemId,
        componentId,
        &msg,
        timeBootMs,
        lat,
        lon,
        alt,
        relAlt,
        vx,
        vy,
        vz,
        hdg);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    uint16_t len =
        mavlink_msg_to_send_buffer(
            buffer,
            &msg);

    Serial.write(
        buffer,
        len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println();
    Serial.println("===== MAVLINK GLOBAL_POSITION_INT =====");

    Serial.print("Lat        : ");
    Serial.println(latitude, 6);

    Serial.print("Lon        : ");
    Serial.println(longitude, 6);

    Serial.print("GPS Alt    : ");
    Serial.print(gpsAltitude, 1);
    Serial.println(" m");

    Serial.print("Relative   : ");
    Serial.print(relativeAltitude, 2);
    Serial.println(" m");

    Serial.println("=======================================");
    Serial.println();

#endif
}

void MAVLinkTelemetry::sendGlobalPositionIntCov(
    double latitude,
    double longitude,
    float gpsAltitude,
    float relativeAltitude,
    float speedKmh)
{
    mavlink_message_t msg;

    uint64_t timeUsec =
        millis() * 1000ULL;

    int32_t lat =
        (int32_t)(latitude * 10000000.0);

    int32_t lon =
        (int32_t)(longitude * 10000000.0);

    int32_t alt =
        (int32_t)(gpsAltitude * 1000.0);

    int32_t relAlt =
        (int32_t)(relativeAltitude * 1000.0);

    float vx =
        speedKmh / 3.6f;

    float vy = 0.0f;

    float vz = 0.0f;

    float covariance[36];

    for (int i = 0; i < 36; i++)
    {
        covariance[i] = NAN;
    }

    mavlink_msg_global_position_int_cov_pack(
        systemId,
        componentId,
        &msg,
        timeUsec,
        MAV_ESTIMATOR_TYPE_GPS,
        lat,
        lon,
        alt,
        relAlt,
        vx,
        vy,
        vz,
        covariance);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    uint16_t len =
        mavlink_msg_to_send_buffer(
            buffer,
            &msg);

    Serial.write(
        buffer,
        len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println();
    Serial.println("===== MAVLINK GLOBAL_POSITION_INT_COV =====");

    Serial.print("Lat : ");
    Serial.println(latitude, 6);

    Serial.print("Lon : ");
    Serial.println(longitude, 6);

    Serial.print("GPS Alt : ");
    Serial.println(gpsAltitude, 1);

    Serial.print("Flight Alt : ");
    Serial.println(relativeAltitude, 1);

    Serial.print("Speed : ");
    Serial.println(speedKmh, 1);

    Serial.println("===========================================");
    Serial.println();

#endif
}

void MAVLinkTelemetry::sendSysStatus(
    float voltageV,
    float currentmA,
    float remainingPercent,
    bool connected)
{
    mavlink_message_t msg;

    uint32_t onboardControlSensorsPresent = 0;
    uint32_t onboardControlSensorsEnabled = 0;
    uint32_t onboardControlSensorsHealth = 0;

    uint16_t load = 0;

    uint16_t voltageBattery =
        connected
            ? (uint16_t)(voltageV * 1000.0f)
            : UINT16_MAX;

    // MAVLink SYS_STATUS current_battery uses centi-amps.
    // 1 cA = 10 mA.
    int16_t currentBattery =
        connected
            ? (int16_t)(currentmA / 10.0f)
            : -1;

    int8_t batteryRemaining =
        connected
            ? (int8_t)remainingPercent
            : -1;

    uint16_t dropRateComm = 0;
    uint16_t errorsComm = 0;

    uint16_t errorsCount1 = 0;
    uint16_t errorsCount2 = 0;
    uint16_t errorsCount3 = 0;
    uint16_t errorsCount4 = 0;

    uint32_t sensorsPresentExtended = 0;
    uint32_t sensorsEnabledExtended = 0;
    uint32_t sensorsHealthExtended = 0;

    mavlink_msg_sys_status_pack(
        systemId,
        componentId,
        &msg,
        onboardControlSensorsPresent,
        onboardControlSensorsEnabled,
        onboardControlSensorsHealth,
        load,
        voltageBattery,
        currentBattery,
        batteryRemaining,
        dropRateComm,
        errorsComm,
        errorsCount1,
        errorsCount2,
        errorsCount3,
        errorsCount4,
        sensorsPresentExtended,
        sensorsEnabledExtended,
        sensorsHealthExtended);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    uint16_t len =
        mavlink_msg_to_send_buffer(
            buffer,
            &msg);

    Serial.write(
        buffer,
        len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println();
    Serial.println("===== MAVLINK SYS_STATUS =====");

    Serial.print("Voltage : ");
    Serial.print(voltageV, 2);
    Serial.println(" V");

    Serial.print("Current : ");
    Serial.print(currentmA, 2);
    Serial.println(" mA");

    Serial.print("SOC     : ");
    Serial.print(remainingPercent, 1);
    Serial.println(" %");

    Serial.print("Conn    : ");
    Serial.println(connected ? "YES" : "NO");

    Serial.println("==============================");
    Serial.println();

#endif
}

void MAVLinkTelemetry::sendVFRHUD(
    float groundSpeedKmh,
    float altitude,
    float heading)
{
    mavlink_message_t msg;

    uint32_t now = millis();

    // Climb rate estimation from successive altitude samples.
    // MAVLink VFR_HUD.climb is expressed in m/s.
    float climbRate = 0.0f;

    if (climbRateInitialized)
    {
        uint32_t deltaTimeMs =
            now - lastAltitudeTime;

        if (deltaTimeMs > 0)
        {
            float deltaAltitude =
                altitude - lastAltitude;

            climbRate =
                deltaAltitude / (deltaTimeMs / 1000.0f);
        }
    }
    else
    {
        climbRateInitialized = true;
    }

    lastAltitude = altitude;
    lastAltitudeTime = now;

    // MAVLink VFR_HUD.groundspeed is expressed in m/s.
    float groundSpeedMs =
        groundSpeedKmh / 3.6f;

    // Airspeed is not measured by this platform.
    float airspeed = 0.0f;

    // MAVLink VFR_HUD.heading is expressed in degrees (0 to 359).
    int16_t headingDeg =
        (int16_t)heading;

    if (headingDeg < 0)
    {
        headingDeg += 360;
    }

    headingDeg = headingDeg % 360;

    // Throttle is not applicable to this platform.
    uint16_t throttle = 0;

    mavlink_msg_vfr_hud_pack(
        systemId,
        componentId,
        &msg,
        airspeed,
        groundSpeedMs,
        headingDeg,
        throttle,
        altitude,
        climbRate);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    uint16_t len =
        mavlink_msg_to_send_buffer(
            buffer,
            &msg);

    Serial.write(
        buffer,
        len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println();
    Serial.println("===== MAVLINK VFR_HUD =====");

    Serial.print("Ground Speed : ");
    Serial.print(groundSpeedMs, 2);
    Serial.println(" m/s");

    Serial.print("Altitude     : ");
    Serial.print(altitude, 2);
    Serial.println(" m");

    Serial.print("Climb Rate   : ");
    Serial.print(climbRate, 2);
    Serial.println(" m/s");

    Serial.print("Heading      : ");
    Serial.print(headingDeg);
    Serial.println(" deg");

    Serial.println("===========================");
    Serial.println();

#endif
}