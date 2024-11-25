#pragma once

#include <Adafruit_LEDBackpack.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_NeoKey_1x4.h>
#include <seesaw_neopixel.h>
#include <tuple>

#include "music.h"
#include "task_handler.h"

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
#define ACK1_COUNTER_CMD 0x21
#define ACK1_EEPROM_READ_CMD 0x80
#define ACK1_EEPROM_WRITE_CMD 0x81

#define ALPHANUM_ADDR 0x70
#define ALPHANUM_CHAR_COUNT 4

#define SS_NEOKEY1_ADDR 0x30
#define SS_NEOKEY2_ADDR 0x31
#define SS_NEOKEY_COUNT 4 * 2

#define SS_ROTARY_ADDR 0x36
#define SS_ROTARY_LED_PIN 6
#define SS_ROTARY_BTN_PIN 24
#define SS_ROTARY_LED_COUNT 1

#define SS_NEOSLIDER_ADDR 0x38
#define SS_NEOSLIDER_LED_PIN 14
#define SS_NEOSLIDER_SLD_PIN 18
#define SS_NEOSLIDER_LED_COUNT 4

#define DELAY_MS 10
#define BASE_DELAY 40

bool display = true;

// TODO: clean up more

const std::tuple<uint8_t, uint16_t> AlphaNumLoopPath[] = {
    {0, ALPHANUM_SEG_A}, {1, ALPHANUM_SEG_A}, {2, ALPHANUM_SEG_A}, {3, ALPHANUM_SEG_A}, {3, ALPHANUM_SEG_B}, {3, ALPHANUM_SEG_C}, {3, ALPHANUM_SEG_D}, {2, ALPHANUM_SEG_D}, {1, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_E}, {0, ALPHANUM_SEG_F}};
const int AlphaNumLoopPathLength = sizeof(AlphaNumLoopPath) / sizeof(AlphaNumLoopPath[0]);

const std::tuple<uint8_t, uint16_t> AlphaNumFigure8Path[] = {
    {0, ALPHANUM_SEG_A}, {1, ALPHANUM_SEG_H}, {1, ALPHANUM_SEG_N}, {2, ALPHANUM_SEG_D}, {3, ALPHANUM_SEG_D}, {3, ALPHANUM_SEG_C}, {3, ALPHANUM_SEG_B}, {3, ALPHANUM_SEG_A}, {2, ALPHANUM_SEG_G2}, {2, ALPHANUM_SEG_K}, {1, ALPHANUM_SEG_L}, {0, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_E}, {0, ALPHANUM_SEG_F}};
const int AlphaNumFigure8PathLength = sizeof(AlphaNumFigure8Path) / sizeof(AlphaNumFigure8Path[0]);

const std::tuple<uint8_t, uint16_t> AlphaNumStarPath[] = {{0, ALPHANUM_SEG_J}, {0, ALPHANUM_SEG_K}, {0, ALPHANUM_SEG_G2}, {0, ALPHANUM_SEG_M}, {0, ALPHANUM_SEG_L}, {0, ALPHANUM_SEG_G1}, {0, ALPHANUM_SEG_J}, {0, ALPHANUM_SEG_H}};
const int AlphaNumStarPathLength = sizeof(AlphaNumStarPath) / sizeof(AlphaNumStarPath[0]);

const std::tuple<uint8_t, uint16_t> *AlphaNumPaths[] = {AlphaNumLoopPath, AlphaNumFigure8Path, AlphaNumStarPath};
int AlphaNumPathLengths[] = {AlphaNumLoopPathLength, AlphaNumFigure8PathLength, AlphaNumStarPathLength};
int AlphaNumPathCount = sizeof(AlphaNumPaths) / sizeof(AlphaNumPaths[0]);

class InputTaskHandler : public TaskHandler
{
private:
  enum Mode
  {
    Music,
    Lights,
    Blinky,
  };

  Mode mode = Mode::Music;
  unsigned long lastInput = 0;

  bool ack1Init = false;
  bool ack1Display = false;
  const char *ack1BootMessage = "Hello world!";
  const char *ack1Message = "Dad loves Stella and Beau!";
  const char *BaseToneNames[8] = {"C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5"};
  const int BaseToneFrequencies[8] = {261, 293, 329, 349, 392, 440, 493, 523}; // C4, D4, E4, F4, G4, A4, B4, C5

