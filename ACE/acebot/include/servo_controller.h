#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>

// === Function Prototypes ===
void moveServoSmooth(uint8_t targetPos); // Smoothly transitions servo to target angle (0–180)
void executePatrol();                    // Performs patrol by sweeping servo across range

#endif // SERVO_CONTROLLER_H