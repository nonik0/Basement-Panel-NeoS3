#include <Arduino.h>
#include <SPI.h>

#include "input.h"
#include "maze7x7.h"
#include "services.hpp"

#define DELAY_MS 10

InputTaskHandler inputTask;
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

  inputTask.createTask();
  mazeRunner.createTask();
}

void loop()
{
  ArduinoOTA.handle();
  restServer.handleClient();
  checkWifiStatus();

  delay(DELAY_MS);
}