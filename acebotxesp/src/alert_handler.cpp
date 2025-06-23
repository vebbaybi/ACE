// alert_handler.cpp

#include "alert_handler.h"
#include "sensors.h"
#include "system_init.h"

    // === Global Variables ===
    bool alertBlinkState = false;     // State of alert LED (true=ON, false=OFF)
unsigned long lastAlertBlinkTime = 0; // Timestamp of last alert blink (ms)
bool faultBlinkState = false;         // State of fault LED (true=ON, false=OFF)
unsigned long lastFaultBlinkTime = 0; // Timestamp of last fault blink (ms)

// === Constants ===
constexpr unsigned long FAULT_BLINK_INTERVAL = 500; // Fault blink interval (ms)
constexpr float MIN_DISTANCE_CM = 20.0;             // Minimum distance for blink mapping (cm)
constexpr float MAX_DISTANCE_CM = 150.0;            // Maximum distance for blink mapping (cm)
constexpr unsigned long MIN_BLINK_INTERVAL = 50;    // Fastest alert blink interval (ms)
constexpr unsigned long MAX_BLINK_INTERVAL = 300;   // Slowest alert blink interval (ms)

// === Handle Alert Blink ===
void handleAlertBlink()
{
    // Turn off LED if no valid threat
    if (!motionDetected || !sensorValid || distance < 0.01)
    {
        if (alertBlinkState)
        {
            digitalWrite(PIN_LED, LOW);
            alertBlinkState = false;
            logMessage("DEBUG", "Alert LED turned off");
        }
        return;
    }

    // Calculate blink interval based on distance
    float distance_cm = distance * 100.0; // Convert meters to cm
    unsigned long blinkInterval = map(
        constrain(distance_cm, MIN_DISTANCE_CM, MAX_DISTANCE_CM),
        MIN_DISTANCE_CM, MAX_DISTANCE_CM,
        MIN_BLINK_INTERVAL, MAX_BLINK_INTERVAL);

    // Toggle LED state if interval has passed
    if (millis() - lastAlertBlinkTime >= blinkInterval)
    {
        alertBlinkState = !alertBlinkState;
        digitalWrite(PIN_LED, alertBlinkState ? HIGH : LOW);
        lastAlertBlinkTime = millis();

        // Log state change
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Alert LED %s, interval: %lums",
                 alertBlinkState ? "ON" : "OFF", blinkInterval);
        logMessage("DEBUG", buffer);
    }
}

// === Handle Fault Blink ===
void handleFaultBlink()
{
    // Toggle LED state at fixed interval
    if (millis() - lastFaultBlinkTime >= FAULT_BLINK_INTERVAL)
    {
        faultBlinkState = !faultBlinkState;
        digitalWrite(PIN_LED, faultBlinkState ? HIGH : LOW);
        lastFaultBlinkTime = millis();

        // Log state change
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "Fault LED %s", faultBlinkState ? "ON" : "OFF");
        logMessage("DEBUG", buffer);
    }
}
