#include <Arduino.h>

// #include "input.h"
#include "maze_runner_7x7.h"
#include "wifi_services.hpp"

// InputTaskHandler inputTask;
MazeRunner7x7TaskHandler mazeRunner;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  log_i("Starting setup...");

  wifiServices.setup(DEVICE_NAME);

  // inputTask.createTask();
  mazeRunner.createTask();
  wifiServices.createTask();

  wifiServices.registerSetDisplayCallback([](bool display)
                                          { mazeRunner.setDisplay(display); });

  log_i("Setup complete");
}

void loop()
{
}