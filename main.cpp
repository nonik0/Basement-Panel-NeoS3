#include <Arduino.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_NeoKey_1x4.h>
#include <seesaw_neopixel.h>
#include <SPI.h>

#include "services.hpp"

#define ACK1_ADDR 0x10
#define ACK1_STATUS_CMD 0x01
#define ACK1_BRIGHTNESS_CMD 0x08
#define ACK1_COLOR_BG_CMD 0x09
#define ACK1_COLOR_FG_CMD 0x0A
#define ACK1_LEDON_CMD 0x11
#define ACK1_LEDOFF_CMD 0x12
#define ACK1_LEDASCII_CMD 0x13
#define ACK1_LEDSCROLL_CMD 0x14
#define ACK1_LEDCLR_CMD 0x15
#define ACK1_TONEON_CMD 0x16
#define ACK1_TONEOFF_CMD 0x17
#define ACK1_EEPROM_READ_CMD 0x80
#define ACK1_EEPROM_WRITE_CMD 0x81

#define SS_NEOKEY_ADDR 0x30

#define SS_ROTARY_LED_PIN 6
#define SS_ROTARY_BTN_PIN 24
#define SS_ROTARY_ADDR 0x36

#define SS_ATTINY_ADDR 0x49
#define SS_ATTINY_LED_PIN 10
#define SS_ATTINY_BTN_PIN 15
#define SS_ATTINY_SLD_PIN 16

bool display = true;
bool isInput = display;

bool ack1Init = false;
bool ack1Display = false;
const char *ack1BootMessage = "Hello world!";
const char *ack1Message = "Dad loves Stella and Beau!";\
const int BaseToneFrequencies1[4] = {261, 293, 329, 349}; // C4, D4, E4, F4
const int BaseToneFrequencies2[4] = {523, 587, 659, 698}; // C5, D5, E5, F5

Adafruit_NeoKey_1x4 neokey;
bool neokeyInit = false;
bool neokeyToggle = false;
bool neokeyState[4] = {true, true, true, true};
NeoKey1x4Callback neokeyCallback(keyEvent evt);

Adafruit_seesaw rotarySs;
bool rotaryInit = false;
seesaw_NeoPixel rotaryRgbLed = seesaw_NeoPixel(1, SS_ROTARY_LED_PIN, NEO_GRB + NEO_KHZ800);
int32_t rotaryEncPos;
bool rotaryBtnIsPressed = false;
unsigned long rotaryLastBtnPress = 0;

Adafruit_seesaw attinySs;
bool attinyInit = false;
bool attinyButtonState = false;
unsigned long attinyLastSliderRead = 0;
unsigned long attinyLastSliderChange = 0;
uint16_t attinySliderReading = 0;
// unsigned long lastLedUpdate = 0;

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

bool ack1Response(size_t len)
{
  size_t recv = Wire.requestFrom(ACK1_ADDR, len);
  if (recv != len)
  {
    log_e("%d response bytes expected, %d bytes received", len, recv);
    return false;
  }

  if (recv > 32)
  {
    log_e("Response too long, max 32 bytes");
    return false;
  }

  uint8_t responseBytes[32];
  for (size_t i = 0; i < recv; i++)
  {
    responseBytes[i] = Wire.read();
    log_d("Reading response byte %d: %02x", i, responseBytes[i]);
  }

  if (len == 1 && responseBytes[0] != 0)
  {
    log_e("Response code is not 0: %02x", responseBytes[0]);
    return false;
  }

  return true;
}

