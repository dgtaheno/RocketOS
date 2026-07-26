#include "SDLogger.h"
#include "../../include/Config.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

// =====================================================
// Initialization
// =====================================================

bool SDLogger::begin()
{
    return SD.begin(SD_CS_PIN);
}

bool SDLogger::selfTest()
{
    const char* testFile = "/selftest.tmp";

    File file = SD.open(testFile, FILE_WRITE);

    if (!file)
    {
        return false;
    }

    file.println("SELF TEST OK");
    file.close();

    file = SD.open(testFile);

    if (!file)
    {
        return false;
    }

    file.close();

    SD.remove(testFile);

    return true;
}

// =====================================================
// Log File Creation
// =====================================================

bool SDLogger::createLogFile()
{
    char fileName[64];

    int index = 1;

    while (true)
    {
        sprintf(
            fileName,
            "/flight_boot_%03d.csv",
            index);

        if (!SD.exists(fileName))
        {
            break;
        }

        index++;
    }

    strcpy(logFileName, fileName);

    File file = SD.open(logFileName, FILE_WRITE);

    if (!file)
    {
        return false;
    }

    file.println(
        "timestamp_s,"
        "temperature_c,"
        "pressure_hpa,"
        "bmp_altitude_m,"
        "gps_fix,"
        "latitude,"
        "longitude,"
        "gps_altitude_m,"
        "flight_altitude_m,"
        "speed_kmh,"
        "battery_voltage_v,"
        "current_ma,"
        "power_mw,"
        "battery_soc_percent");

    file.close();

    return true;
}

bool SDLogger::createLogFile(
    const String& localDate,
    const String& localTime)
{
    if (localDate == "0000-00-00" ||
        localTime == "00:00:00")
    {
        return createLogFile();
    }

    char safeTime[16];

    localTime.toCharArray(
        safeTime,
        sizeof(safeTime));

    for (int i = 0; safeTime[i] != '\0'; i++)
    {
        if (safeTime[i] == ':')
        {
            safeTime[i] = '-';
        }
    }

    char baseName[64];

    sprintf(
        baseName,
        "/flight_%s_%s.csv",
        localDate.c_str(),
        safeTime);

    if (!SD.exists(baseName))
    {
        strcpy(logFileName, baseName);
    }
    else
    {
        int index = 1;

        while (true)
        {
            sprintf(
                logFileName,
                "/flight_%s_%s_%03d.csv",
                localDate.c_str(),
                safeTime,
                index);

            if (!SD.exists(logFileName))
            {
                break;
            }

            index++;
        }
    }

    File file = SD.open(logFileName, FILE_WRITE);

    if (!file)
    {
        return false;
    }

    file.println(
        "timestamp_s,"
        "temperature_c,"
        "pressure_hpa,"
        "bmp_altitude_m,"
        "gps_fix,"
        "latitude,"
        "longitude,"
        "gps_altitude_m,"
        "flight_altitude_m,"
        "speed_kmh,"
        "battery_voltage_v,"
        "current_ma,"
        "power_mw,"
        "battery_soc_percent");

    file.close();

    return true;
}

const char* SDLogger::getLogFileName()
{
    return logFileName;
}

// =====================================================
// Telemetry Logging
// =====================================================

bool SDLogger::writeData(
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
    float batterySoc)
{
    if (logFileName[0] == '\0')
    {
        return false;
    }

    if (isStorageFull())
    {
        return false;
    }

    File file = SD.open(logFileName, FILE_APPEND);

    if (!file)
    {
        return false;
    }

    file.print(timestamp);
    file.print(",");

    file.print(temperature, CSV_DECIMAL_PLACES);
    file.print(",");

    file.print(pressure, CSV_DECIMAL_PLACES);
    file.print(",");

    file.print(bmpAltitude, CSV_DECIMAL_PLACES);
    file.print(",");

    file.print(gpsFix ? 1 : 0);
    file.print(",");

    file.print(latitude, 6);
    file.print(",");

    file.print(longitude, 6);
    file.print(",");

    file.print(gpsAltitude, 1);
    file.print(",");

    file.print(flightAltitude, CSV_DECIMAL_PLACES);
    file.print(",");

    file.print(speed, 2);
    file.print(",");

    file.print(batteryVoltage, 2);
    file.print(",");

    file.print(current_mA, 2);
    file.print(",");

    file.print(power_mW, 2);
    file.print(",");

    file.println(batterySoc, 1);

    bool writeOk = !file.getWriteError();

    file.close();

    return writeOk;
}

// =====================================================
// Storage Diagnostics
// =====================================================

uint64_t SDLogger::getTotalBytes()
{
    return SD.totalBytes();
}

uint64_t SDLogger::getUsedBytes()
{
    return SD.usedBytes();
}

uint64_t SDLogger::getFreeBytes()
{
    uint64_t totalBytes = getTotalBytes();
    uint64_t usedBytes = getUsedBytes();

    if (totalBytes > usedBytes)
    {
        return totalBytes - usedBytes;
    }

    return 0;
}

uint8_t SDLogger::getUsagePercent()
{
    uint64_t totalBytes = getTotalBytes();

    if (totalBytes == 0)
    {
        return 0;
    }

    uint64_t usedBytes = getUsedBytes();

    return (uint8_t)((usedBytes * 100ULL) / totalBytes);
}

bool SDLogger::isStorageAvailable()
{
    return getTotalBytes() > 0;
}

bool SDLogger::isStorageWarning()
{
    uint8_t usagePercent = getUsagePercent();

    return usagePercent >= SD_USAGE_WARNING_PERCENT &&
           usagePercent < SD_USAGE_CRITICAL_PERCENT;
}

bool SDLogger::isStorageCritical()
{
    uint8_t usagePercent = getUsagePercent();

    return usagePercent >= SD_USAGE_CRITICAL_PERCENT &&
           usagePercent < SD_USAGE_FULL_PERCENT;
}

bool SDLogger::isStorageFull()
{
    uint8_t usagePercent = getUsagePercent();

    return usagePercent >= SD_USAGE_FULL_PERCENT;
}