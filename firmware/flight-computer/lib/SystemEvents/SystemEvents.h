#pragma once

#include <Arduino.h>

// =====================================================
// System Event Types
// =====================================================

enum SystemEventType
{
    EVENT_NONE = 0,

    // System
    EVENT_SYSTEM_START,
    EVENT_SYSTEM_READY,

    // GPS
    EVENT_GPS_DETECTED,
    EVENT_GPS_NOT_DETECTED,
    EVENT_GPS_FIX_ACQUIRED,
    EVENT_GPS_FIX_LOST,
    EVENT_GPS_DATA_LOST,
    EVENT_GPS_DATA_RECOVERED,

    // BMP388
    EVENT_BMP_DETECTED,
    EVENT_BMP_NOT_DETECTED,
    EVENT_BMP_READ_FAILED,
    EVENT_BMP_RECOVERED,

    // SD Card
    EVENT_SD_DETECTED,
    EVENT_SD_NOT_DETECTED,
    EVENT_SD_REMOVED,
    EVENT_SD_INSERTED,
    EVENT_SD_RECOVERED,
    EVENT_SD_WRITE_FAILED,
    EVENT_SD_WRITE_RECOVERED,
    EVENT_SD_CARD_FULL,
    EVENT_SD_STORAGE_WARNING,
    EVENT_SD_STORAGE_CRITICAL,

    // Battery

    EVENT_BATTERY_CONNECTED,
    EVENT_BATTERY_DISCONNECTED,

    EVENT_BATTERY_WARNING,
    EVENT_BATTERY_CRITICAL,
    
    // Logging
    EVENT_LOG_FILE_CREATED,
    EVENT_LOG_FILE_CREATION_FAILED,
    EVENT_BUFFER_RECORD_STORED,
    EVENT_BUFFER_FLUSH_STARTED,
    EVENT_BUFFER_FLUSH_COMPLETED,
    EVENT_BUFFER_OVERFLOW,

    // Generic
    EVENT_UNKNOWN
};

// =====================================================
// SD Runtime State
// =====================================================

enum SdRuntimeState
{
    SD_STATE_UNKNOWN = 0,
    SD_STATE_OK,
    SD_STATE_WRITE_FAILED,
    SD_STATE_REMOVED,
    SD_STATE_INSERTED,
    SD_STATE_RECOVERING,
    SD_STATE_RECOVERED,
    SD_STATE_FULL
};

// =====================================================
// System Events
// =====================================================

class SystemEvents
{
private:
    // -------------------------------------------------
    // SD State Machine
    // -------------------------------------------------

    SdRuntimeState sdState;

    bool sdPreviouslyAvailable;
    bool sdPreviouslyWritable;

    uint8_t sdConsecutiveWriteFailures;

    uint32_t sdRemovalCount;
    uint32_t sdRecoveryCount;
    uint32_t sdWriteFailureCount;

    // -------------------------------------------------
    // GPS State Tracking
    // -------------------------------------------------

    bool gpsPreviouslyFixed;
    bool gpsPreviouslyDetected;

    // -------------------------------------------------
    // BMP State Tracking
    // -------------------------------------------------

    bool bmpPreviouslyHealthy;

    // -------------------------------------------------
    // Event Timing
    // -------------------------------------------------

    unsigned long lastEventTimestamp;

public:
    SystemEvents();

    // =================================================
    // Initialization
    // =================================================

    void begin();

    void reset();

    // =================================================
    // Event Logging
    // =================================================

    void logEvent(
        SystemEventType eventType);

    void logEvent(
        SystemEventType eventType,
        const char* details);

    // =================================================
    // SD State Machine
    // =================================================

    void updateSdState(
        bool sdAvailable,
        bool sdWriteOk,
        bool sdFull);

    SdRuntimeState getSdState() const;

    uint8_t getSdConsecutiveWriteFailures() const;

    uint32_t getSdRemovalCount() const;

    uint32_t getSdRecoveryCount() const;

    uint32_t getSdWriteFailureCount() const;

    bool isSdRemoved() const;

    bool isSdOk() const;

    bool isSdFull() const;

    // =================================================
    // GPS State Change Detection
    // =================================================

    void updateGpsState(
        bool gpsDetected,
        bool gpsFix);

    // =================================================
    // BMP State Change Detection
    // =================================================

    void updateBmpState(
        bool bmpHealthy);

    // =================================================
    // State Getters
    // =================================================

    bool wasSdPreviouslyAvailable() const;

    bool wasGpsPreviouslyFixed() const;

    bool wasGpsPreviouslyDetected() const;

    bool wasBmpPreviouslyHealthy() const;

    unsigned long getLastEventTimestamp() const;

    // =================================================
    // Text Helpers
    // =================================================

    const char* eventToText(
        SystemEventType eventType) const;

    const char* sdStateToText(
        SdRuntimeState state) const;

    // =================================================
    // Debug
    // =================================================

    void printSdState() const;
};