bool ack1Command(uint8_t cmd, const uint8_t *data = NULL, size_t len = 0)
{
  if (!ack1Init)
  {
    log_e("ACK1 not initialized!");
    return false;
  }

  uint8_t response[2];
  bool success = true;

  Wire.beginTransmission(ACK1_ADDR);

  size_t responseBytes = 0;
  switch (cmd)
  {
  case ACK1_LEDCLR_CMD:
    log_d("Sending LEDCLR command");
    Wire.write(ACK1_LEDCLR_CMD);
    ack1Display = false;
    break;
  case ACK1_LEDSCROLL_CMD:
    log_d("Sending LEDSCROLL command");
    Wire.write(ACK1_LEDSCROLL_CMD);
    Wire.write(static_cast<uint8_t>(len));
    Wire.write(data, len);
    ack1Display = true;
    break;
  case ACK1_TONEON_CMD:
    log_d("Sending TONEON command");
    Wire.write(ACK1_TONEON_CMD);
    if (len != 2)
    {
      log_e("TONEON command requires 2 bytes of data");
      success = false;
      break;
    }
    Wire.write(data, 2);
    break;
  case ACK1_TONEOFF_CMD:
    log_d("Sending TONEOFF command");
    Wire.write(ACK1_TONEOFF_CMD);
    break;
  case ACK1_STATUS_CMD:
    log_d("Sending status command");
    Wire.write(ACK1_STATUS_CMD);
    responseBytes = 1;
    break;
  default:
    log_d("Sending default command");
    Wire.write(ACK1_STATUS_CMD);
    responseBytes = 1;
    break;
  }

  bool endTransmissionSuccess = Wire.endTransmission();
  if (endTransmissionSuccess != 0)
  {
    log_e("Failed to end transmission, error: %d", endTransmissionSuccess);
    // return false;
  }

  if (responseBytes > 0)
  {
    success &= ack1Response(responseBytes);
  }
  else {
    // TODO: not sure if I need status command after all commands??
    Wire.beginTransmission(ACK1_ADDR);
    Wire.write(ACK1_STATUS_CMD);
    Wire.endTransmission();
    success &= ack1Response(1);
  }

  return success;
}

void ack1Wake()
{
  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)ack1Message, strlen(ack1Message));
}

void ack1Clear()
{
  // one clear only stops scroll, second clear clears display (not sure why?)
  ack1Command(ACK1_LEDCLR_CMD);
  // ack1Command(ACK1_LEDCLR_CMD);
}

void ack1Tone(uint16_t freq)
{
  if (freq == 0)
  {
    ack1Command(ACK1_TONEOFF_CMD);
    return;
  }

  uint8_t toneData[2];
  // freq = ((params[0]) << 8) | (params[1]);
  toneData[0] = freq >> 8;
  toneData[1] = freq & 0xFF;
  log_d("Tone freq: %d", freq);
  log_d("Tone data: %02x %02x", toneData[0], toneData[1]);
  ack1Command(ACK1_TONEON_CMD, toneData, 2);
}

void ack1Setup()
{
  if (!Wire.begin())
  {
    Serial.println("I2C not started!");
    return;
  }

  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)ack1BootMessage, strlen(ack1BootMessage));
  log_d("ACK1 status: %d", ack1Command(ACK1_STATUS_CMD));
  ack1Display = true;
  ack1Init = true;
}

void ack1Update()
{
  if (!ack1Init)
  {
    return;
  }

  if (display && !ack1Display)
  {
    ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)ack1Message, strlen(ack1Message));
  }
  else if (!display)
  {
    ack1Clear();
  }
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

  neokey.pixels.setBrightness(40);

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

void neokeyChangeToggle()
{
  neokeyToggle = !neokeyToggle;

  for (uint16_t i = 0; i < neokey.pixels.numPixels(); i++)
  {
    uint32_t color = 0x000000;

    if (neokeyToggle && neokeyState[i])
    {
      color = wheel(map(i, 0, neokey.pixels.numPixels(), 0, 255));
    }
    else if (!neokeyToggle && attinySliderReading > (1023 / neokey.pixels.numPixels()) * i)
    {
      color = wheel(map(i, 0, neokey.pixels.numPixels(), 0, 255));
    }

    neokey.pixels.setPixelColor(i, color);
  }

  neokey.pixels.show();
}

void neokeyUpdate()
{
  if (!neokeyInit)
  {
    return;
  }

  neokey.read();
}

