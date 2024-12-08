#include <Arduino.h>

//#include "matrix_8x16.h"
#include "maze_runner_7x7.h"
#include "music_matrix.h"
#include "tunnel_runner_8x16.h"
#include "wifi_services.h"

//Matrix8x16TaskHandler matrix8x16;
MazeRunner7x7TaskHandler mazeRunner;
MusicMatrixTaskHandler musicMatrix;
TunnelRunner8x16TaskHandler tunnelRunner;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  delay(5000);
  log_i("Starting setup...");

  wifiServices.setup(DEVICE_NAME);

  mazeRunner.createTask();
  //matrix8x16.createTask();
  musicMatrix.createTask();
  tunnelRunner.createTask();
  wifiServices.createTask();

  // wifiServices.registerSetDisplayCallback([](bool display)
  //                                         { matrix8x16.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { mazeRunner.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { musicMatrix.setDisplay(display); });
  wifiServices.registerSetDisplayCallback([](bool display)
                                          { tunnelRunner.setDisplay(display); });

  // wifiServices.registerSetMessageCallback("/8x16", [](const char *message)
  //                                         { if (strlen(message) > 0)  matrix8x16.setMessage(message);
  //                                           return matrix8x16.getMessage(); });

  log_i("Setup complete");
}

void loop()
{
}