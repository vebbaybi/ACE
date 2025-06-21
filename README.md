# ACE

# ACEBOT - Autonomous Cognitive Evo

**ACEBOT** is a modular, Arduino Uno R4-based, voice-activated companion robot designed for real-time motion tracking, threat analysis, patrol behavior, and visual feedback. Built using `PlatformIO` and organized for robustness, ACEBOT serves as a home assistant prototype blending embedded intelligence with edge-controlled peripherals like PIR, ultrasonic sensors, and ESP32-CAM.

---

## 🚀 Project Overview

ACEBOT is structured into a full-stack firmware project with clean separation of concerns across:

- **Sensor Input**: Dual ultrasonic sensors and PIR motion detection.
- **Motion Logic**: Threat scoring, state transitions (IDLE, PATROLLING, ALERT, FAULT).
- **Actuation**: Pan servo for directional tracking based on proximity logic.
- **Feedback System**: LED blinking speed varies with proximity; fault indications.
- **ESP32-CAM Comms**: Optional visual confirmation via camera command relay.
- **OTA/Serial Handling**: Future-proof interface for software control and diagnostics.

---

## 📁 Folder Structure

```plaintext
acebot/
├── src/
│   ├── main.cpp                # Entry point and loop orchestration
│   ├── system_init.cpp        # Initializes pins, peripherals, watchdog
│   ├── sensors.cpp            # PIR & ultrasonic data reading
│   ├── motion_logic.cpp       # State machine & threat score logic
│   ├── servo_controller.cpp   # Smooth servo transitions & patrol logic
│   ├── alert_handler.cpp      # LED blink speed logic for threat/fault states
│   ├── comms.cpp              # ESP32-CAM comms & serial parsing
│
├── include/
│   ├── system_init.h
│   ├── sensors.h
│   ├── motion_logic.h
│   ├── servo_controller.h
│   ├── alert_handler.h
│   ├── comms.h
│
├── platformio.ini             # PlatformIO build configuration
