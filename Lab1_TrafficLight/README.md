# Lab 1 — Highway Traffic Light

## Goal

Implement a traffic light controller with three LEDs, a buzzer, and a touch button on the ESP32.

## Wiring

- Red LED: GPIO 32
- Yellow LED: GPIO 33
- Green LED: GPIO 25
- Buzzer: GPIO 26
- Touch button: GPIO 4 (T0, built-in capacitive touch)

## Expected Behavior

RED (10 s) -> RED+YELLOW (2 s) -> GREEN (held until touch, then 5 s more) -> YELLOW (2 s) -> RED.
Buzzer: 500 ms on / 1500 ms off in green, 250 ms on / 250 ms off in red, silent in transitions.
