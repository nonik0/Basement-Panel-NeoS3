#pragma once

#include <Adafruit_LEDBackpack.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_NeoKey_1x4.h>
#include <seesaw_neopixel.h>
#include <tuple>

#include "display_task_handler.h"
#include "music.h"

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

class InputTaskHandler : public DisplayTaskHandler
{
private:
  enum Mode
  {
    Music,
    Lights,
    Blinky,
  };

  bool _display = true;
  Mode _mode = Mode::Music;
  unsigned long _lastInput = 0;

  bool _ack1Init = false;
  bool _ack1Display = false;
  const char *_ack1BootMessage = "Hello world!";
  const char *_ack1Message = "Dad loves Stella and Beau!";
  const char *_BaseToneNames[8] = {"C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5"};
  const int _BaseToneFrequencies[8] = {261, 293, 329, 349, 392, 440, 493, 523}; // C4, D4, E4, F4, G4, A4, B4, C5

  bool _alphaNumInit = false;
  Adafruit_AlphaNum4 _alphaNum = Adafruit_AlphaNum4();
  bool _alphaNumUpdated = false;
  char _alphaNumBuffer[ALPHANUM_CHAR_COUNT] = {' ', ' ', ' ', ' '};
  int _alphaNumBlinkyDelay;
  int _alphaNumBlinkyIndex;
  int _alphaNumPathIndex;

  bool _neoKeyInit = false;
  Adafruit_NeoKey_1x4 _neokeyArray[2] = {Adafruit_NeoKey_1x4(SS_NEOKEY1_ADDR), Adafruit_NeoKey_1x4(SS_NEOKEY2_ADDR)};
  Adafruit_MultiNeoKey1x4 _neoKey = Adafruit_MultiNeoKey1x4((Adafruit_NeoKey_1x4 *)_neokeyArray, 2, 1);
  int _neoKeyJustPressedIndex = -1;
  unsigned long _neoKeyLastPressMillis = 0;
  unsigned long _neoKeyLastReleaseMillis = 0;
  // light/blinky mode state
  int _neoKeyPressedIndex = -1;
  bool _neoKeyPixelState[SS_NEOKEY_COUNT];
  int _neoKeyBlinkyDelay[SS_NEOKEY_COUNT]; // blinky state

  bool _neoSliderInit = false;
  Adafruit_seesaw _neoSliderSs;
  seesaw_NeoPixel _neoSliderPixels = seesaw_NeoPixel(SS_NEOSLIDER_LED_COUNT, SS_NEOSLIDER_LED_PIN, NEO_GRB + NEO_KHZ800);
  uint16_t _neoSliderReading = 0;
  uint16_t _neoSliderLastReading = 0;
  bool _neoSliderReaderJustChanged = false;
  unsigned long _neoSliderLastChangeMillis = 0;
  // light/blinky mode state
  bool _neoSliderPixelState[SS_NEOSLIDER_LED_COUNT];
  int _neoSliderBlinkyDelay[SS_NEOSLIDER_LED_COUNT];

  bool _rotaryInit = false;
  Adafruit_seesaw _rotarySs;
  seesaw_NeoPixel _rotaryNeoPixel = seesaw_NeoPixel(SS_ROTARY_LED_COUNT, SS_ROTARY_LED_PIN, NEO_GRB + NEO_KHZ800);
  bool _rotaryJustPressed = false;
  bool _rotaryJustChanged = false;
  int32_t _rotaryEncPos;
  uint8_t _rotaryWheelPos;
  bool _rotaryIsPressed = false;
  unsigned long _rotaryLastPressMillis = 0;
  unsigned long _rotaryLastChangeMillis = 0;
  // light/blinky mode state
  bool _rotaryLedState;
  int _rotaryBlinkyDelay;

public:
  InputTaskHandler() {}

  bool createTask() override;
  void setDisplay(bool display);

private:
  void task(void *parameters) override;
  void update();
  uint32_t wheel(uint8_t wheelPos);
  void changeMode(int mode = -1);

