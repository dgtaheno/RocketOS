#include "SystemEvents.h"

#include "../../include/Config.h"

#ifndef SD_REMOVAL_FAILURE_THRESHOLD
#define SD_REMOVAL_FAILURE_THRESHOLD 3
#endif

// =====================================================
// Constructor
// =====================================================

SystemEvents::SystemEvents()
{
    reset();
}

// =====================================================
// Initialization
// =====================================================

void SystemEvents::begin()
{
    reset();
}

void SystemEvents::reset()
{
    sdState = SD_STATE_UNKNOWN;

    sdPreviouslyAvailable = true;
    sdPreviouslyWritable = true;

    sdConsecutiveWriteFailures = 0;

    sdRemovalCount = 0;
    sdRecoveryCount = 0;
    sdWriteFailureCount = 0;

    gpsPreviouslyFixed = false;
    gpsPreviouslyDetected = false;

    bmpPreviouslyHealthy = true;

    lastEventTimestamp = 0;
}

// =====================================================
// Event Logging
// =====================================================

void SystemEvents::logEvent(
    SystemEventType eventType)
{
    lastEventTimestamp = millis();

    Serial.print("[EVENT] ");
    Serial.println(eventToText(eventType));
}

void SystemEvents::logEvent(
    SystemEventType eventType,
    const char* details)
{
    lastEventTimestamp = millis();

    Serial.print("[EVENT] ");
    Serial.print(eventToText(eventType));

    if (details != nullptr)
    {
        Serial.print(" : ");
        Serial.print(details);
    }

    Serial.println();
}

// =====================================================
// SD State Machine
// =====================================================

void SystemEvents::updateSdState(
    bool sdAvailable,
    bool sdWriteOk,
    bool sdFull)
{
    // --------------------------------------------------
    // SD full has priority over write failures
    // --------------------------------------------------

    if (sdFull)
    {
        if (sdState != SD_STATE_FULL)
        {
            sdState = SD_STATE_FULL;
            logEvent(EVENT_SD_CARD_FULL);
        }

        sdPreviouslyAvailable = sdAvailable;
        sdPreviouslyWritable = false;

        return;
    }

    // --------------------------------------------------
    // Successful write means SD is currently usable
    // --------------------------------------------------

    if (sdWriteOk)
    {
        sdConsecutiveWriteFailures = 0;

        if (sdState == SD_STATE_REMOVED ||
            sdState == SD_STATE_WRITE_FAILED ||
            sdState == SD_STATE_RECOVERING ||
            sdState == SD_STATE_FULL)
        {
            logEvent(EVENT_SD_INSERTED);
            logEvent(EVENT_SD_RECOVERED);

            sdRecoveryCount++;

            sdState = SD_STATE_RECOVERED;
        }
        else if (sdState == SD_STATE_UNKNOWN)
        {
            logEvent(EVENT_SD_DETECTED);

            sdState = SD_STATE_OK;
        }
        else
        {
            sdState = SD_STATE_OK;
        }

        sdPreviouslyAvailable = true;
        sdPreviouslyWritable = true;

        return;
    }

    // --------------------------------------------------
    // If write failed, track consecutive failures
    // --------------------------------------------------

    sdWriteFailureCount++;
    sdConsecutiveWriteFailures++;

    if (sdConsecutiveWriteFailures == 1)
    {
        if (sdState == SD_STATE_OK ||
            sdState == SD_STATE_RECOVERED ||
            sdState == SD_STATE_UNKNOWN)
        {
            sdState = SD_STATE_WRITE_FAILED;

            logEvent(EVENT_SD_WRITE_FAILED);
        }
    }

    // --------------------------------------------------
    // Multiple consecutive failures indicate removal
    // --------------------------------------------------

    if (sdConsecutiveWriteFailures >= SD_REMOVAL_FAILURE_THRESHOLD)
    {
        if (sdState != SD_STATE_REMOVED)
        {
            sdState = SD_STATE_REMOVED;

            sdRemovalCount++;

            logEvent(EVENT_SD_REMOVED);
        }
    }

    sdPreviouslyAvailable = sdAvailable;
    sdPreviouslyWritable = false;
}

