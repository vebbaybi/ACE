// comms.cpp

#include "comms.h"
#include "system_init.h"

    // === Constants ===
    constexpr uint8_t OTA_CHECK_FREQUENCY = 5;     // Check OTA every 5th call
constexpr size_t SERIAL_BUFFER_SIZE = 32;          // Buffer size for serial input
constexpr unsigned long SERIAL_READ_TIMEOUT = 100; // Timeout for reading serial (ms)

// === Check OTA Updates ===
void checkOTA()
{
    static uint8_t count = 0;
    if (count++ % OTA_CHECK_FREQUENCY == 0)
    {
        logMessage("INFO", "Checking for OTA updates (mock implementation)");
    }
}

// === Handle Serial Communications ===
void handleSerialComms()
{
    if (espSerial.available())
    {
        char buffer[SERIAL_BUFFER_SIZE];
        size_t index = 0;
        unsigned long startTime = millis();

        // Read until newline or buffer full, with timeout
        while (index < SERIAL_BUFFER_SIZE - 1 && millis() - startTime < SERIAL_READ_TIMEOUT)
        {
            if (espSerial.available())
            {
                char c = espSerial.read();
                if (c == '\n')
                {
                    buffer[index] = '\0'; // Null-terminate
                    break;
                }
                buffer[index++] = c;
            }
        }
        buffer[index] = '\0'; // Ensure null-termination

        // Log received message
        if (index > 0)
        {
            char logBuffer[64];
            snprintf(logBuffer, sizeof(logBuffer), "Received from ESP: %s", buffer);
            logMessage("DEBUG", logBuffer);
        }
        else
        {
            logMessage("WARN", "Empty or timed-out serial input");
        }
    }
}

// === Send Camera Command ===
void sendCameraCommand(const char *cmd)
{
    // Send command to ESP32-CAM
    espSerial.println(cmd);

    // Log sent command
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Sent to ESP32-CAM: %s", cmd);
    logMessage("INFO", buffer);
}
