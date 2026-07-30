#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define LED_PIN 2

BLECharacteristic *ledChar;

class ServerCallbacks : public BLEServerCallbacks
{
  void onDisconnect(BLEServer *server)
  {
    BLEDevice::startAdvertising();   // stay discoverable after phone disconnects
  }
};

class LedCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *c)
  {
    std::string v = c->getValue();

    if (v == "ON" || v == "on" || v == "1")
    {
      digitalWrite(LED_PIN, HIGH);
      c->setValue("LED is ON");
      Serial.println("LED switched ON");
    }
    else if (v == "OFF" || v == "off" || v == "0")
    {
      digitalWrite(LED_PIN, LOW);
      c->setValue("LED is OFF");
      Serial.println("LED switched OFF");
    }
    else
    {
      Serial.print("Unknown command: ");
      Serial.println(v.c_str());
    }
  }
};

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  BLEDevice::init("CS147-LED");
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService *service = server->createService(SERVICE_UUID);
  ledChar = service->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  ledChar->setCallbacks(new LedCallbacks());
  ledChar->setValue("LED is OFF");
  service->start();

  BLEAdvertising *adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("BLE ready: write ON or OFF to the characteristic");
}

void loop()
{
  delay(100);
}
