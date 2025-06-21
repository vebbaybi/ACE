#ifndef ALERT_HANDLER_H
#define ALERT_HANDLER_H

#include <Arduino.h>

// === Function Prototypes ===
void handleAlertBlink(); // Manages LED blinking based on threat proximity
void handleFaultBlink(); // Manages LED blinking to signal fault state

#endif // ALERT_HANDLER_H