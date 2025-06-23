// motion_logic.cpp
#include "motion_logic.h"
#include "sensors.h"
#include "system_init.h"
#include "servo_controller.h"
#include "alert_handler.h"
#include "comms.h"

// === Global Variables ===
int8_t currentState = 0;          // System state: 0=IDLE, 1=PATROLLING, 2=ALERT, 3=FAULT
unsigned long lastLoopTime = 0;   // Timestamp of last loop execution (ms)
unsigned long lastPatrolTime = 0; // Timestamp of last patrol action (ms)
uint8_t currentServoPos = 90;     // Current servo angle (0–180 degrees)

// === Constants ===
constexpr unsigned long PATROL_INTERVAL = 5000;      // Time before entering PATROLLING state (ms)
constexpr uint8_t SERVO_LEFT_ANGLE = 45;             // Servo angle for left-facing target
constexpr uint8_t SERVO_RIGHT_ANGLE = 135;           // Servo angle for right-facing target
constexpr int16_t THREAT_THRESHOLD = 70;             // Threat score to trigger ALERT state
constexpr char CAMERA_CAPTURE_CMD[] = "CAM_CAPTURE"; // Command for ESP32-CAM capture

// === Calculate Threat Score ===
int16_t calculateThreatScore()
{
    if (!sensorValid)
    {
        return 0;
    }

    float score = 0.0;
    if (motionDetected)
    {
        score += 40.0;
    }
    if (distance < 1.5)
    {
        score += 30.0 * (1.0 - distance / 1.5);
    }
    if (confidence > 80.0)
    {
        score += 30.0 * (confidence / 100.0);
    }

    return min(static_cast<int16_t>(score), 100);
}

// === Run State Machine ===
void runStateMachine()
{
    // Update loop timestamp and feed watchdog
    lastLoopTime = millis();
    feedWatchdog();
    if (!checkWatchdog())
    {
        currentState = 3; // FAULT
        logMessage("ERROR", "Watchdog failure, entering FAULT state");
        return;
    }

    // State machine
    switch (currentState)
    {
    case 0: // IDLE
        updateSensors();
        if (sensorValid)
        {
            int16_t score = calculateThreatScore();
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "Threat score: %d", score);
            logMessage("DEBUG", buffer);

            if (score >= THREAT_THRESHOLD)
            {
                currentState = 2; // ALERT
                if (dist1 >= 0.0 && dist2 >= 0.0)
                {
                    // Move servo toward closer target
                    uint8_t target_angle = (dist1 < dist2) ? SERVO_LEFT_ANGLE : SERVO_RIGHT_ANGLE;
                    moveServoSmooth(target_angle);
                    currentServoPos = target_angle;
                }
                else if (dist1 >= 0.0 || dist2 >= 0.0)
                {
                    // If only one sensor is valid, move to center
                    moveServoSmooth(90);
                    currentServoPos = 90;
                }
                sendCameraCommand(CAMERA_CAPTURE_CMD);
            }
            else if (millis() - lastPatrolTime > PATROL_INTERVAL)
            {
                currentState = 1; // PATROLLING
                lastPatrolTime = millis();
                logMessage("INFO", "Entering PATROLLING state");
            }
        }
        break;

    case 1: // PATROLLING
        executePatrol();
        currentState = 0; // Return to IDLE
        lastPatrolTime = millis();
        logMessage("INFO", "Completed patrol, returning to IDLE");
        break;

    case 2: // ALERT
        updateSensors();
        handleAlertBlink();
        if (!motionDetected && distance >= 1.5)
        {
            currentState = 0;    // Return to IDLE
            moveServoSmooth(90); // Reset servo to center
            currentServoPos = 90;
            logMessage("INFO", "Threat cleared, returning to IDLE");
        }
        break;

    case 3: // FAULT
        handleFaultBlink();
        break;
    }

    // Handle communication tasks
    checkOTA();
    handleSerialComms();
}