  void ack1Setup();
  bool ack1Command(uint8_t cmd, const uint8_t *data = NULL, size_t len = 0);
  bool ack1Response(uint8_t *data, size_t len);
  void ack1Clear();
  void ack1Tone(uint16_t freq);
  void ack1Wake();

  void alphaNumSetup();
  void alphaNumUpdate();
  void alphaNumClear();

  void neoKeySetup();
  void neoKeyUpdate();
  void neoKeyClear();
  NeoKey1x4Callback neoKeyCallback(keyEvent evt);
  static NeoKey1x4Callback neoKeyCallbackStatic(keyEvent evt);

  void neoSliderSetup();
  void neoSliderRead();
  void neoSliderClear();

  void rotarySetup();
  void rotaryUpdate();
  void rotaryClear();
};

bool InputTaskHandler::createTask()
{
  log_d("Starting input setup");

  if (_taskHandle != NULL)
  {
    log_w("Task already started");
    return false;
  }

  // let I2C devices settle
  while (millis() < 5000);

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

void InputTaskHandler::setDisplay(bool displayState)
{
  if (displayState)
  {
    ack1Wake();
  }
  else
  {
    ack1Clear();
    alphaNumClear();
    neoKeyClear();
    neoSliderClear();
    rotaryClear();
  }
}

void InputTaskHandler::task(void *parameters)
{
  log_d("Starting InputTask");

  while (1)
  {
    if (!_display)
    {
      return;
    }

    alphaNumUpdate();
    neoKeyUpdate();
    neoSliderRead();
    rotaryUpdate();

    update();

    delay(DELAY_MS);
  }
}

void InputTaskHandler::update()
{
  if (_mode == Mode::Music)
  {
    if (_neoKeyPressedIndex >= 0)
    {
      uint32_t color = wheel(map(_neoKeyPressedIndex, 0, SS_NEOKEY_COUNT, 0, 255));
      for (uint8_t i = 0; i < _neoSliderPixels.numPixels(); i++)
      {
        _neoSliderPixels.setPixelColor(i, color);
      }
      _neoSliderPixels.show();
    }
  }
  else if (_mode == Mode::Lights)
  {
    if (_neoSliderReading != _neoSliderLastReading)
    {
      int onCount = map(_neoSliderReading, 0, 1023, 0, SS_NEOSLIDER_LED_COUNT);
      for (uint8_t i = 0; i < _neoSliderPixels.numPixels(); i++)
      {
        _neoSliderPixels.setPixelColor(_neoSliderPixels.numPixels() - i - 1, i < onCount ? wheel(map(i, 0, _neoSliderPixels.numPixels(), 0, 255)) : 0);
      }
      _neoSliderPixels.show();
    }
  }
  else if (_mode == Mode::Blinky)
  {
    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      if (--_neoSliderBlinkyDelay[i] > 0)
        continue;

      _neoSliderPixelState[i] = !_neoSliderPixelState[i];
      _neoSliderBlinkyDelay[i] = random(BASE_DELAY, BASE_DELAY * 2);

      uint32_t color = _neoSliderPixelState[i] ? wheel(_rotaryWheelPos + random(0, 16)) : 0;
      _neoSliderPixels.setPixelColor(i, color);
    }
    _neoSliderPixels.show();
  }

  // // just goes to "screensaver" mode after 30 seconds of inactivity
  // if (!ack1Init || !neoKeyInit || !neoSliderInit || !rotaryInit || mode == Mode::Blinky)
  //   return;

  // unsigned long now = millis();
  // unsigned long neoKeyLastPressElapsed = now - neoKeyLastPressMillis;
  // unsigned long neoKeyLastReleaseElapsed = now - neoKeyLastReleaseMillis;
  // unsigned long neoSliderLastChangeElapsed = now - neoSliderLastChangeMillis;
  // unsigned long rotaryLastChangeElapsed = now - rotaryLastChangeMillis;
  // unsigned long rotaryLastPressElapsed = now - rotaryLastPressMillis;

  // unsigned long shortestElapsed = min(neoKeyLastPressElapsed, neoKeyLastReleaseElapsed);
  // shortestElapsed = min(shortestElapsed, neoSliderLastChangeElapsed);
  // shortestElapsed = min(shortestElapsed, rotaryLastChangeElapsed);
  // shortestElapsed = min(shortestElapsed, rotaryLastPressElapsed);

  // if (shortestElapsed > 10 * 1000)
  // {
  //   changeMode(Mode::Blinky);
  // }
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
  if (!_ack1Init || !_neoKeyInit || !_neoSliderInit || !_rotaryInit)
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

    _alphaNum.clear();

    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      _neoKeyPixelState[i] = false;
      _neoKey.setPixelColor(i, 0x000000);
    }

    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      _neoSliderPixelState[i] = false;
      _neoSliderPixels.setPixelColor(i, 0x000000);
    }

    _rotaryNeoPixel.setPixelColor(0, 0x000000);
  }
  else if (mode == Lights)
  {
    log_i("Switching to Lights mode");

    _alphaNum.clear();

    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      _neoKeyPixelState[i] = true;
      _neoKey.setPixelColor(i, wheel(map(i, 0, SS_NEOKEY_COUNT, 0, 255)));
    }

    _rotaryNeoPixel.setPixelColor(0, wheel(_rotaryWheelPos));
  }
  else if (mode == Blinky)
  {
    log_i("Switching to Blinky mode");

    _alphaNum.clear();

    _alphaNumPathIndex = random(0, AlphaNumPathCount);
    _alphaNumBlinkyDelay = 0;
    _alphaNumBlinkyIndex = random(0, AlphaNumPathLengths[_alphaNumPathIndex]);

    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      _neoKeyPixelState[i] = true;
      _neoKeyBlinkyDelay[i] = random(BASE_DELAY * 5, BASE_DELAY * 10);
      _neoKey.setPixelColor(i, wheel(_rotaryWheelPos + random(0, 16)));
    }

    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      _neoKeyPixelState[i] = true;
      _neoKeyBlinkyDelay[i] = random(BASE_DELAY * 5, BASE_DELAY * 10);
      _neoKey.setPixelColor(i, wheel(_rotaryWheelPos + random(0, 32)));
    }

    _rotaryLedState = true;
    _rotaryBlinkyDelay = random(BASE_DELAY * 5, BASE_DELAY * 10);
    _rotaryNeoPixel.setPixelColor(0, wheel(_rotaryWheelPos + random(0, 32)));
  }

  _alphaNum.writeDisplay();
  _neoKey.show();
  _neoSliderPixels.show();
  _rotaryNeoPixel.show();
}

