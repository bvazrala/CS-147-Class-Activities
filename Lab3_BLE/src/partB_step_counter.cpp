#include <Arduino.h>
#include <Wire.h>
#include "SparkFunLSM6DSO.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SAMPLE_MS        20      // read the sensor every 20 ms
#define CALIB_SAMPLES    100
#define STEP_THRESHOLD   0.35f   // g above baseline that counts as a step
#define REARM_THRESHOLD  0.20f   // must fall back below this before next step
#define STEP_GAP_MS      300     // minimum time between two steps

LSM6DSO imu;
BLECharacteristic *stepChar;

float baseline = 0.0f;
int stepCount = 0;
bool armed = true;
unsigned long lastStepMs = 0;

float readMagnitudeXY()
{
  float x = imu.readFloatAccelX();
  float y = imu.readFloatAccelY();
  return sqrtf(x * x + y * y);
}

class ServerCallbacks : public BLEServerCallbacks
{
  void onDisconnect(BLEServer *server)
  {
    BLEDevice::startAdvertising();
  }
};

void setup()
{
  Serial.begin(115200);

  Wire.begin();   // SDA=21, SCL=22
  if (!imu.begin())
  {
    Serial.println("LSM6DSO not found, check wiring");
    while (true) delay(100);
  }
  imu.initialize(BASIC_SETTINGS);

  // Calibration: keep the board still until "Baseline" is printed.
  Serial.println("Calibrating, keep the board still...");
  float sum = 0.0f;
  for (int i = 0; i < CALIB_SAMPLES; i++)
  {
    sum += readMagnitudeXY();
    delay(SAMPLE_MS);
  }
  baseline = sum / CALIB_SAMPLES;
  Serial.print("Baseline: ");
  Serial.println(baseline, 3);

  BLEDevice::init("CS147-StepCounter");
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);
  stepChar = service->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  stepChar->addDescriptor(new BLE2902());
  stepChar->setValue((uint8_t *)"0", 1);
  service->start();

  BLEAdvertising *adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("BLE advertising as CS147-StepCounter");
}

void loop()
{
  static unsigned long lastSample = 0;
  unsigned long now = millis();
  if (now - lastSample < SAMPLE_MS) return;
  lastSample = now;

  float deviation = fabsf(readMagnitudeXY() - baseline);

  // Count on the rising edge only; re-arm after the signal settles.
  if (armed && deviation > STEP_THRESHOLD && now - lastStepMs > STEP_GAP_MS)
  {
    armed = false;
    lastStepMs = now;
    stepCount++;

    Serial.print("Steps: ");
    Serial.println(stepCount);

    char buf[16];
    snprintf(buf, sizeof(buf), "%d", stepCount);
    stepChar->setValue((uint8_t *)buf, strlen(buf));
    stepChar->notify();
  }
  else if (!armed && deviation < REARM_THRESHOLD)
  {
    armed = true;
  }
}
