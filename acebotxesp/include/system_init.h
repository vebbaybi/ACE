//include\system_init.h
#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// === Pin Assignments ===
constexpr uint8_t PIN_PIR = 2;        // Passive Infrared sensor
constexpr uint8_t PIN_TRIG1 = 3;      // Ultrasonic Sensor 1 - Trigger
constexpr uint8_t PIN_ECHO1 = 4;      // Ultrasonic Sensor 1 - Echo
constexpr uint8_t PIN_SERVO = 5;      // Servo motor control
constexpr uint8_t PIN_LED = 6;        // Status LED
constexpr uint8_t PIN_TRIG2 = 8;      // Ultrasonic Sensor 2 - Trigger
constexpr uint8_t PIN_ECHO2 = 9;      // Ultrasonic Sensor 2 - Echo
constexpr uint8_t PIN_SERIAL_RX = 10; // SoftwareSerial RX (ESP32 TX)
constexpr uint8_t PIN_SERIAL_TX = 11; // SoftwareSerial TX (ESP32 RX)

// === Global System State ===
extern Servo panServo;                 // Controls directional panning
extern SoftwareSerial espSerial;       // Serial link to ESP32-CAM
extern bool isInitialized;             // Tracks if system has booted
extern unsigned long lastWatchdogFeed; // Timestamp of last watchdog reset

// === System Initialization & Monitoring ===
void initializeSystem();                                 // Full system boot sequence
void feedWatchdog();                                     // Reset watchdog timer
bool checkWatchdog();                                    // Verify system responsiveness
void logMessage(const char *level, const char *message); // Structured serial logging

#endif // SYSTEM_INIT_H