NeoKey1x4Callback neokeyCallback(keyEvent evt)
{
  uint8_t key = evt.bit.NUM;
  uint32_t color = 0x0; // default event to turn LED off
  uint16_t tone = 0x0;

  if (neokeyToggle)
  {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    {
      neokeyState[key] = !neokeyState[key];
      tone = BaseToneFrequencies2[key];
    }

    if (neokeyState[key])
    {
      color = wheel(map(key, 0, neokey.pixels.numPixels(), 0, 255));
    }
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    color = wheel(map(key, 0, neokey.pixels.numPixels(), 0, 255));
    tone = BaseToneFrequencies1[key];
  }

  if (tone > 0)
  {
    int pitchModifier = map(attinySliderReading, 0, 1023, -200, 200);
    tone += pitchModifier;
  }

  // only play tone if button is pressed
  tone = attinyButtonState ? tone : 0;

  neokey.pixels.setPixelColor(key, color);
  neokey.pixels.show();
  ack1Tone(tone);

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

  rotaryRgbLed.setBrightness(20);
  rotaryRgbLed.show();
  rotarySs.pinMode(SS_ROTARY_BTN_PIN, INPUT_PULLUP);
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

  if (!rotarySs.digitalRead(SS_ROTARY_BTN_PIN))
  {
    if (!rotaryBtnIsPressed && millis() - rotaryLastBtnPress > 100)
    {
      rotaryBtnIsPressed = true;
      rotaryLastBtnPress = millis();
      neokeyChangeToggle();

      if (attinyButtonState) {
        ack1Tone(1000);
        delay(100);
        ack1Tone(0);
      }
    }
  }
  else
  {
    rotaryBtnIsPressed = false;
  }

  int32_t newEncPos = rotarySs.getEncoderPosition();
  if (rotaryEncPos != newEncPos)
  {
    Serial.println(newEncPos);
    rotaryRgbLed.setPixelColor(0, wheel((newEncPos * 5) & 0xFF)); // 3 rotations for full cycle with 24 step encoder?
    rotaryRgbLed.show();
    rotaryEncPos = newEncPos;
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
  attinySs.pinMode(SS_ATTINY_BTN_PIN, INPUT_PULLDOWN);
  // analog setup not needed?

  attinyInit = true;
}

void attinyUpdate()
{
  if (!attinyInit)
  {
    return;
  }

  // check if button state has changed
  bool newButtonState = attinySs.digitalRead(SS_ATTINY_BTN_PIN);
  if (newButtonState != attinyButtonState)
  {
    attinyButtonState = newButtonState;
    attinySs.digitalWrite(SS_ATTINY_LED_PIN, attinyButtonState);
  }

  // read slider, update LED and brightness if changed
  if (millis() - attinyLastSliderRead > 100)
  {
    attinyLastSliderRead = millis();
    uint16_t newReading = attinySs.analogRead(SS_ATTINY_SLD_PIN);

    int delta = abs(newReading - attinySliderReading);
    if (delta > 10 && delta < 768) // ignore small and large changes
    {
      attinyLastSliderChange = millis();
      attinySliderReading = newReading;

      neokey.pixels.setBrightness(map(attinySliderReading, 0, 1024, 0, 255));
      neokey.pixels.show();

      rotaryRgbLed.setBrightness(map(attinySliderReading, 0, 1024, 0, 100));
      rotaryRgbLed.show();

      attinySs.digitalWrite(SS_ATTINY_LED_PIN, attinySliderReading < 512);

      if (!neokeyToggle)
      {
        for (uint16_t i = 0; i < neokey.pixels.numPixels(); i++)
        {
          uint32_t color = attinySliderReading > (1024 / neokey.pixels.numPixels()) * i
                               ? wheel(map(i, 0, neokey.pixels.numPixels(), 0, 255))
                               : 0x0;
          neokey.pixels.setPixelColor(i, color);
          neokey.pixels.show();
        }
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  delay(3000); // let I2C devices settle
  Serial.println("Starting setup...");

  wifiSetup();
  mDnsSetup();
  otaSetup();
  restSetup();

  neokeySetup();
  rotarySetup();
  attinySetup();
  delay(1000);
  ack1Setup();
}

void loop()
{
  ack1Update();
  attinyUpdate();
  neokeyUpdate();
  rotaryUpdate();

  ArduinoOTA.handle();
  restServer.handleClient();
  checkWifiStatus();
  delay(10);
}