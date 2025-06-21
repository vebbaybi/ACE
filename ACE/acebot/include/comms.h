#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>

// === Function Prototypes ===
void checkOTA();                         // Checks for Over-The-Air updates (mock implementation)
void handleSerialComms();                // Processes incoming serial messages
void sendCameraCommand(const char *cmd); // Sends command string to ESP32-CAM

#endif // COMMS_H