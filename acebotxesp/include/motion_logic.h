//include\motion_logic.h
#ifndef MOTION_LOGIC_H
#define MOTION_LOGIC_H

#include <Arduino.h>

// === Motion State Definitions ===
extern int8_t currentState;            // 0 = IDLE, 1 = PATROLLING, 2 = ALERT, 3 = FAULT
extern unsigned long lastLoopTime;     // Last system loop execution time (ms)
extern unsigned long lastPatrolTime;   // Last servo patrol time (ms)
extern uint8_t currentServoPos;        // Current angle of servo (0–180)

// === Core Motion Control Logic ===

// Runs the main state machine to control motion logic
void runStateMachine();

// Computes a threat score based on sensor conditions
int16_t calculateThreatScore();

#endif // MOTION_LOGIC_H
