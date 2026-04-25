# Smart Exhaust and Safety System

A real-time environmental monitoring system built with Arduino. This project monitors ambient temperature and gas levels to ensure workspace safety, specifically designed for PES University lab environments.

## Features
* **Real-time Monitoring:** Tracks temperature using LM235Z and gas levels via MQ-2.
* **Automated Ventilation:** PWM-controlled fan that adjusts speed based on risk levels.
* **Safety Alerts:** High-decibel buzzer and LED indicators for "DANGER" states.
* **User Control:** Includes a physical "Silence" button to mute alarms while maintaining visual alerts.
* **I2C LCD Interface:** Displays system status and live sensor data.

## Hardware Components
* Arduino Uno R3
* LM235Z Temperature Sensor
* MQ-2 Gas Sensor
* I2C LCD (16x2)
* 12V DC Fan (PWM controlled)
* Buzzer & LED

## Setup
1. Connect components according to the pin definitions in `src/smart_exhaust.ino`.
2. Ensure the `Wire` and `LiquidCrystal_I2C` libraries are installed in your Arduino IDE.
3. Upload the code and monitor the LCD for "SYSTEM NORMAL" status.
