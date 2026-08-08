#include <Arduino.h>
#include <math.h>

#include "Config.h"
#include "BMP388Sensor.h"
#include "SDLogger.h"
#include "GPSSensor.h"
#include "SystemHealth.h"
#include "SystemEvents.h"
#include "BufferedLogger.h"
#include "INA219Sensor.h"
#include "BatteryMonitor.h"
#include "MAVLinkTelemetry.h"
#include "FlightStateMachine.h"
#include "FlightSimulator.h"

// --------------------------------------------------
// System Modules
// --------------------------------------------------

BMP388Sensor bmp;
SDLogger logger;
GPSSensor gps;
SystemHealth health;
SystemEvents events;
BufferedLogger telemetryBuffer;
INA219Sensor ina219Sensor;
BatteryMonitor batteryMonitor;
MAVLinkTelemetry mavlink;
FlightStateMachine flightState;

// --------------------------------------------------
// Timing
// --------------------------------------------------

unsigned long lastLog = 0;
unsigned long lastHealthPrint = 0;
unsigned long lastHeartbeat = 0;
bool flightSimDone = false;

// --------------------------------------------------
// GPS Altitude Reference
// --------------------------------------------------

bool gpsReferenceCaptured = false;
float gpsReferenceAltitude = 0.0f;

// --------------------------------------------------
// Home Position (MAVLink)
// --------------------------------------------------

double gpsHomeLatitude = 0.0;
double gpsHomeLongitude = 0.0;
bool homePositionSent = false;
unsigned long lastHomePosition = 0;

// --------------------------------------------------
// GPS Health State Tracking
// --------------------------------------------------

bool gpsWarningActive = false;
GpsErrorCode activeGpsWarning = GPS_ERROR_NONE;

// --------------------------------------------------
// Battery Event State Tracking
// --------------------------------------------------

bool batteryPreviouslyConnected = false;

BatteryState previousBatteryState =
    BATTERY_DISCONNECTED;

// --------------------------------------------------
// MAVLink Cached Battery Telemetry
// --------------------------------------------------

float mavBatteryVoltage = 0.0f;
float mavCurrent_mA = 0.0f;
float mavBatterySoc = 0.0f;
bool mavBatteryConnected = false;

// --------------------------------------------------
// Helper: Write Telemetry Record to SD
// --------------------------------------------------

bool writeTelemetryRecordToSd(
    const TelemetryRecord& record)
{
    return logger.writeData(
        record.timestamp,
        record.temperature,
        record.pressure,
        record.bmpAltitude,
        record.gpsFix,
        record.latitude,
        record.longitude,
        record.gpsAltitude,
        record.flightAltitude,
        record.speed,
        record.batteryVoltage,
        record.current_mA,
        record.power_mW,
        record.batterySoc);
}

// --------------------------------------------------
// Helper: Flush Buffered Telemetry Records
// --------------------------------------------------

bool flushBufferedRecords()
{
    if (telemetryBuffer.isEmpty())
    {
        return true;
    }

    events.logEvent(
        EVENT_BUFFER_FLUSH_STARTED);

    while (!telemetryBuffer.isEmpty())
    {
        TelemetryRecord pendingRecord;

        // IMPORTANT:
        // Use peek() first to preserve FIFO order.
        // Only remove the record from the buffer
        // after it has been written successfully.
        if (!telemetryBuffer.peek(pendingRecord))
        {
            return false;
        }

        bool writeOk =
            writeTelemetryRecordToSd(
                pendingRecord);

        events.updateSdState(
            true,
            writeOk,
            logger.isStorageFull());

        if (!writeOk)
        {
            return false;
        }

        TelemetryRecord discardedRecord;

        telemetryBuffer.pop(
            discardedRecord);
    }

    events.logEvent(
        EVENT_BUFFER_FLUSH_COMPLETED);

    return true;
}

// --------------------------------------------------
// Helper: Update SD Health Information
// --------------------------------------------------

