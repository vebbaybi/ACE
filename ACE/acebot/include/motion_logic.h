#ifndef MOTION_LOGIC_H
#define MOTION_LOGIC_H

#include <Arduino.h>

// === Motion State ===
extern int8_t currentState;          // System state: 0=IDLE, 1=PATROLLING, 2=ALERT, 3=FAULT
extern unsigned long lastLoopTime;   // Timestamp of last loop execution (ms)
extern unsigned long lastPatrolTime; // Timestamp of last patrol action (ms)
extern uint8_t currentServoPos;      // Current servo angle (0–180 degrees)

// === Function Prototypes ===
void runStateMachine();         // Executes the motion state machine
int16_t calculateThreatScore(); // Calculates threat score based on sensor data

#endif // MOTION_LOGIC_H