// main.cpp

#include <Arduino.h>
#include "system_init.h"
#include "motion_logic.h"

    // === Arduino Setup ===
    void
    setup()
{
    initializeSystem();
    logMessage("INFO", "Arduino setup completed");
}

// === Arduino Loop ===
void loop()
{
    if (isInitialized)
    {
        runStateMachine();
    }
    else
    {
        logMessage("ERROR", "System not initialized, skipping state machine");
        delay(1000); // Wait before retrying
    }
}
