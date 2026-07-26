#pragma once

#include <Arduino.h>

class SDLogger
{
private:
    char logFileName[64];

public:
    // =====================================================
    // Initialization
    // =====================================================

    bool begin();

    bool selfTest();

    // =====================================================
    // Log File Creation
    // =====================================================

    // Creates a fallback boot-based log file:
    // /flight_boot_001.csv
    bool createLogFile();

    // Creates a GPS timestamp-based log file:
    // /flight_2026-01-01_13-34-56.csv
    bool createLogFile(
        const String& localDate,
        const String& localTime);

    const char* getLogFileName();

    // =====================================================
    // Telemetry Logging
    // =====================================================

    bool writeData(
        unsigned long timestamp,
        float temperature,
        float pressure,
        float bmpAltitude,
        bool gpsFix,
        double latitude,
        double longitude,
        float gpsAltitude,
        float flightAltitude,
        float speed,
        float batteryVoltage,
        float current_mA,
        float power_mW,
        float batterySoc);

    // =====================================================
    // Storage Diagnostics
    // =====================================================

    uint64_t getTotalBytes();

    uint64_t getUsedBytes();

    uint64_t getFreeBytes();

    uint8_t getUsagePercent();

    bool isStorageAvailable();

    bool isStorageWarning();

    bool isStorageCritical();

    bool isStorageFull();
};