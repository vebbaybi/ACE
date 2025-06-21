// system_init.cpp
#include "system_init.h"

// === Global Variables ===
Servo panServo;                                         // Servo object for pan control
SoftwareSerial espSerial(PIN_SERIAL_RX, PIN_SERIAL_TX); // SoftwareSerial for ESP communication
bool isInitialized = false;                             // System initialization status
unsigned long lastWatchdogFeed = 0;                     // Last watchdog feed timestamp

// === Constants ===
constexpr unsigned long WATCHDOG_TIMEOUT = 2000; // Watchdog timeout in ms (2 seconds)
constexpr uint32_t SERIAL_BAUD_RATE = 9600;      // Serial baud rate for ESP communication

// === Initialize System ===
void initializeSystem()
{
    // Initialize pins
    pinMode(PIN_PIR, INPUT);
    pinMode(PIN_TRIG1, OUTPUT);
    pinMode(PIN_ECHO1, INPUT);
    pinMode(PIN_TRIG2, OUTPUT);
    pinMode(PIN_ECHO2, INPUT);
    pinMode(PIN_LED, OUTPUT);

    // Ensure LED is off initially
    digitalWrite(PIN_LED, LOW);

    // Initialize servo
    panServo.attach(PIN_SERVO);
    panServo.write(90); // Center position

    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    espSerial.begin(SERIAL_BAUD_RATE);

    // Log initialization
    logMessage("INFO", "System initialized successfully");

    // Set initialization flag and feed watchdog
    isInitialized = true;
    feedWatchdog();
}

// === Feed Watchdog ===
void feedWatchdog()
{
    lastWatchdogFeed = millis();
    logMessage("DEBUG", "Watchdog fed");
}

// === Check Watchdog ===
bool checkWatchdog()
{
    if (millis() - lastWatchdogFeed > WATCHDOG_TIMEOUT)
    {
        logMessage("ERROR", "Watchdog timeout detected");
        return false;
    }
    return true;
}

// === Log Message ===
void logMessage(const char *level, const char *message)
{
    // Format: [time_s LEVEL] message
    Serial.print("[");
    Serial.print(millis() / 1000.0, 1);
    Serial.print("s ");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);

    // Also send to ESP serial
    espSerial.print("[");
    espSerial.print(millis() / 1000.0, 1);
    espSerial.print("s ");
    espSerial.print(level);
    Serial.print("] ");
    espSerial.println(message);
}