  bool alphaNumInit = false;
  Adafruit_AlphaNum4 alphaNum = Adafruit_AlphaNum4();
  bool alphaNumUpdated = false;
  char alphaNumBuffer[ALPHANUM_CHAR_COUNT] = {' ', ' ', ' ', ' '};
  int alphaNumBlinkyDelay;
  int alphaNumBlinkyIndex;
  int alphaNumPathIndex;

  bool neoKeyInit = false;
  Adafruit_NeoKey_1x4 neokeyArray[2] = {Adafruit_NeoKey_1x4(SS_NEOKEY1_ADDR), Adafruit_NeoKey_1x4(SS_NEOKEY2_ADDR)};
  Adafruit_MultiNeoKey1x4 neoKey = Adafruit_MultiNeoKey1x4((Adafruit_NeoKey_1x4 *)neokeyArray, 2, 1);

  unsigned long neoKeyLastPressMillis = 0;
  unsigned long neoKeyLastReleaseMillis = 0;
  // light/blinky mode state
  int neoKeyPressedIndex = -1;
  bool neoKeyPixelState[SS_NEOKEY_COUNT];
  int neoKeyBlinkyDelay[SS_NEOKEY_COUNT]; // blinky state

  bool neoSliderInit = false;
  Adafruit_seesaw neoSliderSs;
  seesaw_NeoPixel neoSliderPixels = seesaw_NeoPixel(SS_NEOSLIDER_LED_COUNT, SS_NEOSLIDER_LED_PIN, NEO_GRB + NEO_KHZ800);
  uint16_t neoSliderReading = 0;
  uint16_t neoSliderLastReading = 0;
  unsigned long neoSliderLastChangeMillis = 0;
  // light/blinky mode state
  bool neoSliderPixelState[SS_NEOSLIDER_LED_COUNT];
  int neoSliderBlinkyDelay[SS_NEOSLIDER_LED_COUNT];

  bool rotaryInit = false;
  Adafruit_seesaw rotarySs;
  seesaw_NeoPixel rotaryNeoPixel = seesaw_NeoPixel(SS_ROTARY_LED_COUNT, SS_ROTARY_LED_PIN, NEO_GRB + NEO_KHZ800);
  int32_t rotaryEncPos;
  uint8_t rotaryWheelPos;
  bool rotaryIsPressed = false;
  unsigned long rotaryLastPressMillis = 0;
  unsigned long rotaryLastChangeMillis = 0;
  // light/blinky mode state
  bool rotaryLedState;
  int rotaryBlinkyDelay;

public:
  InputTaskHandler() {}

  bool createTask() override;

private:
  void task(void *parameters) override;
  void update();
  uint32_t wheel(uint8_t wheelPos);
  void changeMode(int mode = -1);

  void ack1Setup();
  void ack1Update();
  bool ack1Command(uint8_t cmd, const uint8_t *data = NULL, size_t len = 0);
  bool ack1Response(uint8_t *data, size_t len);
  void ack1Clear();
  void ack1Tone(uint16_t freq);
  void ack1Wake();

  void alphaNumSetup();
  void alphaNumUpdate();

  void neoKeySetup();
  void neoKeyUpdate();
  NeoKey1x4Callback neoKeyCallback(keyEvent evt);
  static NeoKey1x4Callback neoKeyCallbackStatic(keyEvent evt);

  void neoSliderSetup();
  void neoSliderUpdate();

  void rotarySetup();
  void rotaryUpdate();
};

bool InputTaskHandler::createTask()
{
  log_d("Starting input setup");

  if (_taskHandle != NULL)
  {
    log_w("Task already started");
    return false;
  }

  alphaNumSetup();
  neoKeySetup();
  neoSliderSetup();
  rotarySetup();
  delay(1000);
  ack1Setup(); // slow to initialize

  xTaskCreatePinnedToCore(taskWrapper, "InputTask", 4096, this, 2, &_taskHandle, 1);

  log_d("Input setup complete");
  return true;
}

void InputTaskHandler::task(void *parameters)
{
  log_d("Starting InputTask");

  while (1)
  {
    ack1Update();
    alphaNumUpdate();
    neoKeyUpdate();
    neoSliderUpdate();
    rotaryUpdate();
    update();

    delay(DELAY_MS);
  }
}

