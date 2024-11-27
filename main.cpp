#include <Arduino.h>

#include "maze_runner_7x7.h"
#include "music_matrix.h"
#include "wifi_services.h"

MusicMatrixTaskHandler musicMatrix;
MazeRunner7x7TaskHandler mazeRunner;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  log_i("Starting setup...");

  wifiServices.setup(DEVICE_NAME);

  musicMatrix.createTask();
  mazeRunner.createTask();
  wifiServices.createTask();

  wifiServices.registerSetDisplayCallback([](bool display)
                                          { musicMatrix.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { mazeRunner.setDisplay(display); });

  log_i("Setup complete");
}

void loop()
{
}