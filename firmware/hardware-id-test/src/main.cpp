#include <Arduino.h>
#include "esp_chip_info.h"

void printHardwareInfo()
{
    Serial.println();
    Serial.println("=== RocketOS Hardware Check ===");

    Serial.print("Chip model     : ");
    Serial.println(ESP.getChipModel());

    Serial.print("Chip revision  : ");
    Serial.println(ESP.getChipRevision());

    Serial.print("CPU frequency  : ");
    Serial.print(ESP.getCpuFreqMHz());
    Serial.println(" MHz");

    Serial.print("Flash size     : ");
    Serial.print(ESP.getFlashChipSize() / (1024UL * 1024UL));
    Serial.println(" MB");

    Serial.print("Flash speed    : ");
    Serial.print(ESP.getFlashChipSpeed() / 1000000UL);
    Serial.println(" MHz");

    bool hasPsram = psramFound();

    Serial.print("PSRAM detected : ");
    Serial.println(hasPsram ? "YES" : "NO");

    if (hasPsram)
    {
        uint32_t psramBytes = ESP.getPsramSize();
        uint32_t freePsramBytes = ESP.getFreePsram();

        Serial.print("PSRAM bytes    : ");
        Serial.println(psramBytes);

        Serial.print("PSRAM MiB      : ");
        Serial.println(
            psramBytes / (1024.0f * 1024.0f),
            3);

        Serial.print("Free PSRAM B   : ");
        Serial.println(freePsramBytes);

        Serial.print("Free PSRAM MiB : ");
        Serial.println(
            freePsramBytes / (1024.0f * 1024.0f),
            3);
    }

    Serial.println("==============================");
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    printHardwareInfo();
}

void loop()
{
    printHardwareInfo();
    delay(5000);
}