void InputTaskHandler::update()
{
  // just goes to "screensaver" mode after 30 seconds of inactivity
  if (!ack1Init || !neoKeyInit || !neoSliderInit || !rotaryInit || mode == Mode::Blinky)
    return;

  unsigned long now = millis();
  unsigned long neoKeyLastPressElapsed = now - neoKeyLastPressMillis;
  unsigned long neoKeyLastReleaseElapsed = now - neoKeyLastReleaseMillis;
  unsigned long neoSliderLastChangeElapsed = now - neoSliderLastChangeMillis;
  unsigned long rotaryLastChangeElapsed = now - rotaryLastChangeMillis;
  unsigned long rotaryLastPressElapsed = now - rotaryLastPressMillis;

  unsigned long shortestElapsed = min(neoKeyLastPressElapsed, neoKeyLastReleaseElapsed);
  shortestElapsed = min(shortestElapsed, neoSliderLastChangeElapsed);
  shortestElapsed = min(shortestElapsed, rotaryLastChangeElapsed);
  shortestElapsed = min(shortestElapsed, rotaryLastPressElapsed);

  if (shortestElapsed > 30 * 1000)
  {
    changeMode(Mode::Blinky);
  }
}

uint32_t InputTaskHandler::wheel(uint8_t wheelPos)
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

void InputTaskHandler::changeMode(int mode)
{
  if (!ack1Init || !neoKeyInit || !neoSliderInit || !rotaryInit)
  {
    log_e("Not all components initialized");
    return;
  }

  if (mode > 0)
  {
    mode = mode % 3;
  }
  else
  {
    mode = static_cast<Mode>((static_cast<int>(mode) + 1) % 3);
  }

  if (mode == Music)
  {
    log_i("Switching to Music mode");

    alphaNum.clear();

    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      neoKeyPixelState[i] = false;
      neoKey.setPixelColor(i, 0x000000);
    }

    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      neoSliderPixelState[i] = false;
      neoSliderPixels.setPixelColor(i, 0x000000);
    }

    rotaryNeoPixel.setPixelColor(0, 0x000000);
  }
  else if (mode == Lights)
  {
    log_i("Switching to Lights mode");

    alphaNum.clear();

    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      neoKeyPixelState[i] = true;
      neoKey.setPixelColor(i, wheel(map(i, 0, SS_NEOKEY_COUNT, 0, 255)));
    }

    rotaryNeoPixel.setPixelColor(0, wheel(rotaryWheelPos));
  }
  else if (mode == Blinky)
  {
    log_i("Switching to Blinky mode");

    alphaNum.clear();

    alphaNumPathIndex = random(0, AlphaNumPathCount);
    alphaNumBlinkyDelay = 0;
    alphaNumBlinkyIndex = random(0, AlphaNumPathLengths[alphaNumPathIndex]);

    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      neoKeyPixelState[i] = true;
      neoKeyBlinkyDelay[i] = random(BASE_DELAY * 5, BASE_DELAY * 10);
      neoKey.setPixelColor(i, wheel(rotaryWheelPos + random(0, 16)));
    }

    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      neoKeyPixelState[i] = true;
      neoKeyBlinkyDelay[i] = random(BASE_DELAY * 5, BASE_DELAY * 10);
      neoKey.setPixelColor(i, wheel(rotaryWheelPos + random(0, 32)));
    }

    rotaryLedState = true;
    rotaryBlinkyDelay = random(BASE_DELAY * 5, BASE_DELAY * 10);
    rotaryNeoPixel.setPixelColor(0, wheel(rotaryWheelPos + random(0, 32)));
  }

  alphaNum.writeDisplay();
  neoKey.show();
  neoSliderPixels.show();
  rotaryNeoPixel.show();
}

void InputTaskHandler::ack1Setup()
{
  if (ack1Init)
  {
    log_w("ACK1 already initialized");
    return;
  }

  if (!Wire.begin())
  {
    log_e("Couldn't setup I2C");
    return;
  }

  ack1Init = true;
  if (!ack1Command(ACK1_STATUS_CMD))
  {
    ack1Init = false;
    log_e("ACK1 not found");
    return;
  }

  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)ack1BootMessage, strlen(ack1BootMessage));
  log_d("ACK1 status: %d", ack1Command(ACK1_STATUS_CMD));
  ack1Display = true;
}

