// sensors.h
#include "sensors.h"
#include "system_init.h"

// === Global Variables ===
bool motionDetected = false; // Flag for PIR motion detection
float dist1 = -1.0;          // Distance from ultrasonic sensor 1 (cm)
float dist2 = -1.0;          // Distance from ultrasonic sensor 2 (cm)
float distance = 0.0;        // Minimum detected distance (m)
float confidence = 0.0;      // Threat confidence level (0.0–100.0)
bool sensorValid = false;    // Flag for valid sensor data

// === Constants ===
constexpr unsigned long ULTRASONIC_TIMEOUT = 30000; // Timeout for pulseIn (µs)

// === Read Ultrasonic Distance ===
float readUltrasonicCM(uint8_t trigPin, uint8_t echoPin)
{
    // Send trigger pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure echo pulse duration
    long duration = pulseIn(echoPin, HIGH, ULTRASONIC_TIMEOUT);
    if (duration == 0)
    {
        logMessage("WARN", "Ultrasonic pulse timeout");
        return -1.0;
    }

    // Convert to distance (cm)
    float distance_cm = (duration / 2.0) * 0.0343;
    return distance_cm;
}

// === Update Sensor Readings ===
void updateSensors()
{
    // Read PIR sensor
    motionDetected = digitalRead(PIN_PIR) == HIGH;

    // Read ultrasonic sensors
    dist1 = readUltrasonicCM(PIN_TRIG1, PIN_ECHO1);
    dist2 = readUltrasonicCM(PIN_TRIG2, PIN_ECHO2);

    // Validate sensor readings
    if (dist1 < 0.0 && dist2 < 0.0)
    {
        sensorValid = false;
        distance = 0.0;
        confidence = 0.0;
        logMessage("WARN", "Both ultrasonic sensors failed");
        return;
    }

    // Use minimum valid distance, convert to meters
    if (dist1 >= 0.0 && dist2 >= 0.0)
    {
        distance = min(dist1, dist2) / 100.0;
    }
    else if (dist1 >= 0.0)
    {
        distance = dist1 / 100.0;
    }
    else
    {
        distance = dist2 / 100.0;
    }

    // Calculate confidence (simplified logic from provided code)
    confidence = (distance > 0.0 && distance < 1.5) ? 90.0 : 40.0;
    sensorValid = true;

    // Log sensor data
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Motion: %d, Distance: %.2fm, Confidence: %.1f",
             motionDetected, distance, confidence);
    logMessage("DEBUG", buffer);
}
