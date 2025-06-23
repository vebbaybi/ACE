//include\alert_handler.h
#ifndef ALERT_HANDLER_H
#define ALERT_HANDLER_H

#include <Arduino.h>

// === ALERT HANDLER INTERFACE ===
// Provides LED-based visual feedback for threat and fault states.

// Controls LED blinking pattern based on active threat proximity.
// Higher threat = faster blink rate.
void handleAlertBlink();

// Controls LED blinking pattern to indicate system fault state,
// such as communication failure or sensor fault.
void handleFaultBlink();

#endif // ALERT_HANDLER_H
