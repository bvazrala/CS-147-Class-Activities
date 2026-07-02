#include <Arduino.h>
#include "cs147_common.h"

volatile uint32_t pressCount = 0;
volatile uint32_t lastInterruptMs = 0;

void IRAM_ATTR onButtonPress() {
  // TODO 1: Add simple debounce logic.
  uint32_t currentMs = millis();
  if (currentMs - lastInterruptMs >= 150) {
    lastInterruptMs = currentMs;
  // TODO 2: Increment pressCount.
    pressCount++;
  }
}

void setup() {
  cs147StartSerial("C03 Button Interrupts");
  // TODO 3: Configure BUTTON_1 as INPUT_PULLUP.
  pinMode(CS147Pins::BUTTON_1, INPUT_PULLUP);
  // TODO 4: Attach onButtonPress() to BUTTON_1 using FALLING edge.
  attachInterrupt(CS147Pins::BUTTON_1, onButtonPress, FALLING);
}

void loop() {
  static uint32_t lastPrint = 0;
  if (cs147Every(500, lastPrint)) {
    // TODO 5: Print pressCount.
  Serial.println("Button presses: ");
  Serial.println(pressCount);
  }
}
