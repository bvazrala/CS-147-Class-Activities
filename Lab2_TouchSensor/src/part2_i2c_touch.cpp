#include <Arduino.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <Adafruit_CAP1188.h>

TFT_eSPI tft;
Adafruit_CAP1188 cap;   // hardware I2C: SDA=21, SCL=22, addr 0x29

void setup()
{
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);

  if (!cap.begin())
  {
    tft.drawString("CAP1188 not found", tft.width() / 2, tft.height() / 2, 4);
    while (true) delay(100);
  }
  cap.writeRegister(0x2A, 0);   // allow multiple simultaneous touches

  tft.drawString("Touched pads (HW I2C)", tft.width() / 2, 20, 2);
}

void loop()
{
  static int lastCount = -1;

  uint8_t bits = cap.touched();
  int count = 0;
  for (int i = 0; i < 8; i++)
    if (bits & (1 << i)) count++;

  if (count != lastCount)
  {
    lastCount = count;
    tft.setTextPadding(tft.width());
    tft.drawNumber(count, tft.width() / 2, tft.height() / 2 + 15, 7);
  }
  delay(50);
}