void InputTaskHandler::ack1Update()
{
  if (!ack1Init)
  {
    return;
  }

  if (display && !ack1Display)
  {
    ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)ack1Message, strlen(ack1Message));
    ack1Display = true;
  }
  else if (!display && ack1Display)
  {
    ack1Clear();
    ack1Display = false;
  }
}

bool InputTaskHandler::ack1Command(uint8_t cmd, const uint8_t *data, size_t len)
{
  if (!ack1Init)
  {
    log_e("ACK1 not initialized!");
    return false;
  }

  bool success = true;

  Wire.beginTransmission(ACK1_ADDR);

  size_t responseSize = 0;
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
    responseSize = 1;
    break;
  default:
    log_d("Sending default command");
    Wire.write(ACK1_STATUS_CMD);
    responseSize = 1;
    break;
  }

  bool endTransmissionSuccess = Wire.endTransmission();
  if (endTransmissionSuccess != 0)
  {
    log_e("Failed to end transmission, error: %d", endTransmissionSuccess);
    return false;
  }

  // read response, otherwise check status
  uint8_t responseBytes[32];
  if (responseSize > 0)
  {
    if (!ack1Response(responseBytes, responseSize))
    {
      log_e("Failed to read command response");
      return false;
    }

    for (size_t i = 0; i < responseSize; i++)
    {
      log_i("Response byte %d: %02x", i, responseBytes[i]);
    }
  }
  else
  {
    // TODO: not sure if I need status command after all commands??
    Wire.beginTransmission(ACK1_ADDR);
    Wire.write(ACK1_STATUS_CMD);
    Wire.endTransmission();
    if (!ack1Response(responseBytes, 1))
    {
      log_e("Failed to read command status");
      return false;
    }

    success &= responseBytes[0] == 0x00 || responseBytes[0] == 0xFF; // 0xFF when LEDs are off
  }

  return success;
}

bool InputTaskHandler::ack1Response(uint8_t *data, size_t len)
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

  for (size_t i = 0; i < recv; i++)
  {
    data[i] = Wire.read();
    log_d("Read response byte %d: %02x", i, data[i]);
  }

  return true;
}

void InputTaskHandler::ack1Clear()
{
  // one clear only stops scroll, second clear clears display (not sure why?)
  ack1Command(ACK1_LEDCLR_CMD);
  ack1Command(ACK1_LEDCLR_CMD);
}

void InputTaskHandler::ack1Tone(uint16_t freq)
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

void InputTaskHandler::ack1Wake()
{
  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)ack1Message, strlen(ack1Message));
}

void InputTaskHandler::alphaNumSetup()
{
  if (alphaNumInit)
  {
    log_w("AlphaNum already initialized");
    return;
  }

  if (!alphaNum.begin(ALPHANUM_ADDR))
  {
    log_e("Couldn't find AlphaNum seesaw on default address");
    return;
  }

  alphaNum.writeDigitRaw(3, 0x0);
  alphaNum.writeDigitRaw(0, 0xFFFF);
  alphaNum.writeDisplay();
  delay(200);
  alphaNum.writeDigitRaw(0, 0x0);
  alphaNum.writeDigitRaw(1, 0xFFFF);
  alphaNum.writeDisplay();
  delay(200);
  alphaNum.writeDigitRaw(1, 0x0);
  alphaNum.writeDigitRaw(2, 0xFFFF);
  alphaNum.writeDisplay();
  delay(200);
  alphaNum.writeDigitRaw(2, 0x0);
  alphaNum.writeDigitRaw(3, 0xFFFF);
  alphaNum.writeDisplay();
  delay(200);

  alphaNum.clear();
  alphaNum.writeDisplay();

  alphaNumInit = true;
  log_i("AlphaNum initialized");
}

