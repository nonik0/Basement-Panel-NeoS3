#include <Arduino.h>

#include "maze_runner.h"
#include "music_matrix.h"
#include "tunnel_runner.h"
#include "wifi_services.h"

MazeRunnerTaskHandler mazeRunner;
MusicMatrixTaskHandler musicMatrix;
TunnelRunnerTaskHandler tunnelRunner;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  delay(5000);
  log_i("Starting setup...");

  wifiServices.setup(DEVICE_NAME);

  // frequent heap issue when tasks are created quickly, delays seem to help
  tunnelRunner.createTask();
  delay(1000);
  mazeRunner.createTask();
  delay(1000);
  musicMatrix.createTask();
  delay(1000);
  wifiServices.createTask();

  wifiServices.registerSetDisplayCallback([](bool display)
                                          { mazeRunner.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { musicMatrix.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { tunnelRunner.setDisplay(display); });

  log_i("Setup complete");
}

void loop()
{
}