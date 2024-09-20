#include <Arduino.h>

#include "services.hpp"

#define ACK1_PIN RX
#define M8X8_PIN1 3
#define M8X8_PIN2 4
#define M8x8_NUM_LEDS 64
#define ANANO_PIN 10

bool display = true;

// default checkboard pattern
bool m8x8[M8x8_NUM_LEDS] = {
    1, 0, 1, 0, 1, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 0, 1, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 0, 1, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 1,
    1, 0, 1, 0, 1, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0, 1};

bool isInput = display;
void setup()
{
  delay(5000);

  pinMode(ACK1_PIN, OUTPUT);
  pinMode(M8X8_PIN1, OUTPUT);
  pinMode(M8X8_PIN2, OUTPUT);
  pinMode(ANANO_PIN, OUTPUT);

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
  digitalWrite(ANANO_PIN, display);

  // high impedance mode when display is on, low output when display is off
  if (display && !isInput)
  {
    pinMode(M8X8_PIN2, INPUT);
    digitalWrite(M8X8_PIN2, false);
    isInput = true;
    delay(10);
  }
  else if (!display && isInput)
  {
    pinMode(M8X8_PIN2, OUTPUT);
    digitalWrite(M8X8_PIN2, true);
    isInput = false;
    delay(10);
  }

  digitalWrite(M8X8_PIN1, !display);

  ArduinoOTA.handle();
  restServer.handleClient();
  checkWifiStatus();
  delay(10);
}