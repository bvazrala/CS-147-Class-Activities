#include <Arduino.h>
#include "cs147_common.h"

void setup() {
  cs147StartSerial("C02 Blink LED");

  // TODO 1: Configure LED_1 as an output.
  pinMode(CS147Pins::LED_1, OUTPUT);
}

void loop() {
  // TODO 2: Turn LED_1 on
  digitalWrite(CS147Pins::LED_1, HIGH);
  // TODO 3: Wait 500 ms.
  delay(500);
  // TODO 4: Turn LED_1 off.
  digitalWrite(CS147Pins::LED_1, LOW);
  // TODO 5: Wait 500 ms.
  delay(500);
}
