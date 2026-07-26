#include "MAVLinkTelemetry.h"

MAVLinkTelemetry::MAVLinkTelemetry()
{
    systemId = 1;
    componentId = MAV_COMP_ID_AUTOPILOT1;
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

    //Serial.write(
    //    buffer,
    //    len);

#if PRINT_TELEMETRY_TO_SERIAL

    Serial.println(
        "[MAVLINK] HEARTBEAT sent");

#endif
}