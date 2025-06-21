#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// === Pin Assignments ===
constexpr uint8_t PIN_PIR = 2;        // PIR sensor pin
constexpr uint8_t PIN_TRIG1 = 3;      // Ultrasonic sensor 1 trigger pin
constexpr uint8_t PIN_ECHO1 = 4;      // Ultrasonic sensor 1 echo pin
constexpr uint8_t PIN_SERVO = 5;      // Servo motor pin
constexpr uint8_t PIN_LED = 6;        // LED indicator pin
constexpr uint8_t PIN_TRIG2 = 8;      // Ultrasonic sensor 2 trigger pin
constexpr uint8_t PIN_ECHO2 = 9;      // Ultrasonic sensor 2 echo pin
constexpr uint8_t PIN_SERIAL_RX = 10; // Software serial RX pin
constexpr uint8_t PIN_SERIAL_TX = 11; // Software serial TX pin

// === System State ===
extern Servo panServo;                 // Servo object for pan control
extern SoftwareSerial espSerial;       // SoftwareSerial for ESP communication
extern bool isInitialized;             // System initialization status
extern unsigned long lastWatchdogFeed; // Last watchdog feed timestamp

// === Function Prototypes ===
void initializeSystem();                                 // Initializes system components
void feedWatchdog();                                     // Feeds the watchdog timer
bool checkWatchdog();                                    // Checks watchdog status
void logMessage(const char *level, const char *message); // Logs messages with level

#endif // SYSTEM_INIT_H