SdRuntimeState SystemEvents::getSdState() const
{
    return sdState;
}

uint8_t SystemEvents::getSdConsecutiveWriteFailures() const
{
    return sdConsecutiveWriteFailures;
}

uint32_t SystemEvents::getSdRemovalCount() const
{
    return sdRemovalCount;
}

uint32_t SystemEvents::getSdRecoveryCount() const
{
    return sdRecoveryCount;
}

uint32_t SystemEvents::getSdWriteFailureCount() const
{
    return sdWriteFailureCount;
}

bool SystemEvents::isSdRemoved() const
{
    return sdState == SD_STATE_REMOVED;
}

bool SystemEvents::isSdOk() const
{
    return sdState == SD_STATE_OK ||
           sdState == SD_STATE_RECOVERED;
}

bool SystemEvents::isSdFull() const
{
    return sdState == SD_STATE_FULL;
}

// =====================================================
// GPS State Change Detection
// =====================================================

void SystemEvents::updateGpsState(
    bool gpsDetected,
    bool gpsFix)
{
    // --------------------------------------------------
    // GPS Detection
    // --------------------------------------------------

    if (!gpsPreviouslyDetected &&
         gpsDetected)
    {
        logEvent(EVENT_GPS_DETECTED);
    }
    else if (gpsPreviouslyDetected &&
            !gpsDetected)
    {
        logEvent(EVENT_GPS_DATA_LOST);
    }

    // --------------------------------------------------
    // GPS Fix
    // --------------------------------------------------

    if (!gpsPreviouslyFixed &&
         gpsFix)
    {
        logEvent(EVENT_GPS_FIX_ACQUIRED);
    }
    else if (gpsPreviouslyFixed &&
            !gpsFix)
    {
        logEvent(EVENT_GPS_FIX_LOST);
    }

    gpsPreviouslyDetected = gpsDetected;
    gpsPreviouslyFixed = gpsFix;
}

// =====================================================
// BMP State Change Detection
// =====================================================

void SystemEvents::updateBmpState(
    bool bmpHealthy)
{
    if (bmpPreviouslyHealthy &&
       !bmpHealthy)
    {
        logEvent(EVENT_BMP_READ_FAILED);
    }
    else if (!bmpPreviouslyHealthy &&
              bmpHealthy)
    {
        logEvent(EVENT_BMP_RECOVERED);
    }

    bmpPreviouslyHealthy = bmpHealthy;
}

// =====================================================
// State Getters
// =====================================================

bool SystemEvents::wasSdPreviouslyAvailable() const
{
    return sdPreviouslyAvailable;
}

bool SystemEvents::wasGpsPreviouslyFixed() const
{
    return gpsPreviouslyFixed;
}

bool SystemEvents::wasGpsPreviouslyDetected() const
{
    return gpsPreviouslyDetected;
}

bool SystemEvents::wasBmpPreviouslyHealthy() const
{
    return bmpPreviouslyHealthy;
}

unsigned long SystemEvents::getLastEventTimestamp() const
{
    return lastEventTimestamp;
}

// =====================================================
// Text Helpers
// =====================================================

