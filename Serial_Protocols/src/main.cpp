#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "cs147_common.h"
#include "cs147_display.h"

int i2cDeviceCount = 0;
int oledUpdateCount = 0;
int bytesSentOverUart = 0;

String identifyI2CDevice(uint8_t addr) {
  // TODO 6: Return the known device name for each I2C address.
  if (addr == 0x60) {
    return "ATECC608A Secure Element";
  }
  if (addr == 0x68) {
    return "MPU6050 Motion Sensor";
  }
  if (addr == 0x76 || addr == 0x77) {
    return "BMP280 Pressure Sensor";
  }
  return "Unknown";
}

int scanI2CBus() {
  int count = 0;

  Serial.println("Scanning I2C bus...");

  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);

    if (Wire.endTransmission() == 0) {
      // TODO 5: Print the discovered I2C address.
      Serial.print("Found I2C device at 0x");
      if (addr < 16) {
        Serial.print("0");
      }
      Serial.print(addr, HEX);

      // TODO 7: Print the device name using identifyI2CDevice(addr).
      Serial.print(" -> ");
      Serial.println(identifyI2CDevice(addr));

      count++;
    }
  }

  Serial.print("Total I2C devices found: ");
  Serial.println(count);
  return count;
}

void setup() {
  // TODO 1: Start UART communication with the computer.
  cs147StartSerial("C05 Serial Protocols");

  Serial.println();
  Serial.println("This activity compares UART, I2C, and SPI.");
  Serial.println("UART: Serial Monitor");
  Serial.println("I2C : BMP280 / MPU6050 / ATECC608A");
  Serial.println("SPI : OLED display");
  Serial.println();

  // TODO 2: Initialize the I2C bus.
  cs147InitI2C();

  // TODO 3: Initialize the SPI bus.
  cs147InitSPI();

  // TODO 4: Initialize the OLED display and store whether it succeeded.
  bool displayOk = cs147InitDisplay();

  if (displayOk) {
    cs147DisplayLines(
      "C05 Protocols",
      "UART: Ready",
      "I2C: Scanning",
      "SPI: OLED Ready"
    );
  }

  // TODO 8: Scan the I2C bus and store the number of detected devices.
  i2cDeviceCount = scanI2CBus();

  if (displayOk) {
    // TODO 9: Display UART, I2C device count, and SPI status on the OLED.
    String i2cLine = "I2C devs: " + String(i2cDeviceCount);
    cs147DisplayLines(
      "C05 Protocols",
      "UART: Active",
      i2cLine.c_str(),
      "SPI: OLED OK"
    );
  }
}

void loop() {
  static uint32_t lastUpdate = 0;

  if (cs147Every(2000, lastUpdate)) {
    // TODO 10: Read the potentiometer value.
    int potValue = analogRead(CS147Pins::POT_ADC);

    // TODO 11: Print a UART status report to the Serial Monitor.
    Serial.println("=== Protocol Status Report ===");
    Serial.println("UART: Active");
    Serial.print("I2C Devices: ");
    Serial.println(i2cDeviceCount);
    Serial.print("Potentiometer: ");
    Serial.println(potValue);
    Serial.print("OLED Updates: ");
    Serial.println(oledUpdateCount);
    Serial.print("Approx. UART Bytes Sent: ");
    Serial.println(bytesSentOverUart);
    Serial.println();

    // TODO 12: Update bytesSentOverUart.
    bytesSentOverUart += 80;

    // TODO 13: Update the OLED with current protocol status.
    String uartLine = "UART B: " + String(bytesSentOverUart);
    String i2cLine  = "I2C devs: " + String(i2cDeviceCount);
    String spiLine  = "SPI upd: " + String(oledUpdateCount);
    String potLine  = "Pot: " + String(potValue);

    cs147DisplayLines(
      uartLine.c_str(),
      i2cLine.c_str(),
      spiLine.c_str(),
      potLine.c_str()
    );

    // TODO 14: Increment oledUpdateCount after updating the OLED.
    oledUpdateCount++;
  }
}
