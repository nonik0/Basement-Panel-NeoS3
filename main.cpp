#include <Arduino.h>
#include <SPI.h>

//#include "input.h"
#include "maze_runner_7x7.h"
#include "wifi_services.hpp"

#define DELAY_MS 10

#ifndef DEVICE_NAME
#define DEVICE_NAME "what"
#endif

//InputTaskHandler inputTask;
MazeRunner7x7TaskHandler mazeRunner;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  delay(3000); // let I2C devices settle
  log_i("Starting setup...");

  wifiServices.setup(DEVICE_NAME);
  wifiServices.start();

  //inputTask.createTask();
  mazeRunner.createTask();
}

void loop()
{
}