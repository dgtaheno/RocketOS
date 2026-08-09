#pragma once

// =====================================================
// System Information
// =====================================================

#define SYSTEM_NAME                    "Flight Telemetry & Data Logger"
#define SYSTEM_VERSION                 "0.8.0"

// =====================================================
// Installed Modules
// =====================================================

#define BMP388_ENABLED                 true
#define GPS_ENABLED                    true
#define SD_LOGGER_ENABLED              true

#define SYSTEM_HEALTH_ENABLED          true
#define EVENT_LOGGING_ENABLED          true
#define BUFFERED_LOGGING_ENABLED       true

#define LORA_ENABLED                   false
#define INA219_ENABLED                 false

// =====================================================
// GPS Configuration
// =====================================================

#define GPS_UART_BAUDRATE              38400

#define GPS_RX_PIN                     16
#define GPS_TX_PIN                     17

// Local time offset from UTC
#define UTC_OFFSET_HOURS               2

// Ignore GPS speed values below this threshold.
// Helps eliminate GPS noise when the device is stationary.
#define GPS_SPEED_DEADBAND_KMH         3.0f

// Time to wait during startup for GPS FIX before using fallback filename.
#define GPS_FIX_TIMEOUT_SECONDS        30

// =====================================================
// BMP388 Configuration
// =====================================================

#define BMP388_I2C_ADDRESS             0x77

// =====================================================
// SD Card Configuration
// =====================================================

#define SD_CS_PIN                      5

// SD usage monitoring thresholds
#define SD_USAGE_WARNING_PERCENT       80
#define SD_USAGE_CRITICAL_PERCENT      95
#define SD_USAGE_FULL_PERCENT          99

// =====================================================
// Logging Configuration
// =====================================================

#define LOG_INTERVAL_MS                1000

#define CSV_DECIMAL_PLACES             2

// Number of telemetry records stored in RAM if SD writing fails.
// This provides short-term protection against temporary SD write failures.
#define LOG_BUFFER_SIZE                100

// =====================================================
// GPS Logging Configuration
// =====================================================

#define USE_LOCAL_TIME_FOR_LOGS        true

#define FALLBACK_TO_BOOT_FILENAME      true

// =====================================================
// System Health Configuration
// =====================================================

// How often the system health status is printed to Serial.
#define HEALTH_PRINT_INTERVAL_MS       10000

// Timeout thresholds for detecting stale module activity.
#define GPS_DATA_TIMEOUT_MS            5000
#define BMP_DATA_TIMEOUT_MS            3000
#define SD_WRITE_TIMEOUT_MS            5000

// =====================================================
// POST Configuration
// =====================================================

#define HALT_ON_BMP388_FAIL            true
#define HALT_ON_SD_FAIL                true
#define HALT_ON_GPS_FAIL               false

// =====================================================
// Event Logging Configuration
// =====================================================

#define EVENT_LOG_FILE_NAME            "/events.log"

// =====================================================
// Debug Configuration
// =====================================================

#define PRINT_TELEMETRY_TO_SERIAL      true
#define PRINT_HEALTH_TO_SERIAL         true

// Runs a synthetic flight profile through the
// FlightStateMachine at boot instead of normal operation.
// Set to 0 for normal flight.
#define FLIGHT_SIMULATION_MODE 0

// =====================================================
// Buzzer Configuration
// =====================================================
#define BUZZER_ENABLED true
#define BUZZER_PIN 25

// =====================================================
// DS18B20 External Thermometer Configuration
// =====================================================
#define EXT_THERMOMETER_ENABLED   true
#define EXT_THERMOMETER_PIN       4
