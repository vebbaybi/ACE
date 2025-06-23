// include\servo_controller.h
#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>

// === Servo Control Module ===
// Handles smooth panning and patrol sweep functionality

// Smoothly transitions the pan servo to a specified angle (0–180 degrees)
void moveServoSmooth(uint8_t targetPos);

// Executes a predefined patrol movement by sweeping the servo
void executePatrol();

#endif // SERVO_CONTROLLER_H
