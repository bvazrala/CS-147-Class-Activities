#include <Arduino.h>
#include "cs147_common.h"
#include "cs147_display.h"

// Shared global variable between tasks
volatile int globalPotValue = 0;

// Task 1: Samples the potentiometer input
void taskSamplePot(void *pvParameters) {
  pinMode(CS147Pins::POT_ADC, INPUT);
  
  for (;;) {
    // TODO 1: Read the analog value from the potentiometer pin and store it in globalPotValue
    globalPotValue = analogRead(CS147Pins::POT_ADC);

    // TODO 2: Delay the task for 100ms using FreeRTOS vTaskDelay() to yield CPU time
    // Hint: Use pdMS_TO_TICKS(100)
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// Task 2: Reports status to the Serial monitor and OLED
void taskReportStatus(void *pvParameters) {
  uint32_t lastPrint = 0;
  for (;;) {
    // TODO 3: Check if 1000ms have passed using cs147Every.
    // If true, print the globalPotValue to Serial and write it to the OLED display.
    if (cs147Every(1000, lastPrint)) {
      Serial.print("Potentiometer Value: ");
      Serial.println(globalPotValue);
      cs147DisplayLines("C04 FreeRTOS Tasks", "Pot Value:", String(globalPotValue));
    }

    // TODO 4: Yield control with a short vTaskDelay of 10ms to keep the watchdog timer happy
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void setup() {
  cs147StartSerial("C04 FreeRTOS Tasks");
  cs147InitSPI();
  cs147InitDisplay();

  // TODO 5: Create taskSamplePot using xTaskCreate()
  // Assign it a stack size of 2048, no parameters, priority 1, and no task handle output.
  xTaskCreate(taskSamplePot, "SamplePot", 2048, NULL, 1, NULL);

  // TODO 6: Create taskReportStatus using xTaskCreate() with the same configuration.
  xTaskCreate(taskReportStatus, "ReportStatus", 2048, NULL, 1, NULL);
}

void loop() {
  // This remains empty, FreeRTOS tasks manage execution execution lines independently.
}