void InputTaskHandler::ack1Setup()
{
  if (_ack1Init)
  {
    log_w("ACK1 already initialized");
    return;
  }

  if (!Wire.begin())
  {
    log_e("Couldn't setup I2C");
    return;
  }

  _ack1Init = true;
  if (!ack1Command(ACK1_STATUS_CMD))
  {
    _ack1Init = false;
    log_e("ACK1 not found");
    return;
  }

  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)_ack1BootMessage, strlen(_ack1BootMessage));
  log_d("ACK1 status: %d", ack1Command(ACK1_STATUS_CMD));
  _ack1Display = true;
}

bool InputTaskHandler::ack1Command(uint8_t cmd, const uint8_t *data, size_t len)
{
  if (!_ack1Init)
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
    _ack1Display = false;
    break;
  case ACK1_LEDSCROLL_CMD:
    log_d("Sending LEDSCROLL command");
    Wire.write(ACK1_LEDSCROLL_CMD);
    Wire.write(static_cast<uint8_t>(len));
    Wire.write(data, len);
    _ack1Display = true;
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
  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)_ack1Message, strlen(_ack1Message));
}

void InputTaskHandler::alphaNumSetup()
{
  if (_alphaNumInit)
  {
    log_w("AlphaNum already initialized");
    return;
  }

  if (!_alphaNum.begin(ALPHANUM_ADDR))
  {
    log_e("Couldn't find AlphaNum seesaw on default address");
    return;
  }

  _alphaNum.writeDigitRaw(3, 0x0);
  _alphaNum.writeDigitRaw(0, 0xFFFF);
  _alphaNum.writeDisplay();
  delay(200);
  _alphaNum.writeDigitRaw(0, 0x0);
  _alphaNum.writeDigitRaw(1, 0xFFFF);
  _alphaNum.writeDisplay();
  delay(200);
  _alphaNum.writeDigitRaw(1, 0x0);
  _alphaNum.writeDigitRaw(2, 0xFFFF);
  _alphaNum.writeDisplay();
  delay(200);
  _alphaNum.writeDigitRaw(2, 0x0);
  _alphaNum.writeDigitRaw(3, 0xFFFF);
  _alphaNum.writeDisplay();
  delay(200);

  _alphaNum.clear();
  _alphaNum.writeDisplay();

  _alphaNumInit = true;
  log_i("AlphaNum initialized");
}

