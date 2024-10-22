#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_NeoKey_1x4.h>
#include <seesaw_neopixel.h>
#include <SPI.h>

#include "services.hpp"

// #define ACK1_PIN RX
// #define FIGPI_PIN 2
// #define M8X8_PIN1 3
// #define M8X8_PIN2 4
// #define ANANO_PIN 10

#define SS_NEOKEY_ADDR 0x30

#define SS_ROTARY_LED_PIN 6
#define SS_ROTARY_ENC_PIN 24
#define SS_ROTARY_ADDR 0x36

#define SS_ATTINY_ADDR 0x49
#define SS_ATTINY_LED_PIN 10

bool display = true;
bool isInput = display;

bool neokeyInit = false;
Adafruit_NeoKey_1x4 neokey;
NeoKey1x4Callback neokeyCallback(keyEvent evt);

bool rotaryInit = false;
Adafruit_seesaw rotarySs;
seesaw_NeoPixel rotaryRgbLed = seesaw_NeoPixel(1, SS_ROTARY_LED_PIN, NEO_GRB + NEO_KHZ800);
int32_t rotaryEncPos;
bool rotaryRgbLedState = false;

bool attinyInit = false;
Adafruit_seesaw attinySs;
unsigned long lastLedUpdate = 0;

uint32_t wheel(byte wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return seesaw_NeoPixel::Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170)
  {
    wheelPos -= 85;
    return seesaw_NeoPixel::Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return seesaw_NeoPixel::Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

void neokeySetup()
{
  if (!neokey.begin(SS_NEOKEY_ADDR))
  {
    Serial.println("Could not start NeoKey,check wiring?");
    while (1)
      delay(10);
  }
  Serial.println("NeoKey started!");

  neokey.pixels.setBrightness(20);

  for (uint16_t i = 0; i < neokey.pixels.numPixels(); i++)
  {
    neokey.pixels.setPixelColor(i, 0x808080);
    neokey.pixels.show();
    delay(50);
  }
  for (uint16_t i = 0; i < neokey.pixels.numPixels(); i++)
  {
    neokey.pixels.setPixelColor(i, 0x000000);
    neokey.pixels.show();
    delay(50);
  }

  for (int i = 0; i < NEOKEY_1X4_KEYS; i++)
  {
    neokey.registerCallback(i, neokeyCallback);
  }

  neokeyInit = true;
}

void neokeyUpdate()
{
  if (neokeyInit)
  {
    neokey.read();
  }
}

NeoKey1x4Callback neokeyCallback(keyEvent evt)
{
  uint8_t key = evt.bit.NUM;

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    Serial.print("Key press ");
    Serial.println(key);
    neokey.pixels.setPixelColor(key, wheel(map(key, 0, neokey.pixels.numPixels(), 0, 255)));
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    Serial.print("Key release ");
    Serial.println(key);

    neokey.pixels.setPixelColor(key, 0);
  }

  neokey.pixels.show();
  return 0;
}

void rotarySetup()
{
  Serial.println("Looking for rotary seesaw!");

  if (!rotarySs.begin(SS_ROTARY_ADDR) || !rotaryRgbLed.begin(SS_ROTARY_ADDR))
  {
    Serial.println("Couldn't find rotary seesaw on default address");
    while (1)
      delay(10);
  }
  Serial.println("rotary seesaw started");

  uint32_t version = ((rotarySs.getVersion() >> 16) & 0xFFFF);
  if (version != 4991)
  {
    Serial.print("Wrong firmware loaded? ");
    Serial.println(version);
    while (1)
      delay(10);
  }
  Serial.println("Found Product 4991");

  rotaryRgbLedState = true;
  rotaryRgbLed.setBrightness(20);
  rotaryRgbLed.show();

  rotarySs.pinMode(SS_ROTARY_ENC_PIN, INPUT_PULLUP);
  rotaryEncPos = rotarySs.getEncoderPosition();

  // Serial.println("Turning on interrupts");
  // delay(10);
  // rotarySs.setGPIOInterrupts((uint32_t)1 << SS_ROTARY_ENC_PIN, 1);
  // rotarySs.enableEncoderInterrupt();

  rotaryInit = true;
}

void rotaryUpdate()
{
  if (!rotaryInit)
  {
    return;
  }

  if (!rotarySs.digitalRead(SS_ROTARY_ENC_PIN))
  {
    rotaryRgbLedState = !rotaryRgbLedState;
  }

  if (rotaryRgbLedState)
  {
    rotaryRgbLed.setPixelColor(0, 0x000000);
    rotaryRgbLed.show();
  }
  else
  {
    int32_t newEncPos = rotarySs.getEncoderPosition();
    if (rotaryEncPos != newEncPos)
    {
      Serial.println(newEncPos);
      rotaryRgbLed.setPixelColor(0, wheel(newEncPos & 0xFF));
      rotaryRgbLed.show();
      rotaryEncPos = newEncPos;
    }
  }
}

void attinySetup()
{
  if (!attinySs.begin())
  {
    Serial.println("attiny seesaw not found!");
    while (1)
      delay(10);
  }
  Serial.println(F("attiny seesaw started OK!"));

  attinySs.pinMode(SS_ATTINY_LED_PIN, OUTPUT);

  attinyInit = true;
}

void attinyUpdate()
{
  if (attinyInit && millis() - lastLedUpdate > 1000)
  {
    lastLedUpdate = millis();
    attinySs.digitalWrite(SS_ATTINY_LED_PIN, !attinySs.digitalRead(SS_ATTINY_LED_PIN));
  }
}

void setup()
{
  delay(5000);

  // pinMode(ACK1_PIN, OUTPUT);
  // pinMode(FIGPI_PIN, OUTPUT);
  // pinMode(M8X8_PIN1, OUTPUT);
  // pinMode(M8X8_PIN2, OUTPUT);
  // pinMode(ANANO_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Starting setup...");

  wifiSetup();
  mDnsSetup();
  otaSetup();
  restSetup();

  neokeySetup();
  rotarySetup();
  attinySetup();
}

void loop()
{
  // digitalWrite(ACK1_PIN, display);
  // digitalWrite(FIGPI_PIN, display);
  // digitalWrite(ANANO_PIN, display);
  // digitalWrite(M8X8_PIN1, !display);

  // // high impedance mode when display is on, low output when display is off
  // if (display && !isInput)
  // {
  //   pinMode(M8X8_PIN2, INPUT);
  //   digitalWrite(M8X8_PIN2, false);
  //   isInput = true;
  //   delay(10);
  // }
  // else if (!display && isInput)
  // {
  //   pinMode(M8X8_PIN2, OUTPUT);
  //   digitalWrite(M8X8_PIN2, true);
  //   isInput = false;
  //   delay(10);
  // }

  neokeyUpdate();
  rotaryUpdate();
  attinyUpdate();

  ArduinoOTA.handle();
  restServer.handleClient();
  checkWifiStatus();
  delay(10);
}