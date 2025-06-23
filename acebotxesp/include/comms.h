// include\comms.h
#ifndef COMMS_H
#define COMMS_H

#include <Arduino.h>

// === COMMS Interface for UNO ↔ ESP32-CAM ===
// This module handles serial communication between the Arduino UNO R4
// and the ESP32-CAM module using SoftwareSerial.

// Initializes SoftwareSerial and prepares the communication channel
void initComms();

// Sends a string command to the ESP32-CAM (e.g., "STREAM_ON", "PING")
void sendCameraCommand(const char *cmd);

// Continuously listens for incoming serial messages from ESP32-CAM
void handleSerialComms();

// Placeholder for future Over-The-Air update logic
void checkOTA();

// Internal use: Parses and responds to incoming ESP32 messages
void processIncomingMessage(const String &msg);

#endif // COMMS_H