void InputTaskHandler::alphaNumUpdate()
{
  if (!_alphaNumInit)
  {
    return;
  }

  if (_mode == Mode::Music)
  {
    if (_neoKeyPressedIndex != -1)
    {
      if (!_alphaNumUpdated)
      {
        // shift tone name from the right into buffer
        const char *toneName = _BaseToneNames[_neoKeyPressedIndex % 8];
        _alphaNumBuffer[0] = _alphaNumBuffer[1];
        _alphaNumBuffer[1] = _alphaNumBuffer[2];
        _alphaNumBuffer[2] = _alphaNumBuffer[3];
        _alphaNumBuffer[3] = toneName[0]; // ignore the note letter for now
        _alphaNum.writeDigitAscii(0, _alphaNumBuffer[0]);
        _alphaNum.writeDigitAscii(1, _alphaNumBuffer[1]);
        _alphaNum.writeDigitAscii(2, _alphaNumBuffer[2]);
        _alphaNum.writeDigitAscii(3, _alphaNumBuffer[3]);
        _alphaNum.writeDisplay();
        _alphaNumUpdated = true;
      }
    }
    else if (_neoKeyPressedIndex == -1)
    {
      if (_alphaNumUpdated)
      {
        _alphaNumUpdated = false;
      }
    }
  }
  else if (_mode == Mode::Lights)
  {
    // alphaNum.print(neoSliderReading);
    // alphaNum.writeDisplay();
  }
  else if (_mode == Mode::Blinky)
  {
    if (--_alphaNumBlinkyDelay > 0)
      return;

    _alphaNum.clear();
    _alphaNumBuffer[3] = _alphaNumBuffer[2] = _alphaNumBuffer[1] = _alphaNumBuffer[0] = 0;
    for (int i = 0; i < 3; i++)
    {
      auto index = (_alphaNumBlinkyIndex + i) % AlphaNumPathLengths[_alphaNumPathIndex];
      const auto [digit, bitmask] = AlphaNumPaths[_alphaNumPathIndex][index];
      _alphaNumBuffer[digit] |= bitmask;
    }
    _alphaNum.writeDigitRaw(0, _alphaNumBuffer[0]);
    _alphaNum.writeDigitRaw(1, _alphaNumBuffer[1]);
    _alphaNum.writeDigitRaw(2, _alphaNumBuffer[2]);
    _alphaNum.writeDigitRaw(3, _alphaNumBuffer[3]);
    _alphaNum.writeDisplay();

    _alphaNumBlinkyIndex = (_alphaNumBlinkyIndex + 1) % AlphaNumPathLengths[_alphaNumPathIndex];
    _alphaNumBlinkyDelay = 6;
  }
}

void InputTaskHandler::alphaNumClear()
{
  _alphaNum.clear();
  _alphaNum.writeDisplay();
}

void InputTaskHandler::neoKeySetup()
{
  if (_neoKeyInit)
  {
    log_w("NeoKey already initialized");
    return;
  }

  if (!_neoKey.begin())
  {
    log_e("Couldn't find NeoKey seesaw(s) on default address(es)");
    return;
  }

  for (uint16_t i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    _neoKey.setPixelColor(i, 0x808080);
    _neoKey.show();
    delay(50);
  }
  for (uint16_t i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    _neoKey.setPixelColor(i, 0x000000);
    _neoKey.show();
    delay(50);
  }

  for (int i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    _neoKey.registerCallback(i, InputTaskHandler::neoKeyCallbackStatic);
  }

  log_i("NeoKey initialized");
  _neoKeyInit = true;
}

