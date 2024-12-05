#include <Arduino.h>

#include "matrix_8x16.h"
#include "maze_runner_7x7.h"
#include "music_matrix.h"
#include "wifi_services.h"

Matrix8x16TaskHandler matrix8x16;
MazeRunner7x7TaskHandler mazeRunner;
MusicMatrixTaskHandler musicMatrix;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  log_i("Starting setup...");

  wifiServices.setup(DEVICE_NAME);

  mazeRunner.createTask();
  matrix8x16.createTask();
  musicMatrix.createTask();
  wifiServices.createTask();

  wifiServices.registerSetDisplayCallback([](bool display)
                                          { matrix8x16.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { mazeRunner.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { musicMatrix.setDisplay(display); });


  log_i("Setup complete");
}

void loop()
{
}