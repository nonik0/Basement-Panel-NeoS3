#include <Arduino.h>

#include "services.hpp"

#define ACK1_PIN RX

bool display = true;

void setup()
{
  delay(5000);

  pinMode(ACK1_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Starting setup...");

  wifiSetup();
  mDnsSetup();
  otaSetup();
  restSetup();
}

void loop()
{
  digitalWrite(ACK1_PIN, display);

  ArduinoOTA.handle();
  restServer.handleClient();
  checkWifiStatus();
  delay(10);
}