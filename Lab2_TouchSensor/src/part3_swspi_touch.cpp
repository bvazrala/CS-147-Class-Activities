#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Adafruit_CAP1188.h>

#define CAP_CLK   25
#define CAP_MISO  26
#define CAP_MOSI  27
#define CAP_CS    33
#define CAP_RST   32

TFT_eSPI tft;
Adafruit_CAP1188 cap(CAP_CLK, CAP_MISO, CAP_MOSI, CAP_CS, CAP_RST);   // software SPI

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

  tft.drawString("Touched pads (SW SPI)", tft.width() / 2, 20, 2);
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