void updateSdHealth()
{
    uint64_t totalBytes = logger.getTotalBytes();
    uint64_t usedBytes = logger.getUsedBytes();

    if (events.isSdRemoved())
    {
        health.reportSdFailure(
            SD_ERROR_CARD_REMOVED);

        return;
    }

    if (totalBytes == 0)
    {
        health.reportSdFailure(
            SD_ERROR_NOT_INITIALIZED);

        return;
    }

    health.updateSdStorage(
        totalBytes,
        usedBytes);
}

// --------------------------------------------------
// Helper: Report GPS Health Without Counter Spam
// --------------------------------------------------

void reportGpsHealth(
    bool gpsDetected,
    bool gpsFix)
{
    if (!gpsDetected)
    {
        if (!gpsWarningActive ||
            activeGpsWarning != GPS_ERROR_NO_DATA)
        {
            health.reportGpsWarning(
                GPS_ERROR_NO_DATA);

            gpsWarningActive = true;
            activeGpsWarning = GPS_ERROR_NO_DATA;
        }

        return;
    }

    if (!gpsFix)
    {
        if (!gpsWarningActive ||
            activeGpsWarning != GPS_ERROR_NO_FIX)
        {
            health.reportGpsWarning(
                GPS_ERROR_NO_FIX);

            gpsWarningActive = true;
            activeGpsWarning = GPS_ERROR_NO_FIX;
        }

        return;
    }

    health.reportGpsOk();

    gpsWarningActive = false;
    activeGpsWarning = GPS_ERROR_NONE;
}

// --------------------------------------------------
// Helper: Update Battery Events Without Spam
// --------------------------------------------------

void updateBatteryEvents()
{
    bool batteryConnected =
        batteryMonitor.isConnected();

    BatteryState currentBatteryState =
        batteryMonitor.getState();

    // --------------------------------------------------
    // Battery connection state changes
    // --------------------------------------------------

    if (!batteryPreviouslyConnected &&
         batteryConnected)
    {
        events.logEvent(
            EVENT_BATTERY_CONNECTED);
    }
    else if (batteryPreviouslyConnected &&
            !batteryConnected)
    {
        events.logEvent(
            EVENT_BATTERY_DISCONNECTED);
    }

    // --------------------------------------------------
    // Battery warning / critical state changes
    // --------------------------------------------------

    if (batteryConnected &&
        currentBatteryState != previousBatteryState)
    {
        if (currentBatteryState == BATTERY_WARNING)
        {
            events.logEvent(
                EVENT_BATTERY_WARNING);
        }
        else if (currentBatteryState == BATTERY_CRITICAL)
        {
            events.logEvent(
                EVENT_BATTERY_CRITICAL);
        }
    }

    batteryPreviouslyConnected =
        batteryConnected;

    previousBatteryState =
        currentBatteryState;
}

// --------------------------------------------------
// Helper: Print System Health Periodically
// --------------------------------------------------

void printHealthIfNeeded()
{
#if PRINT_HEALTH_TO_SERIAL

    if (millis() - lastHealthPrint >= HEALTH_PRINT_INTERVAL_MS)
    {
        lastHealthPrint = millis();

        health.printStatus();

        events.printSdState();

        Serial.println();
        Serial.println("Buffered Logging");
        Serial.println("--------------------------------");

        Serial.print("Buffered Records : ");
        Serial.println(telemetryBuffer.size());

        Serial.print("Buffer Capacity  : ");
        Serial.println(telemetryBuffer.capacity());

        Serial.print("Buffer Free      : ");
        Serial.println(telemetryBuffer.freeSpace());

        Serial.print("Dropped Records  : ");
        Serial.println(telemetryBuffer.getDroppedRecords());

        Serial.print("Total Buffered   : ");
        Serial.println(telemetryBuffer.getTotalBuffered());

        Serial.println("--------------------------------");
        Serial.println();
    }

#endif
}

// --------------------------------------------------
// Setup
// --------------------------------------------------

