// servo_controller.cpp
#include "servo_controller.h"
#include "system_init.h"
#include "motion_logic.h"

// === Constants ===
constexpr uint8_t SERVO_STEP_SIZE = 5;      // Step size for smooth servo movement (degrees)
constexpr uint16_t SERVO_STEP_DELAY = 15;    // Delay between steps (ms)
constexpr uint8_t PATROL_MIN_ANGLE = 0;     // Minimum angle for patrol sweep
constexpr uint8_t PATROL_MAX_ANGLE = 180;   // Maximum angle for patrol sweep
constexpr uint8_t PATROL_STEP = 45;         // Angle increment per patrol step

// === Smooth Servo Movement ===
void moveServoSmooth(uint8_t targetPos) {
    // Constrain target position to valid servo range
    targetPos = constrain(targetPos, 0, 180);

    // Log movement
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Moving servo to %u degrees", targetPos);
    logMessage("DEBUG", buffer);

    // Move servo in steps for smoothness
    while (currentServoPos != targetPos) {
        if (currentServoPos < targetPos) {
            currentServoPos = min(currentServoPos + SERVO_STEP_SIZE, targetPos);
        } else {
            currentServoPos = max(currentServoPos - SERVO_STEP_SIZE, targetPos);
        }
        panServo.write(currentServoPos);
        delay(SERVO_STEP_DELAY);
    }

    // Ensure final position is set
    panServo.write(targetPos);
}

// === Execute Patrol Sweep ===
void executePatrol() {
    // Determine next position for patrol (sweep back and forth)
    static bool sweepingRight = true; // Tracks sweep direction

    uint8_t nextPos;
    if (sweepingRight) {
        nextPos = currentServoPos + PATROL_STEP;
        if (nextPos >= PATROL_MAX_ANGLE) {
            nextPos = PATROL_MAX_ANGLE;
            sweepingRight = false;
        }
    } else {
        nextPos = currentServoPos - PATROL_STEP;
        if (nextPos <= PATROL_MIN_ANGLE) {
            nextPos = PATROL_MIN_ANGLE;
            sweepingRight = true;
        }
    }

    // Move to next position
    moveServoSmooth(nextPos);

    // Log patrol action
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Patrol sweep to %u degrees", nextPos);
    logMessage("INFO", buffer);
}