const char* SystemEvents::eventToText(
    SystemEventType eventType) const
{
    switch (eventType)
    {
        case EVENT_SYSTEM_START:
            return "SYSTEM_START";

        case EVENT_SYSTEM_READY:
            return "SYSTEM_READY";

        case EVENT_GPS_DETECTED:
            return "GPS_DETECTED";

        case EVENT_GPS_NOT_DETECTED:
            return "GPS_NOT_DETECTED";

        case EVENT_GPS_FIX_ACQUIRED:
            return "GPS_FIX_ACQUIRED";

        case EVENT_GPS_FIX_LOST:
            return "GPS_FIX_LOST";

        case EVENT_GPS_DATA_LOST:
            return "GPS_DATA_LOST";

        case EVENT_GPS_DATA_RECOVERED:
            return "GPS_DATA_RECOVERED";

        case EVENT_BMP_DETECTED:
            return "BMP_DETECTED";

        case EVENT_BMP_NOT_DETECTED:
            return "BMP_NOT_DETECTED";

        case EVENT_BMP_READ_FAILED:
            return "BMP_READ_FAILED";

        case EVENT_BMP_RECOVERED:
            return "BMP_RECOVERED";

        case EVENT_SD_DETECTED:
            return "SD_DETECTED";

        case EVENT_SD_NOT_DETECTED:
            return "SD_NOT_DETECTED";

        case EVENT_SD_REMOVED:
            return "SD_REMOVED";

        case EVENT_SD_INSERTED:
            return "SD_INSERTED";

        case EVENT_SD_RECOVERED:
            return "SD_RECOVERED";

        case EVENT_SD_WRITE_FAILED:
            return "SD_WRITE_FAILED";

        case EVENT_SD_WRITE_RECOVERED:
            return "SD_WRITE_RECOVERED";

        case EVENT_SD_CARD_FULL:
            return "SD_CARD_FULL";

        case EVENT_SD_STORAGE_WARNING:
            return "SD_STORAGE_WARNING";

        case EVENT_SD_STORAGE_CRITICAL:
            return "SD_STORAGE_CRITICAL";

        case EVENT_BATTERY_CONNECTED:
            return "BATTERY_CONNECTED";

        case EVENT_BATTERY_DISCONNECTED:
            return "BATTERY_DISCONNECTED";

        case EVENT_BATTERY_WARNING:
            return "BATTERY_WARNING";

        case EVENT_BATTERY_CRITICAL:
            return "BATTERY_CRITICAL";

        case EVENT_LOG_FILE_CREATED:
            return "LOG_FILE_CREATED";

        case EVENT_LOG_FILE_CREATION_FAILED:
            return "LOG_FILE_CREATION_FAILED";

        case EVENT_BUFFER_RECORD_STORED:
            return "BUFFER_RECORD_STORED";

        case EVENT_BUFFER_FLUSH_STARTED:
            return "BUFFER_FLUSH_STARTED";

        case EVENT_BUFFER_FLUSH_COMPLETED:
            return "BUFFER_FLUSH_COMPLETED";

        case EVENT_BUFFER_OVERFLOW:
            return "BUFFER_OVERFLOW";

        default:
            return "UNKNOWN_EVENT";
    }
}

const char* SystemEvents::sdStateToText(
    SdRuntimeState state) const
{
    switch (state)
    {
        case SD_STATE_UNKNOWN:
            return "UNKNOWN";

        case SD_STATE_OK:
            return "OK";

        case SD_STATE_WRITE_FAILED:
            return "WRITE_FAILED";

        case SD_STATE_REMOVED:
            return "REMOVED";

        case SD_STATE_INSERTED:
            return "INSERTED";

        case SD_STATE_RECOVERING:
            return "RECOVERING";

        case SD_STATE_RECOVERED:
            return "RECOVERED";

        case SD_STATE_FULL:
            return "FULL";

        default:
            return "UNKNOWN";
    }
}

// =====================================================
// Debug
// =====================================================

void SystemEvents::printSdState() const
{
    Serial.println();
    Serial.println("SD Runtime State");
    Serial.println("--------------------------------");

    Serial.print("State              : ");
    Serial.println(sdStateToText(sdState));

    Serial.print("Consecutive Failures: ");
    Serial.println(sdConsecutiveWriteFailures);

    Serial.print("Write Failures      : ");
    Serial.println(sdWriteFailureCount);

    Serial.print("Removal Count       : ");
    Serial.println(sdRemovalCount);

    Serial.print("Recovery Count      : ");
    Serial.println(sdRecoveryCount);

    Serial.println("--------------------------------");
    Serial.println();
}