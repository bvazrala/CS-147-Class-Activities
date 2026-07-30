# Lab 3 — BLE Connectivity

Board: ESP32 (TTGO). BLE uses the ESP32 Arduino core's built-in BLE stack
(no extra library needed). Same service/characteristic UUIDs as the handout.

## Parts / Environments

Switch env in the VS Code status bar, or `pio run -e partX -t upload`:

- `partA` — LED over BLE: phone writes `ON` / `OFF` (or `1` / `0`) to the
  characteristic to switch the LED; current state is readable back.
- `partB` — Step counter: LSM6DSO accelerometer over I2C, own threshold-based
  step detection (no pedometer library), count printed to serial and pushed
  to the phone via BLE notify whenever it changes.

## Wiring

LSM6DSO (part B): VDD -> 3V3 ONLY (5 V permanently damages the board),
GND -> GND, SDA -> GPIO 21, SCL -> GPIO 22.

LED (part A): GPIO 2. That is the onboard LED on DevKit-style boards; on the
TTGO T-Display wire an external LED + ~220 ohm resistor from GPIO 2 to GND.

## Phone side (nRF Connect)

Part A: scan, connect to `CS147-LED`, open the characteristic
(beb5483e-...), tap the up arrow, send `ON` or `OFF` as TEXT.

Part B: connect to `CS147-StepCounter`, tap the multiple-down-arrows icon on
the characteristic to enable notifications; the value updates on every step.
Keep the board still for the ~2 s calibration right after reset.

## Tuning (part B)

STEP_THRESHOLD, REARM_THRESHOLD, and STEP_GAP_MS at the top of
`src/partB_step_counter.cpp`. Mimic steps while watching the serial monitor
and adjust until one motion = one count.