void setup()
{
    Serial.begin(115200);

    health.reset();

    events.begin();

    events.logEvent(
        EVENT_SYSTEM_START);

    Serial.println();
    Serial.println("================================");
    Serial.println(SYSTEM_NAME);
    Serial.println("================================");
    Serial.println();

    Serial.println("Running Power-On Self Test (POST)...");
    Serial.println();

    // --------------------------------------------------
    // BufferedLogger Self-Test
    // --------------------------------------------------

    Serial.println("BufferedLogger Self-Test");

    TelemetryRecord test = {};
    test.timestamp = 123;

    if (!telemetryBuffer.push(test))
    {
        Serial.println("[FAIL] BufferedLogger push");
    }
    else
    {
        TelemetryRecord out = {};

        if (!telemetryBuffer.pop(out))
        {
            Serial.println("[FAIL] BufferedLogger pop");
        }
        else if (out.timestamp != 123)
        {
            Serial.println("[FAIL] BufferedLogger FIFO");
        }
        else
        {
            Serial.println("[PASS] BufferedLogger");
        }
    }

    // --------------------------------------------------
    // BMP388
    // --------------------------------------------------

    if (!bmp.begin())
    {
        health.reportBmpFailure(
            BMP_ERROR_READ_FAILED);

        Serial.println("[FAIL] BMP388 sensor not detected");

#if HALT_ON_BMP388_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    if (!bmp.selfTest())
    {
        health.reportBmpFailure(
            BMP_ERROR_INVALID_DATA);

        Serial.println("[FAIL] BMP388 self-test failed");

#if HALT_ON_BMP388_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    health.reportBmpOk();

    Serial.println("[PASS] BMP388 sensor");

    // --------------------------------------------------
    // INA219
    // --------------------------------------------------

    if (!ina219Sensor.begin())
    {
        Serial.println("[WARN] INA219 power monitor not detected");
    }

    // --------------------------------------------------
    // SD Card
    // --------------------------------------------------

    if (!logger.begin())
    {
        health.reportSdFailure(
            SD_ERROR_NOT_INITIALIZED);

        Serial.println("[FAIL] SD card not detected");
        Serial.println();
        Serial.println("Please check:");
        Serial.println(" - microSD card is inserted");
        Serial.println(" - SD module wiring");
        Serial.println(" - SD module power supply (5V)");
        Serial.println(" - SD card format (FAT32)");

#if HALT_ON_SD_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    if (!logger.selfTest())
    {
        health.reportSdFailure(
            SD_ERROR_SELFTEST_FAILED);

        Serial.println("[FAIL] SD card self-test failed");

#if HALT_ON_SD_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    health.reportSdOk();
    updateSdHealth();

    Serial.println("[PASS] SD card");

#if GPS_ENABLED

    // --------------------------------------------------
    // GPS
    // --------------------------------------------------

    gps.begin();

    if (!gps.selfTest())
    {
        health.reportGpsWarning(
            GPS_ERROR_NO_DATA);

        gpsWarningActive = true;
        activeGpsWarning = GPS_ERROR_NO_DATA;

        Serial.println("[WARN] GPS receiver not detected");
    }
    else
    {
        health.reportGpsOk();

        gpsWarningActive = false;
        activeGpsWarning = GPS_ERROR_NONE;

        Serial.println("[PASS] GPS receiver");
    }

#endif

#if GPS_ENABLED

    // --------------------------------------------------
    // Wait for GPS Fix
    // --------------------------------------------------

    unsigned long gpsStart = millis();

    while ((millis() - gpsStart) <
           (GPS_FIX_TIMEOUT_SECONDS * 1000UL))
    {
        gps.update();

        if (gps.hasFix())
        {
            break;
        }

        delay(50);
    }

    reportGpsHealth(
        gps.isDetected(),
        gps.hasFix());

#endif

    // --------------------------------------------------
    // Create Flight Log
    // --------------------------------------------------

#if GPS_ENABLED

    if (gps.hasFix())
    {
        Serial.println("[INFO] Creating GPS timestamped log file");

        if (!logger.createLogFile(
                gps.getLocalDate(),
                gps.getLocalTime()))
        {
            health.reportSdFailure(
                SD_ERROR_FILE_CREATION_FAILED);

            Serial.println("[FAIL] Flight log creation failed");

#if HALT_ON_SD_FAIL
            while (1)
            {
                delay(1000);
            }
#endif
        }
    }
    else
    {
        Serial.println("[WARN] GPS fix unavailable");
        Serial.println("[INFO] Using fallback filename");

        if (!logger.createLogFile())
        {
            health.reportSdFailure(
                SD_ERROR_FILE_CREATION_FAILED);

            Serial.println("[FAIL] Flight log creation failed");

#if HALT_ON_SD_FAIL
            while (1)
            {
                delay(1000);
            }
#endif
        }
    }

#else

    if (!logger.createLogFile())
    {
        health.reportSdFailure(
            SD_ERROR_FILE_CREATION_FAILED);

        Serial.println("[FAIL] Flight log creation failed");

#if HALT_ON_SD_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

#endif

    health.reportSdOk();
    updateSdHealth();

    Serial.print("[PASS] Flight log: ");
    Serial.println(logger.getLogFileName());

    Serial.println();
    Serial.println("All systems passed");
    Serial.println("System READY");
    Serial.println();

    mavlink.begin();

    events.logEvent(
        EVENT_SYSTEM_READY);

    health.printStatus();
}

// --------------------------------------------------
// Main Loop
// --------------------------------------------------

void loop()
{
#if FLIGHT_SIMULATION_MODE

    // Deferred flight simulation.
    // Runs once, 15 seconds after boot, so QGroundControl has
    // time to connect and receive the STATUSTEXT events.
    static uint32_t lastSimRun = 0;
    if (millis() - lastSimRun > 30000)
    {
        lastSimRun = millis();

        Serial.println();
        Serial.println("===== FLIGHT SIMULATION =====");

        FlightSimulator sim;
        FlightStateMachine fsm;

        sim.reset();
        fsm.reset();

        uint32_t simTimeMs = 0;
        const uint32_t stepMs = 100;

        // Flight phase.
        while (!sim.isFinished() && simTimeMs < 30000)
        {
            float alt = sim.step(stepMs);
            simTimeMs += stepMs;

            fsm.update(alt, simTimeMs);

            if (fsm.hasStateChanged())
            {
                Serial.print("[");
                Serial.print(simTimeMs / 1000.0f, 1);
                Serial.print(" s] -> ");
                Serial.print(fsm.getStateString());
                Serial.print("  | alt = ");
                Serial.print(alt, 1);
                Serial.println(" m");

                char simMsg[50];

                if (fsm.getState() == FLIGHT_APOGEE)
                {
                    snprintf(simMsg, sizeof(simMsg),
                        "APOGEE DETECTED - %.1f m",
                        fsm.getApogeeAltitude());
                }
                else
                {
                    snprintf(simMsg, sizeof(simMsg),
                        "FLIGHT STATE: %s",
                        fsm.getStateString());
                }

                // Keep the MAVLink link alive and send the event.
                mavlink.sendHeartbeat();
                mavlink.sendStatusText(6, simMsg);

                delay(400);   // Let QGC display each notification
            }
        }

        // Resting-on-ground phase.
        for (int i = 0; i < 20; i++)
        {
            simTimeMs += stepMs;
            fsm.update(0.0f, simTimeMs);

            if (fsm.hasStateChanged())
            {
                mavlink.sendHeartbeat();
                mavlink.sendStatusText(6, "FLIGHT STATE: LANDED");
                delay(400);
            }
        }

        Serial.print("Apogee: ");
        Serial.print(fsm.getApogeeAltitude(), 1);
        Serial.println(" m");
        Serial.println("=============================");
    }

#endif

#if GPS_ENABLED
    gps.update();
#endif

    // --------------------------------------------------
    // MAVLink Periodic Telemetry
    // --------------------------------------------------

    if (millis() - lastHeartbeat >= 1000)
    {
        lastHeartbeat = millis();

    #if GPS_ENABLED

        double mavLatitude =
            gps.getLatitude();

        double mavLongitude =
            gps.getLongitude();

        float mavGpsAltitude =
            gps.getAltitude();

        float mavGpsSpeed =
            gps.getSpeed();

        bool mavGpsFix =
            gps.hasFix();

        float mavRelativeAltitude =
            gpsReferenceCaptured
                ? gpsReferenceAltitude +
                bmp.getRelativeAltitude()
                : mavGpsAltitude;

        // Height above the barometric reference point.
        // MAVLink GLOBAL_POSITION_INT.relative_alt expects
        // altitude above home, not altitude above sea level.
        float mavHeightAboveGround =
            bmp.getRelativeAltitude();

    #else

        double mavLatitude = 0.0;
        double mavLongitude = 0.0;

        float mavGpsAltitude = 0.0f;
        float mavGpsSpeed = 0.0f;
        bool mavGpsFix = false;

        float mavRelativeAltitude = 0.0f;
        float mavHeightAboveGround = 0.0f;

    #endif

        // GPS speed deadband.
        // Prevents ground station instruments from showing
        // residual speed noise while the vehicle is stationary.
        if (fabs(mavGpsSpeed) < GPS_SPEED_DEADBAND_KMH)
        {
            mavGpsSpeed = 0.0f;
        }

        mavlink.sendHeartbeat();

        mavlink.sendGPSRawInt(
            mavLatitude,
            mavLongitude,
            mavGpsAltitude,
            mavGpsSpeed,
            mavGpsFix);

        mavlink.sendBatteryStatus(
            mavBatteryVoltage,
            mavCurrent_mA,
            mavBatterySoc,
            mavBatteryConnected);

        mavlink.sendGlobalPositionInt(
            mavLatitude,
            mavLongitude,
            mavGpsAltitude,
            mavHeightAboveGround,
            mavGpsSpeed);

        mavlink.sendGlobalPositionIntCov(
            mavLatitude,
            mavLongitude,
            mavGpsAltitude,
            mavRelativeAltitude,
            mavGpsSpeed);

        mavlink.sendSysStatus(
            mavBatteryVoltage,
            mavCurrent_mA,
            mavBatterySoc,
            mavBatteryConnected);

        mavlink.sendVFRHUD(
            mavGpsSpeed,
            mavRelativeAltitude,
            0.0f);

        // Re-emit HOME_POSITION every 5 seconds once captured.
        // Some QGroundControl versions request it periodically.
        if (homePositionSent &&
            millis() - lastHomePosition >= 5000)
        {
            lastHomePosition = millis();

            mavlink.sendHomePosition(
                gpsHomeLatitude,
                gpsHomeLongitude,
                gpsReferenceAltitude);
        }
    }
 
    if (millis() - lastLog >= LOG_INTERVAL_MS)
    {
        lastLog = millis();

        // --------------------------------------------------
        // BMP388 Update
        // --------------------------------------------------

        if (!bmp.update())
        {
            health.reportBmpFailure(
                BMP_ERROR_READ_FAILED);

            events.updateBmpState(
                false);

            Serial.println("[WARN] BMP388 read error");

            printHealthIfNeeded();

            return;
        }

        health.reportBmpOk();

        events.updateBmpState(
            true);

        float temperature = bmp.getTemperature();
        float pressure = bmp.getPressure();
        float bmpAltitude = bmp.getRelativeAltitude();

        // --------------------------------------------------
        // Flight State Machine (barometric, autonomous)
        //
        // Fed from the barometric relative altitude so it
        // operates independently of GPS fix. Detects launch,
        // burnout, apogee, descent and landing, and reports
        // each transition to the ground station via STATUSTEXT.
        // --------------------------------------------------

        flightState.update(bmpAltitude, millis());

        if (flightState.hasStateChanged())
        {
            const char* stateName =
                flightState.getStateString();

            Serial.print("[FLIGHT] State -> ");
            Serial.println(stateName);

            char flightMsg[50];

            if (flightState.getState() == FLIGHT_APOGEE)
            {
                snprintf(flightMsg, sizeof(flightMsg),
                    "APOGEE DETECTED - %.1f m",
                    flightState.getApogeeAltitude());
            }
            else
            {
                snprintf(flightMsg, sizeof(flightMsg),
                    "FLIGHT STATE: %s",
                    stateName);
            }

            // MAV_SEVERITY_INFO = 6
            mavlink.sendStatusText(6, flightMsg);
        }

        // --------------------------------------------------
        // INA219 Update
        // --------------------------------------------------

        bool ina219Ok = ina219Sensor.update();

        float batteryVoltage = 0.0f;
        float current_mA = 0.0f;
        float power_mW = 0.0f;

        if (ina219Ok)
        {
            batteryVoltage =
                ina219Sensor.getBusVoltage_V();

            current_mA =
                ina219Sensor.getCurrent_mA();

            power_mW =
                ina219Sensor.getPower_mW();

            batteryMonitor.update(
                batteryVoltage);
        }
        else
        {
            batteryMonitor.update(
                0.0f);
        }

        updateBatteryEvents();

        mavBatteryVoltage = batteryVoltage;
        mavCurrent_mA = current_mA;
        mavBatterySoc = batteryMonitor.getPercentage();
        mavBatteryConnected = batteryMonitor.isConnected();

#if GPS_ENABLED

        // --------------------------------------------------
        // GPS Update
        // --------------------------------------------------

        bool gpsDetected = gps.isDetected();
        bool gpsFix = gps.hasFix();

        events.updateGpsState(
            gpsDetected,
            gpsFix);

        reportGpsHealth(
            gpsDetected,
            gpsFix);

        double latitude = gps.getLatitude();
        double longitude = gps.getLongitude();

        float gpsAltitude = gps.getAltitude();
        float gpsSpeed = gps.getSpeed();

        // --------------------------------------------------
        // GPS Speed Deadband
        // --------------------------------------------------

        if (fabs(gpsSpeed) < GPS_SPEED_DEADBAND_KMH)
        {
            gpsSpeed = 0.0f;
        }

#else

        bool gpsDetected = false;
        bool gpsFix = false;

        double latitude = 0.0;
        double longitude = 0.0;

        float gpsAltitude = 0.0f;
        float gpsSpeed = 0.0f;

#endif

        // --------------------------------------------------
        // Capture GPS Reference Altitude and Home Position
        // --------------------------------------------------

        if (gpsFix && !gpsReferenceCaptured)
        {
            gpsReferenceAltitude = gpsAltitude;
            gpsHomeLatitude = latitude;
            gpsHomeLongitude = longitude;
            gpsReferenceCaptured = true;

            Serial.print("[INFO] GPS reference altitude captured: ");
            Serial.print(gpsReferenceAltitude, 1);
            Serial.println(" m");

            // Send HOME_POSITION once, as soon as the reference
            // point is captured on the first GPS fix.
            mavlink.sendHomePosition(
                gpsHomeLatitude,
                gpsHomeLongitude,
                gpsReferenceAltitude);

            homePositionSent = true;
            lastHomePosition = millis();
        }

        // --------------------------------------------------
        // Flight Altitude
        // --------------------------------------------------

        float flightAltitude = 0.0f;

        if (gpsReferenceCaptured)
        {
            flightAltitude =
                gpsReferenceAltitude +
                bmpAltitude;
        }
        else
        {
            flightAltitude = gpsAltitude;
        }

        // --------------------------------------------------
        // Create Telemetry Record
        // --------------------------------------------------

        TelemetryRecord record = {};

        record.timestamp = millis() / 1000;

        record.temperature = temperature;
        record.pressure = pressure;
        record.bmpAltitude = bmpAltitude;

        record.gpsFix = gpsFix;

        record.latitude = latitude;
        record.longitude = longitude;

        record.gpsAltitude = gpsAltitude;
        record.flightAltitude = flightAltitude;

        record.speed = gpsSpeed;

        record.batteryVoltage = batteryVoltage;
        record.current_mA = current_mA;
        record.power_mW = power_mW;
        record.batterySoc = batteryMonitor.getPercentage();

        // --------------------------------------------------
        // Store Telemetry
        // --------------------------------------------------

        bool writeOk = false;

        if (!telemetryBuffer.isEmpty())
        {
            if (!telemetryBuffer.push(record))
            {
                events.logEvent(
                    EVENT_BUFFER_OVERFLOW);
            }

            writeOk =
                flushBufferedRecords();
        }
        else
        {
            writeOk =
                writeTelemetryRecordToSd(record);

            events.updateSdState(
                true,
                writeOk,
                logger.isStorageFull());

            if (!writeOk)
            {
                if (!telemetryBuffer.push(record))
                {
                    events.logEvent(
                        EVENT_BUFFER_OVERFLOW);
                }
                else
                {
                    events.logEvent(
                        EVENT_BUFFER_RECORD_STORED);
                }
            }
        }

        // --------------------------------------------------
        // Health Reporting
        // --------------------------------------------------

        if (writeOk)
        {
            health.reportSdOk();
        }
        else
        {
            if (events.isSdRemoved())
            {
                health.reportSdFailure(
                    SD_ERROR_CARD_REMOVED);
            }
            else if (logger.isStorageFull())
            {
                health.reportSdFailure(
                    SD_ERROR_CARD_FULL);
            }
            else
            {
                health.reportSdFailure(
                    SD_ERROR_WRITE_FAILED);
            }
        }

        updateSdHealth();

#if PRINT_TELEMETRY_TO_SERIAL

        // --------------------------------------------------
        // Console Output
        // --------------------------------------------------

        Serial.println("--------------------------------");

        Serial.print("Temperature : ");
        Serial.print(temperature, 2);
        Serial.println(" C");

        Serial.print("Pressure    : ");
        Serial.print(pressure, 2);
        Serial.println(" hPa");

        Serial.print("BMP Alt     : ");
        Serial.print(bmpAltitude, 2);
        Serial.println(" m");

        Serial.print("Flight State: ");
        Serial.println(flightState.getStateString());

        Serial.println();

        if (ina219Ok)
        {
            Serial.print("Battery V   : ");
            Serial.print(batteryVoltage, 2);
            Serial.println(" V");

            Serial.print("Current     : ");
            Serial.print(current_mA, 2);
            Serial.println(" mA");

            Serial.print("Power       : ");
            Serial.print(power_mW, 2);
            Serial.println(" mW");

            Serial.print("Battery Conn: ");
            Serial.println(
                batteryMonitor.isConnected()
                    ? "YES"
                    : "NO");

            Serial.print("Battery SOC : ");
            Serial.print(
                batteryMonitor.getPercentage(),
                1);
            Serial.println(" %");

            Serial.print("Battery St. : ");
            Serial.println(
                batteryMonitor.getStateString());
        }
        else
        {
            Serial.println("INA219      : NOT AVAILABLE");

            Serial.print("Battery Conn: ");
            Serial.println("NO");

            Serial.print("Battery SOC : ");
            Serial.println("0.0 %");

            Serial.print("Battery St. : ");
            Serial.println("DISCONNECTED");
        }

#if GPS_ENABLED

        Serial.println();

        Serial.print("GPS Detected: ");
        Serial.println(gpsDetected ? "YES" : "NO");

        Serial.print("GPS Fix     : ");
        Serial.println(gpsFix ? "YES" : "NO");

        if (gpsFix)
        {
            Serial.print("Latitude    : ");
            Serial.println(latitude, 6);

            Serial.print("Longitude   : ");
            Serial.println(longitude, 6);

            Serial.print("GPS Alt     : ");
            Serial.print(gpsAltitude, 1);
            Serial.println(" m");

            Serial.print("Flight Alt  : ");
            Serial.print(flightAltitude, 2);
            Serial.println(" m");

            Serial.print("Speed       : ");
            Serial.print(gpsSpeed, 2);
            Serial.println(" km/h");

            Serial.print("UTC Date    : ");
            Serial.println(gps.getUtcDate());

            Serial.print("UTC Time    : ");
            Serial.println(gps.getUtcTime());

            Serial.print("Local Time  : ");
            Serial.println(gps.getLocalTime());
        }

#endif

        Serial.println("--------------------------------");
        Serial.println();

#endif

        printHealthIfNeeded();
    }
}