#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft;

void setup()
{
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Seconds since start", tft.width() / 2, 20, 2);
}

void loop()
{
  static uint32_t lastShown = 0xFFFFFFFF;
  uint32_t seconds = millis() / 1000;

  if (seconds != lastShown)
  {
    lastShown = seconds;
    tft.setTextPadding(tft.width());
    tft.drawNumber(seconds, tft.width() / 2, tft.height() / 2 + 15, 7);
  }
}
