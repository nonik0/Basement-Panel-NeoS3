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
  wifiServices.createTask();
  delay(3000);

  // frequent heap issue when tasks are created quickly, delays seem to help
  mazeRunner.createTask();
  delay(3000);
  musicMatrix.createTask();
  delay(3000);
  tunnelRunner.createTask();

  wifiServices.registerSetDisplayCallback([](bool display)
                                          { mazeRunner.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { musicMatrix.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { tunnelRunner.setDisplay(display); });

  wifiServices.registerSetMessageCallback("/ack1", [](const char *message)
                                          { if (strlen(message) > 0)  musicMatrix.setMessage(message);
                                            return musicMatrix.getMessage(); });

  log_i("Setup complete");
}

void loop()
{
}