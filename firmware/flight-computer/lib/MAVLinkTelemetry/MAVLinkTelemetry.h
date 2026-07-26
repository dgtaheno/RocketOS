#pragma once

#include <Arduino.h>

extern "C"
{
    #include "../MAVLink/common/mavlink.h"
}

class MAVLinkTelemetry
{
public:
    MAVLinkTelemetry();

    void begin();

    void sendHeartbeat();

private:
    uint8_t systemId;
    uint8_t componentId;
};