void InputTaskHandler::neoKeyUpdate()
{
  if (!_neoKeyInit)
  {
    return;
  }

  _neoKey.read();

  // debounce after key press
  if (millis() - _neoKeyLastPressMillis < 100 || millis() - _neoKeyLastReleaseMillis < 100)
  {
    return;
  }

  if (_mode == Mode::Lights)
  {
    if (_neoSliderReading != _neoSliderLastReading)
    {
      // int onCount = map(neoSliderReading, 0, 1023, 0, SS_NEOKEY_COUNT);
      // for (int i = 0; i < SS_NEOKEY_COUNT; i++)
      // {
      //   neoKey.setPixelColor(i, i < onCount ? wheel(map(i, 0, SS_NEOKEY_COUNT, 0, 255)) : 0);
      // }
      // neoKey.show();
      auto keyIndex = map(_neoSliderReading, 0, 1023, 0, SS_NEOKEY_COUNT);
      auto color = _neoSliderReading > _neoSliderLastReading ? wheel(map(keyIndex, 0, SS_NEOKEY_COUNT, 0, 255)) : 0;
      _neoKey.setPixelColor(keyIndex, color);
      _neoKey.show();
    }
  }
  else if (_mode == Mode::Blinky)
  {
    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      if (--_neoKeyBlinkyDelay[i] > 0)
        continue;

      _neoKeyPixelState[i] = !_neoKeyPixelState[i];
      _neoKeyBlinkyDelay[i] = random(BASE_DELAY, BASE_DELAY * 2);

      uint32_t color = _neoKeyPixelState[i] ? wheel(_rotaryWheelPos + random(0, 16)) : 0;
      _neoKey.setPixelColor(i, color);
    }
    _neoKey.show();
  }
}

void InputTaskHandler::neoKeyClear()
{
  for (int i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    _neoKey.setPixelColor(i, 0x000000);
  }
  _neoKey.show();
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
  _neoKeyPressedIndex = -1;

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    _neoKeyPressedIndex = key;
    _neoKeyLastPressMillis = millis();
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    _neoKeyLastReleaseMillis = millis();
  }

  if (_mode == Mode::Music)
  {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    {
      color = wheel(map(key, 0, SS_NEOKEY_COUNT, 0, 255)); // TODO also eventually show note name on display
      tone = _BaseToneFrequencies[key % 8];
    }
  }
  else if (_mode == Mode::Lights)
  {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    {
      _neoKeyPixelState[key] = !_neoKeyPixelState[key];
    }

    if (_neoKeyPixelState[key])
    {
      color = wheel(map(key, 0, SS_NEOKEY_COUNT, 0, 255));
    }
  }
  else if (_mode == Mode::Blinky)
  {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
    {
      _neoKeyBlinkyDelay[key] = 1000;
    }
  }

  if (tone > 0)
  {
    int pitchModifier = map(_neoSliderReading, 0, 1023, -200, 200);
    tone += pitchModifier;
  }

  _neoKey.setPixelColor(key, color);
  _neoKey.show();
  ack1Tone(tone);
}

void InputTaskHandler::neoSliderSetup()
{
  if (_neoSliderInit)
  {
    log_w("NeoSlider already initialized");
    return;
  }

  if (!_neoSliderSs.begin(SS_NEOSLIDER_ADDR) || !_neoSliderPixels.begin(SS_NEOSLIDER_ADDR))
  {
    log_e("Couldn't find NeoSlider seesaw on default address");
    return;
  }

  uint16_t version = _neoSliderSs.getVersion() >> 16;
  if (version != 5295)
  {
    log_e("Wrong seesaw firmware for slider loaded: %d", version);
    return;
  }

  _neoSliderPixels.setBrightness(255);
  _neoSliderPixels.show();
  _neoSliderInit = true;

  log_i("NeoSlider initialized");
}

