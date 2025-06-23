#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// === Sensor State ===
extern bool motionDetected; // Flag for PIR motion detection
extern float dist1;         // Distance from ultrasonic sensor 1 (cm)
extern float dist2;         // Distance from ultrasonic sensor 2 (cm)
extern float distance;      // Minimum detected distance (m)
extern float confidence;    // Threat confidence level (0.0–100.0)
extern bool sensorValid;    // Flag for valid sensor data

// === Function Prototypes ===
void updateSensors();                                     // Updates all sensor readings
float readUltrasonicCM(uint8_t trigPin, uint8_t echoPin); // Reads ultrasonic distance in cm

#endif // SENSORS_H