void InputTaskHandler::alphaNumUpdate()
{
  if (!alphaNumInit)
  {
    return;
  }

  if (!display)
  {
    alphaNum.clear();
    alphaNum.writeDisplay();
    return;
  }

  if (mode == Mode::Music)
  {
    if (neoKeyPressedIndex != -1)
    {
      if (!alphaNumUpdated)
      {
        // shift tone name from the right into buffer
        const char *toneName = BaseToneNames[neoKeyPressedIndex % 8];
        alphaNumBuffer[0] = alphaNumBuffer[1];
        alphaNumBuffer[1] = alphaNumBuffer[2];
        alphaNumBuffer[2] = alphaNumBuffer[3];
        alphaNumBuffer[3] = toneName[0]; // ignore the note letter for now
        alphaNum.writeDigitAscii(0, alphaNumBuffer[0]);
        alphaNum.writeDigitAscii(1, alphaNumBuffer[1]);
        alphaNum.writeDigitAscii(2, alphaNumBuffer[2]);
        alphaNum.writeDigitAscii(3, alphaNumBuffer[3]);
        alphaNum.writeDisplay();
        alphaNumUpdated = true;
      }
    }
    else if (neoKeyPressedIndex == -1)
    {
      if (alphaNumUpdated)
      {
        alphaNumUpdated = false;
      }
    }
  }
  else if (mode == Mode::Lights)
  {
    // alphaNum.print(neoSliderReading);
    // alphaNum.writeDisplay();
  }
  else if (mode == Mode::Blinky)
  {
    if (--alphaNumBlinkyDelay > 0)
      return;

    alphaNum.clear();
    alphaNumBuffer[3] = alphaNumBuffer[2] = alphaNumBuffer[1] = alphaNumBuffer[0] = 0;
    for (int i = 0; i < 3; i++)
    {
      auto index = (alphaNumBlinkyIndex + i) % AlphaNumPathLengths[alphaNumPathIndex];
      const auto [digit, bitmask] = AlphaNumPaths[alphaNumPathIndex][index];
      alphaNumBuffer[digit] |= bitmask;
    }
    alphaNum.writeDigitRaw(0, alphaNumBuffer[0]);
    alphaNum.writeDigitRaw(1, alphaNumBuffer[1]);
    alphaNum.writeDigitRaw(2, alphaNumBuffer[2]);
    alphaNum.writeDigitRaw(3, alphaNumBuffer[3]);
    alphaNum.writeDisplay();

    alphaNumBlinkyIndex = (alphaNumBlinkyIndex + 1) % AlphaNumPathLengths[alphaNumPathIndex];
    alphaNumBlinkyDelay = 6;
  }
}

void InputTaskHandler::neoKeySetup()
{
  if (neoKeyInit)
  {
    log_w("NeoKey already initialized");
    return;
  }

  if (!neoKey.begin())
  {
    log_e("Couldn't find NeoKey seesaw(s) on default address(es)");
    return;
  }

  for (uint16_t i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    neoKey.setPixelColor(i, 0x808080);
    neoKey.show();
    delay(50);
  }
  for (uint16_t i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    neoKey.setPixelColor(i, 0x000000);
    neoKey.show();
    delay(50);
  }

  for (int i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    neoKey.registerCallback(i, InputTaskHandler::neoKeyCallbackStatic);
  }

  log_i("NeoKey initialized");
  neoKeyInit = true;
}

void InputTaskHandler::neoKeyUpdate()
{
  if (!neoKeyInit)
  {
    return;
  }

  neoKey.read();

  if (!display)
  {
    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      neoKey.setPixelColor(i, 0x000000);
    }
    neoKey.show();
    return;
  }

  // debounce after key press
  if (millis() - neoKeyLastPressMillis < 100 || millis() - neoKeyLastReleaseMillis < 100)
  {
    return;
  }

  if (mode == Mode::Lights)
  {
    if (neoSliderReading != neoSliderLastReading)
    {
      // int onCount = map(neoSliderReading, 0, 1023, 0, SS_NEOKEY_COUNT);
      // for (int i = 0; i < SS_NEOKEY_COUNT; i++)
      // {
      //   neoKey.setPixelColor(i, i < onCount ? wheel(map(i, 0, SS_NEOKEY_COUNT, 0, 255)) : 0);
      // }
      // neoKey.show();
      auto keyIndex = map(neoSliderReading, 0, 1023, 0, SS_NEOKEY_COUNT);
      auto color = neoSliderReading > neoSliderLastReading ? wheel(map(keyIndex, 0, SS_NEOKEY_COUNT, 0, 255)) : 0;
      neoKey.setPixelColor(keyIndex, color);
      neoKey.show();
    }
  }
  else if (mode == Mode::Blinky)
  {
    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      if (--neoKeyBlinkyDelay[i] > 0)
        continue;

      neoKeyPixelState[i] = !neoKeyPixelState[i];
      neoKeyBlinkyDelay[i] = random(BASE_DELAY, BASE_DELAY * 2);

      uint32_t color = neoKeyPixelState[i] ? wheel(rotaryWheelPos + random(0, 16)) : 0;
      neoKey.setPixelColor(i, color);
    }
    neoKey.show();
  }
}