void InputTaskHandler::neoSliderRead()
{
  if (!_neoSliderInit)
  {
    return;
  }

  _neoSliderLastReading = _neoSliderReading;
  _neoSliderReading = 1023 - _neoSliderSs.analogRead(SS_NEOSLIDER_SLD_PIN); // invert slider reading

  if (_neoSliderReading != _neoSliderLastReading)
  {
    _neoSliderLastChangeMillis = millis(); // TODO: might need debounce?
  }
}

void InputTaskHandler::neoSliderClear()
{
  for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
  {
    _neoSliderPixels.setPixelColor(i, 0x000000);
  }
  _neoSliderPixels.show();
}

void InputTaskHandler::rotarySetup()
{
  if (_rotaryInit)
  {
    log_w("Rotary already initialized");
    return;
  }

  if (!_rotarySs.begin(SS_ROTARY_ADDR) || !_rotaryNeoPixel.begin(SS_ROTARY_ADDR))
  {
    log_e("Couldn't find rotary seesaw on default address");
    return;
  }

  uint32_t version = ((_rotarySs.getVersion() >> 16) & 0xFFFF);
  if (version != 4991)
  {
    log_e("Wrong seesaw firmware for rotary loaded: %d", version);
    return;
  }

  _rotaryNeoPixel.setBrightness(20);
  _rotaryNeoPixel.show();
  _rotarySs.pinMode(SS_ROTARY_BTN_PIN, INPUT_PULLUP);
  _rotaryEncPos = _rotarySs.getEncoderPosition();

  // Serial.println("Turning on interrupts");
  // delay(10);
  // rotarySs.setGPIOInterrupts((uint32_t)1 << SS_ROTARY_ENC_PIN, 1);
  // rotarySs.enableEncoderInterrupt();

  _rotaryInit = true;

  log_i("Rotary initialized");
}

void InputTaskHandler::rotaryUpdate()
{
  if (!_rotaryInit)
  {
    return;
  }

  if (!_rotarySs.digitalRead(SS_ROTARY_BTN_PIN))
  {
    if (!_rotaryIsPressed && millis() - _rotaryLastPressMillis > 100)
    {
      _rotaryIsPressed = true;
      _rotaryLastPressMillis = millis();
      changeMode();
      ack1Tone(1000);
      delay(100);
      ack1Tone(0);
    }
  }
  else
  {
    _rotaryIsPressed = false;
  }

  int32_t newEncPos = _rotarySs.getEncoderPosition();
  if (_rotaryEncPos != newEncPos)
  {
    // log_d(newEncPos);
    _rotaryWheelPos = (newEncPos * 5) & 0xFF;
    _rotaryNeoPixel.setPixelColor(0, wheel(_rotaryWheelPos)); // 3 rotations for full cycle with 24 step encoder?
    _rotaryNeoPixel.show();
    _rotaryEncPos = newEncPos;
    _rotaryLastChangeMillis = millis();
  }

  if (_mode == Mode::Music)
  {
    if (_neoKeyPressedIndex >= 0)
    {
      uint32_t color = wheel(map(_neoKeyPressedIndex, 0, SS_NEOKEY_COUNT, 0, 255));
      _rotaryNeoPixel.setPixelColor(0, color);
      _rotaryNeoPixel.show();
    }
  }
  else if (_mode == Mode::Lights)
  {
    if (_neoKeyPressedIndex >= 0)
    {
      uint32_t color = wheel(map(_neoKeyPressedIndex, 0, SS_NEOKEY_COUNT, 0, 255));
      _rotaryNeoPixel.setPixelColor(0, color);
      _rotaryNeoPixel.show();
    }
  }
  else if (_mode == Mode::Blinky)
  {
    if (!_rotaryIsPressed && --_rotaryBlinkyDelay <= 0)
    {
      _rotaryLedState = !_rotaryLedState;
      _rotaryBlinkyDelay = random(BASE_DELAY, BASE_DELAY * 2);

      uint32_t color = _rotaryLedState ? wheel(_rotaryWheelPos + random(0, 16)) : 0;
      _rotaryNeoPixel.setPixelColor(0, color);
      _rotaryNeoPixel.show();
    }
  }
}

void InputTaskHandler::rotaryClear()
{
  _rotaryNeoPixel.setPixelColor(0, 0x000000);
  _rotaryNeoPixel.show();
}