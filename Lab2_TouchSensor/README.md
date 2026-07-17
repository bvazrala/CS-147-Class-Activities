# Lab 2 — Touch Sensor (UART, I2C, SPI)

Board: TTGO T-Display (ESP32, onboard 135x240 ST7789 via hardware SPI).

## Parts / Environments

One PlatformIO environment per part. Switch with the env selector in the
VS Code status bar, or `pio run -e partN -t upload`:

- `part1` — seconds-elapsed counter on the onboard display (TFT_eSPI)
- `part2` — CAP1188 over hardware I2C; display shows number of touched pads (0-8)
- `part3` — CAP1188 over software SPI on free GPIOs; same behavior, demoed separately

## Wiring (CAP1188 breakout)

Part 2 (hardware I2C):
- VIN -> 3V3, GND -> GND
- SDA -> GPIO 21, SCK/SCL -> GPIO 22
- AD/ADDR floating (default I2C address 0x29)

Part 3 (software SPI):
- VIN -> 3V3, GND -> GND
- SCK -> GPIO 25
- SDA/MISO -> GPIO 26
- WAKE/MOSI -> GPIO 27
- CS -> GPIO 33
- RESET -> GPIO 32

(Pins for part 3 are flexible; edit the #defines at the top of
src/part3_swspi_touch.cpp if you wire differently.)

## Expected Behavior

Part 1: display counts seconds since boot. Parts 2 and 3: display shows how
many CAP1188 pads are currently touched, updating live.