// I hate this
extern InputTaskHandler inputTask;
NeoKey1x4Callback InputTaskHandler::neoKeyCallbackStatic(keyEvent evt)
{
  inputTask.neoKeyCallback(evt);
}

NeoKey1x4Callback InputTaskHandler::neoKeyCallback(keyEvent evt)
{
  log_d("NeoKey event: %d %s", evt.bit.NUM, evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING ? "rising" : "falling");

  uint8_t key = evt.bit.NUM;
  uint32_t color = 0x0; // default event to turn LED off
  uint16_t tone = 0x0;  // default event to turn tone off
  neoKeyPressedIndex = -1;

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    neoKeyPressedIndex = key;
    neoKeyLastPressMillis = millis();
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    neoKeyLastReleaseMillis = millis();
  }

  if (mode == Mode::Music)
  {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    {
      color = wheel(map(key, 0, SS_NEOKEY_COUNT, 0, 255)); // TODO also eventually show note name on display
      tone = BaseToneFrequencies[key % 8];
    }
  }
  else if (mode == Mode::Lights)
  {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    {
      neoKeyPixelState[key] = !neoKeyPixelState[key];
    }

    if (neoKeyPixelState[key])
    {
      color = wheel(map(key, 0, SS_NEOKEY_COUNT, 0, 255));
    }
  }
  else if (mode == Mode::Blinky)
  {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    {
      neoKeyBlinkyDelay[key] = 1000;
    }
  }

  if (tone > 0)
  {
    int pitchModifier = map(neoSliderReading, 0, 1023, -200, 200);
    tone += pitchModifier;
  }

  neoKey.setPixelColor(key, color);
  neoKey.show();
  ack1Tone(tone);
}

void InputTaskHandler::neoSliderSetup()
{
  if (neoSliderInit)
  {
    log_w("NeoSlider already initialized");
    return;
  }

  if (!neoSliderSs.begin(SS_NEOSLIDER_ADDR) || !neoSliderPixels.begin(SS_NEOSLIDER_ADDR))
  {
    log_e("Couldn't find NeoSlider seesaw on default address");
    return;
  }

  uint16_t version = neoSliderSs.getVersion() >> 16;
  if (version != 5295)
  {
    log_e("Wrong seesaw firmware for slider loaded: %d", version);
    return;
  }

  neoSliderPixels.setBrightness(255);
  neoSliderPixels.show();
  neoSliderInit = true;

  log_i("NeoSlider initialized");
}

void InputTaskHandler::neoSliderUpdate()
{
  if (!neoSliderInit)
  {
    return;
  }

  if (!display)
  {
    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      neoSliderPixels.setPixelColor(i, 0x000000);
    }
    neoSliderPixels.show();
    return;
  }

  neoSliderLastReading = neoSliderReading;
  neoSliderReading = 1023 - neoSliderSs.analogRead(SS_NEOSLIDER_SLD_PIN); // invert slider reading

  if (neoSliderReading != neoSliderLastReading)
  {
    neoSliderLastChangeMillis = millis(); // TODO: might need debounce?
  }

  if (mode == Mode::Music)
  {
    if (neoKeyPressedIndex >= 0)
    {
      uint32_t color = wheel(map(neoKeyPressedIndex, 0, SS_NEOKEY_COUNT, 0, 255));
      for (uint8_t i = 0; i < neoSliderPixels.numPixels(); i++)
      {
        neoSliderPixels.setPixelColor(i, color);
      }
      neoSliderPixels.show();
    }
  }
  else if (mode == Mode::Lights)
  {
    if (neoSliderReading != neoSliderLastReading)
    {
      int onCount = map(neoSliderReading, 0, 1023, 0, SS_NEOSLIDER_LED_COUNT);
      for (uint8_t i = 0; i < neoSliderPixels.numPixels(); i++)
      {
        neoSliderPixels.setPixelColor(neoSliderPixels.numPixels() - i - 1, i < onCount ? wheel(map(i, 0, neoSliderPixels.numPixels(), 0, 255)) : 0);
      }
      neoSliderPixels.show();
    }
  }
  else if (mode == Mode::Blinky)
  {
    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      if (--neoSliderBlinkyDelay[i] > 0)
        continue;

      neoSliderPixelState[i] = !neoSliderPixelState[i];
      neoSliderBlinkyDelay[i] = random(BASE_DELAY, BASE_DELAY * 2);

      uint32_t color = neoSliderPixelState[i] ? wheel(rotaryWheelPos + random(0, 16)) : 0;
      neoSliderPixels.setPixelColor(i, color);
    }
    neoSliderPixels.show();
  }
}

