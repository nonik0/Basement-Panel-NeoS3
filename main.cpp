#include <Arduino.h>
#include <SPI.h>

#include "input.h"
#include "maze7x7.h"
#include "services.hpp"

#define DELAY_MS 10

MazeRunnerTaskHandler mazeRunner;

void setup()
{
  Serial.begin(115200);
  delay(3000); // let I2C devices settle
  log_i("Starting setup...");

  wifiSetup();
  mDnsSetup();
  otaSetup();
  restSetup();

  alphaNumSetup();
  neoKeySetup();
  neoSliderSetup();
  rotarySetup();
  delay(1000);
  ack1Setup(); // slow to initialize

  mazeRunner.createTask();
}

void loop()
{
  ack1Update();
  alphaNumUpdate();
  neoKeyUpdate();
  neoSliderUpdate();
  rotaryUpdate();
  inputUpdate();

  ArduinoOTA.handle();
  restServer.handleClient();
  checkWifiStatus();

  delay(DELAY_MS);
}