void InputTaskHandler::rotarySetup()
{
  if (rotaryInit)
  {
    log_w("Rotary already initialized");
    return;
  }

  if (!rotarySs.begin(SS_ROTARY_ADDR) || !rotaryNeoPixel.begin(SS_ROTARY_ADDR))
  {
    log_e("Couldn't find rotary seesaw on default address");
    return;
  }

  uint32_t version = ((rotarySs.getVersion() >> 16) & 0xFFFF);
  if (version != 4991)
  {
    log_e("Wrong seesaw firmware for rotary loaded: %d", version);
    return;
  }

  rotaryNeoPixel.setBrightness(20);
  rotaryNeoPixel.show();
  rotarySs.pinMode(SS_ROTARY_BTN_PIN, INPUT_PULLUP);
  rotaryEncPos = rotarySs.getEncoderPosition();

  // Serial.println("Turning on interrupts");
  // delay(10);
  // rotarySs.setGPIOInterrupts((uint32_t)1 << SS_ROTARY_ENC_PIN, 1);
  // rotarySs.enableEncoderInterrupt();

  rotaryInit = true;

  log_i("Rotary initialized");
}

void InputTaskHandler::rotaryUpdate()
{
  if (!rotaryInit)
  {
    return;
  }

  if (!display)
  {
    rotaryNeoPixel.setPixelColor(0, 0x000000);
    rotaryNeoPixel.show();
    return;
  }

  if (!rotarySs.digitalRead(SS_ROTARY_BTN_PIN))
  {
    if (!rotaryIsPressed && millis() - rotaryLastPressMillis > 100)
    {
      rotaryIsPressed = true;
      rotaryLastPressMillis = millis();
      changeMode();
      ack1Tone(1000);
      delay(100);
      ack1Tone(0);
    }
  }
  else
  {
    rotaryIsPressed = false;
  }

  int32_t newEncPos = rotarySs.getEncoderPosition();
  if (rotaryEncPos != newEncPos)
  {
    // log_d(newEncPos);
    rotaryWheelPos = (newEncPos * 5) & 0xFF;
    rotaryNeoPixel.setPixelColor(0, wheel(rotaryWheelPos)); // 3 rotations for full cycle with 24 step encoder?
    rotaryNeoPixel.show();
    rotaryEncPos = newEncPos;
    rotaryLastChangeMillis = millis();
  }

  if (mode == Mode::Music)
  {
    if (neoKeyPressedIndex >= 0)
    {
      uint32_t color = wheel(map(neoKeyPressedIndex, 0, SS_NEOKEY_COUNT, 0, 255));
      rotaryNeoPixel.setPixelColor(0, color);
      rotaryNeoPixel.show();
    }
  }
  else if (mode == Mode::Lights)
  {
    if (neoKeyPressedIndex >= 0)
    {
      uint32_t color = wheel(map(neoKeyPressedIndex, 0, SS_NEOKEY_COUNT, 0, 255));
      rotaryNeoPixel.setPixelColor(0, color);
      rotaryNeoPixel.show();
    }
  }
  else if (mode == Mode::Blinky)
  {
    if (!rotaryIsPressed && --rotaryBlinkyDelay <= 0)
    {
      rotaryLedState = !rotaryLedState;
      rotaryBlinkyDelay = random(BASE_DELAY, BASE_DELAY * 2);

      uint32_t color = rotaryLedState ? wheel(rotaryWheelPos + random(0, 16)) : 0;
      rotaryNeoPixel.setPixelColor(0, color);
      rotaryNeoPixel.show();
    }
  }
}
