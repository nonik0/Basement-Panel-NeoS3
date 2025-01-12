#pragma once

#include <Adafruit_LEDBackpack.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_NeoKey_1x4.h>
#include <seesaw_neopixel.h>
#include <tuple>

#include "display_task_handler.h"
#include "font3x4.h"
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
#define SS_NEOKEY3_ADDR 0x32
#define SS_NEOKEY4_ADDR 0x33
#define SS_NEOKEY5_ADDR 0x34
#define SS_NEOKEY_KEY_COUNT 4
#define SS_NEOKEY_COUNT 5 * SS_NEOKEY_KEY_COUNT

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

using namespace std;

// I hate this
class MusicMatrixTaskHandler;
extern MusicMatrixTaskHandler musicMatrix;

const tuple<uint8_t, uint16_t> AlphaNumLoopPath[] = {
    {0, ALPHANUM_SEG_A}, {1, ALPHANUM_SEG_A}, {2, ALPHANUM_SEG_A}, {3, ALPHANUM_SEG_A}, {3, ALPHANUM_SEG_B}, {3, ALPHANUM_SEG_C}, {3, ALPHANUM_SEG_D}, {2, ALPHANUM_SEG_D}, {1, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_E}, {0, ALPHANUM_SEG_F}};
const int AlphaNumLoopPathLength = sizeof(AlphaNumLoopPath) / sizeof(AlphaNumLoopPath[0]);

const tuple<uint8_t, uint16_t> AlphaNumFigure8Path[] = {
    {0, ALPHANUM_SEG_A}, {1, ALPHANUM_SEG_H}, {1, ALPHANUM_SEG_N}, {2, ALPHANUM_SEG_D}, {3, ALPHANUM_SEG_D}, {3, ALPHANUM_SEG_C}, {3, ALPHANUM_SEG_B}, {3, ALPHANUM_SEG_A}, {2, ALPHANUM_SEG_K}, {2, ALPHANUM_SEG_L}, {1, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_E}, {0, ALPHANUM_SEG_F}};
const int AlphaNumFigure8PathLength = sizeof(AlphaNumFigure8Path) / sizeof(AlphaNumFigure8Path[0]);

const tuple<uint8_t, uint16_t> AlphaNumWavyPath[] = {
    {0, ALPHANUM_SEG_A}, {1, ALPHANUM_SEG_H}, {1, ALPHANUM_SEG_N}, {2, ALPHANUM_SEG_L}, {2, ALPHANUM_SEG_K}, {3, ALPHANUM_SEG_A}, {3, ALPHANUM_SEG_B}, {3, ALPHANUM_SEG_C}, {3, ALPHANUM_SEG_D}, {2, ALPHANUM_SEG_N}, {2, ALPHANUM_SEG_H}, {1, ALPHANUM_SEG_K}, {1, ALPHANUM_SEG_L}, {0, ALPHANUM_SEG_D}, {0, ALPHANUM_SEG_E}, {0, ALPHANUM_SEG_F}};
const int AlphaNumWavyPathLength = sizeof(AlphaNumWavyPath) / sizeof(AlphaNumWavyPath[0]);

const tuple<uint8_t, uint16_t> AlphaNumZLoopPath[] = {
    {0, ALPHANUM_SEG_A}, {1, ALPHANUM_SEG_A}, {2, ALPHANUM_SEG_A}, {3, ALPHANUM_SEG_A}, {3, ALPHANUM_SEG_B}, {3, ALPHANUM_SEG_G2}, {3, ALPHANUM_SEG_G1}, {2, ALPHANUM_SEG_G2}, {2, ALPHANUM_SEG_G1}, {1, ALPHANUM_SEG_G2}, {1, ALPHANUM_SEG_G1}, {0, ALPHANUM_SEG_G2}, {0, ALPHANUM_SEG_G1}, {0, ALPHANUM_SEG_E}, {0, ALPHANUM_SEG_D}, {1, ALPHANUM_SEG_D}, {2, ALPHANUM_SEG_D}, {3, ALPHANUM_SEG_D}, {3, ALPHANUM_SEG_C}, {3, ALPHANUM_SEG_G2}, {3, ALPHANUM_SEG_G1}, {2, ALPHANUM_SEG_G2}, {2, ALPHANUM_SEG_G1}, {1, ALPHANUM_SEG_G2}, {1, ALPHANUM_SEG_G1}, {0, ALPHANUM_SEG_G2}, {0, ALPHANUM_SEG_G1}, {0, ALPHANUM_SEG_F}};
const int AlphaNumZLoopPathLength = sizeof(AlphaNumZLoopPath) / sizeof(AlphaNumZLoopPath[0]);

const tuple<uint8_t, uint16_t> *AlphaNumPaths[] = {AlphaNumLoopPath, AlphaNumFigure8Path, AlphaNumWavyPath, AlphaNumZLoopPath};
int AlphaNumPathLengths[] = {AlphaNumLoopPathLength, AlphaNumFigure8PathLength, AlphaNumWavyPathLength, AlphaNumZLoopPathLength};
int AlphaNumPathCount = sizeof(AlphaNumPaths) / sizeof(AlphaNumPaths[0]);

const char *Ack1BootMessage = "Hello world!";
const char *Ack1Message = "Dad loves Stella and Beau!";

uint32_t RED = 0xFF0000;
uint32_t GREEN = 0x00FF00;
uint32_t BLUE = 0x0000FF;

class MusicMatrixTaskHandler : public DisplayTaskHandler
{
private:
  static const int SONG_TASK_PRIORITY = 10;
  static const int TASK_PRIORITY = 6;

  enum Mode
  {
    Music,
    PlaySong,
    Lights,
    Blinky,
  };
  const int ModeCount = 4;

  const int DefaultTimingUnitMs = 60;
  const int TimingUnitMinMs = 5;
  const int TimingUnitMaxMs = 200;
  const int TimingUnitStepMs = 5;

  TaskHandle_t _songTask = NULL;
  int _octaveAdjust;
  int _timingUnitMs;

  Mode _mode = Mode::Music;
  unsigned long _lastInput = 0;
  const int InputTimeout = 60 * 1000;
  volatile bool _isSongPlaying = false;
  volatile int _songIndex = -1;
  volatile unsigned long _songTaskWatchdogMillis;

  bool _ack1Init = false;
  bool _ack1Display = false;

  bool _alphaNumInit = false;
  Adafruit_AlphaNum4 _alphaNum = Adafruit_AlphaNum4();
  char _alphaNumBuffer[ALPHANUM_CHAR_COUNT] = {' ', ' ', ' ', ' '};
  uint16_t _alphaNumRawBuffer[ALPHANUM_CHAR_COUNT];
  int _alphaNumBlinkyDelay;
  int _alphaNumBlinkyIndex;
  int _alphaNumPathIndex;

  bool _neoKeyInit = false;
  Adafruit_NeoKey_1x4 _neokeyArray[5] = {
      Adafruit_NeoKey_1x4(SS_NEOKEY1_ADDR),
      Adafruit_NeoKey_1x4(SS_NEOKEY2_ADDR),
      Adafruit_NeoKey_1x4(SS_NEOKEY3_ADDR),
      Adafruit_NeoKey_1x4(SS_NEOKEY4_ADDR),
      Adafruit_NeoKey_1x4(SS_NEOKEY5_ADDR)};
  Adafruit_MultiNeoKey1x4 _neoKey = Adafruit_MultiNeoKey1x4((Adafruit_NeoKey_1x4 *)_neokeyArray, 5, 1);
  int _neoKeyJustPressedIndex = -1;
  int _neoKeyJustReleasedIndex = -1;
  // light/blinky mode state
  bool _neoKeyPixelState[SS_NEOKEY_COUNT];
  int _neoKeyBlinkyDelay[SS_NEOKEY_COUNT];

  bool _neoSliderInit = false;
  Adafruit_seesaw _neoSliderSs;
  seesaw_NeoPixel _neoSliderPixels = seesaw_NeoPixel(SS_NEOSLIDER_LED_COUNT, SS_NEOSLIDER_LED_PIN, NEO_GRB + NEO_KHZ800);
  bool _neoSliderJustChanged = false;
  uint16_t _neoSliderReading = 0;
  uint16_t _neoSliderLastReading = 0;
  unsigned long _neoSliderLastChangeMillis = 0;
  // light/blinky mode state
  bool _neoSliderPixelState[SS_NEOSLIDER_LED_COUNT];
  int _neoSliderBlinkyDelay[SS_NEOSLIDER_LED_COUNT];

  bool _rotaryInit = false;
  Adafruit_seesaw _rotarySs;
  seesaw_NeoPixel _rotaryNeoPixel = seesaw_NeoPixel(SS_ROTARY_LED_COUNT, SS_ROTARY_LED_PIN, NEO_GRB + NEO_KHZ800);
  bool _rotaryJustPressed = false;
  bool _rotaryJustRotated = false;
  int32_t _rotaryEncPos;
  int32_t _rotaryLastEncPos;
  uint8_t _rotaryWheelPos;
  bool _rotaryIsPressed = false;
  unsigned long _rotaryLastPressMillis = 0;
  // light/blinky mode state
  bool _rotaryLedState;
  int _rotaryBlinkyDelay;

public:
  MusicMatrixTaskHandler() {}

  bool createTask() override;
  void setDisplay(bool display);

private:
  void task(void *parameters) override;
  void update();
  void showError(const char *message);
  uint32_t wheel(uint8_t wheelPos);

  void updateMusicFreeplay();
  void updateMusicPlay();
  void updateLights();
  void updateBlinky();
  void changeMode(int mode = -1);

  void playSongTask(void *parameters); // has static wrapper
  void playNote(uint8_t noteIndex, uint8_t octave, uint8_t timing);
  void startSong(int songIndex);
  void stopSong();
  void showMusicMenu();

  void ack1Setup();
  bool ack1Command(uint8_t cmd, const uint8_t *data = NULL, size_t len = 0);
  bool ack1Response(uint8_t *data, size_t len);
  void ack1Clear();
  void ack1Tone(uint16_t freq);
  void ack1Wake();

  void alphaNumSetup();
  void alphaNumShiftIn(const char *str, size_t len = 0);
  void alphaNumClear();

  void neoKeySetup();
  void neoKeyRead();
  void neoKeyClear(bool show = true);
  void neoKeyDisplay(char c);
  NeoKey1x4Callback neoKeyCallback(keyEvent evt);
  static NeoKey1x4Callback neoKeyCallbackStatic(keyEvent evt);

  void neoSliderSetup();
  void neoSliderRead();
  void neoSliderClear(bool show = true);

  void rotarySetup();
  void rotaryRead();
  void rotaryClear();

  static void playSongTaskWrapper(void *parameters)
  {
    MusicMatrixTaskHandler *handler = static_cast<MusicMatrixTaskHandler *>(parameters);
    handler->playSongTask(parameters);
  }
};

bool MusicMatrixTaskHandler::createTask()
{
  log_i("Starting input setup");

  if (_taskHandle != NULL)
  {
    log_w("Task already started");
    return false;
  }

  // let I2C devices settle
  while (millis() < 5000)
    ;

  alphaNumSetup();
  neoKeySetup();
  neoSliderSetup();
  rotarySetup();
  delay(1000);
  ack1Setup(); // slow to initialize

  log_i("Starting InputTask");
  xTaskCreatePinnedToCore(taskWrapper, "InputTask", 4096 * 4, this, TASK_PRIORITY, &_taskHandle, 1);

  log_i("Input setup complete");
  return true;
}

void MusicMatrixTaskHandler::setDisplay(bool displayState)
{
  DisplayTaskHandler::setDisplay(displayState);

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

void MusicMatrixTaskHandler::task(void *parameters)
{
  while (1)
  {
    neoKeyRead();
    neoSliderRead();
    rotaryRead();

    if (_display)
    {
      update();
    }

    delay(DELAY_MS);
  }
}

void MusicMatrixTaskHandler::update()
{
  // timeout to blinky mode
  if (_mode != Mode::Blinky && millis() - _lastInput > InputTimeout)
  {
    changeMode(Mode::Blinky);
  }

  // rotary button changes mode
  if (_rotaryJustPressed)
  {
    changeMode();
  }

  if (_mode == Mode::Music)
  {
    updateMusicFreeplay();
  }
  else if (_mode == Mode::PlaySong)
  {
    updateMusicPlay();
  }
  else if (_mode == Mode::Lights)
  {
    updateLights();
  }
  else if (_mode == Mode::Blinky)
  {
    updateBlinky();
  }
}

void MusicMatrixTaskHandler::showError(const char *message)
{
  log_e("Error: %s", message);
  const char *prefix = "ERR:";
  char fullMessage[MaxMessageSize];
  snprintf(fullMessage, MaxMessageSize, "%s %s", prefix, message);
  for (int i = 0; i < strlen(fullMessage); i++)
  {
    alphaNumShiftIn(fullMessage + i, 1);
    delay(300);
  }
  for (int i = 0; i < 4; i++)
  {
    alphaNumShiftIn(" ", 1);
    delay(300);
  }
}

uint32_t MusicMatrixTaskHandler::wheel(uint8_t wheelPos)
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

void MusicMatrixTaskHandler::updateMusicFreeplay()
{
  if (_neoKeyJustPressedIndex >= 0)
  {
    // ACK1 tone
    int note = _neoKeyJustPressedIndex % NOTE_COUNT;
    int noteOctave = _neoKeyJustPressedIndex / NOTE_COUNT;
    int baseOctave = map(_neoSliderReading, 0, 1023, MIN_OCTAVE, MAX_OCTAVE);
    int octave = baseOctave + noteOctave;
    uint16_t freq = getNoteFrequency(note, octave);
    uint32_t color = wheel(map(note, 0, NOTE_COUNT, 0, 255));
    ack1Tone(freq);

    // alphanum note name
    const char *noteName = Notes[note];
    alphaNumShiftIn(noteName);

    // neokey color
    _neoKey.setPixelColor(_neoKeyJustPressedIndex, color);
    _neoKey.show();

    if (octave % 2 == 0)
    {
      color = color & 0x7F7F7F;
    }
    _neoSliderPixels.setPixelColor(4 - octave / 2, color);
    _neoSliderPixels.show();

    // rotary color
    _rotaryNeoPixel.setPixelColor(0, color);
    _rotaryNeoPixel.show();
  }

  if (_neoSliderJustChanged)
  {
    int lastOctave = map(_neoSliderLastReading, 0, 1023, MIN_OCTAVE, MAX_OCTAVE);
    int octave = map(_neoSliderReading, 0, 1023, MIN_OCTAVE, MAX_OCTAVE);

    if (lastOctave != octave)
    {
      alphaNumShiftIn(std::to_string(octave).c_str());
    }
  }

  // turn off tone and light on release
  if (_neoKeyJustReleasedIndex >= 0)
  {
    neoKeyClear(false);
    _neoKey.setPixelColor(_neoKeyJustReleasedIndex, 0x0);
    _neoKey.show();

    neoSliderClear();

    ack1Tone(0);
  }
}

void MusicMatrixTaskHandler::updateMusicPlay()
{
  if (_rotaryJustRotated)
  {
    int msIncrement = _rotaryEncPos < _rotaryLastEncPos ? TimingUnitStepMs : -TimingUnitStepMs;
    int newTimingUnitMs = constrain(_timingUnitMs + msIncrement, TimingUnitMinMs, TimingUnitMaxMs);

    if (newTimingUnitMs != _timingUnitMs)
    {
      _timingUnitMs = newTimingUnitMs;
      log_i("Timing unit updated: %d", _timingUnitMs);

      uint8_t value = map(_timingUnitMs, TimingUnitMinMs, TimingUnitMaxMs, 0x00, 0xFF);
      uint32_t color = (0xFF - value) << 16 | value;
      _rotaryNeoPixel.setPixelColor(0, color);
      _rotaryNeoPixel.show();

      if (!_isSongPlaying)
      {
        char timingUnitStr[5];
        snprintf(timingUnitStr, 5, "%4d", _timingUnitMs);
        alphaNumShiftIn(timingUnitStr);
      }
    }
  }

  if (_neoSliderJustChanged)
  {
    int newOctaveAdjust = map(_neoSliderReading, 0, 1023, -2, 2);

    if (newOctaveAdjust != _octaveAdjust)
    {
      _octaveAdjust = newOctaveAdjust;
      log_i("Octave adjust updated: %d", _octaveAdjust);

      if (!_isSongPlaying)
      {
        char timingUnitStr[5];
        snprintf(timingUnitStr, 5, "%4d", _octaveAdjust);
        alphaNumShiftIn(timingUnitStr);
      }
    }
  }

  if (!_isSongPlaying)
  {
    if (_neoKeyJustPressedIndex >= 0)
    {
      if (_neoKeyJustPressedIndex < SongCount)
      {
        startSong(_neoKeyJustPressedIndex);
      }
      else
      {
        // error tone
        ack1Tone(1000);
        delay(100);
        ack1Tone(0);
        delay(100);
        ack1Tone(1000);
        delay(100);
        ack1Tone(0);
      }
    }
    else if (millis() - _lastInput > 1000)
    {
      showMusicMenu();
    }
  }
  else
  {
    // pressing any neokey stops playing song
    if (_neoKeyJustPressedIndex >= 0)
    {
      stopSong();
    }
  }
}

void MusicMatrixTaskHandler::updateLights()
{
  // rotary encoder changes color
  if (_rotaryJustRotated)
  {
    _rotaryWheelPos = (_rotaryEncPos * 5) & 0xFF;
    _rotaryNeoPixel.setPixelColor(0, wheel(_rotaryWheelPos)); // 3 rotations for full cycle with 24 step encoder?
    _rotaryNeoPixel.show();
  }

  if (_neoKeyJustPressedIndex >= 0)
  {
    uint32_t color = wheel(map(_neoKeyJustPressedIndex, 0, SS_NEOKEY_COUNT, 0, 255));

    // toggle neokey light
    _neoKeyPixelState[_neoKeyJustPressedIndex] = !_neoKeyPixelState[_neoKeyJustPressedIndex];
    _neoKey.setPixelColor(_neoKeyJustPressedIndex, _neoKeyPixelState[_neoKeyJustPressedIndex] ? color : 0);
    _neoKey.show();

    // set rotary light
    _rotaryNeoPixel.setPixelColor(0, color);
    _rotaryNeoPixel.show();
  }

  if (_neoKeyJustReleasedIndex >= 0)
  {
    ack1Tone(0);
  }

  if (_neoSliderJustChanged)
  {
    // slider colors
    int onCount = map(_neoSliderReading, 0, 1023, 0, SS_NEOSLIDER_LED_COUNT);
    for (uint8_t i = 0; i < _neoSliderPixels.numPixels(); i++)
    {
      _neoSliderPixels.setPixelColor(_neoSliderPixels.numPixels() - i - 1, i < onCount ? wheel(map(i, 0, _neoSliderPixels.numPixels(), 0, 255)) : 0);
    }
    _neoSliderPixels.show();

    // turn on/off neokey lights with slider
    int keyIndex = map(_neoSliderReading, 0, 1023, 0, SS_NEOKEY_COUNT);
    uint32_t color = _neoSliderReading > _neoSliderLastReading ? wheel(map(keyIndex, 0, SS_NEOKEY_COUNT, 0, 255)) : 0;
    _neoKey.setPixelColor(keyIndex, color);
    _neoKey.show();
  }
}

void MusicMatrixTaskHandler::updateBlinky()
{
  // pressing neokey resets light delay
  if (_neoKeyJustPressedIndex >= 0)
  {
    changeMode(Mode::Music);
    return;
  }

  // rotary encoder changes color
  if (_rotaryJustRotated)
  {
    _rotaryWheelPos = (_rotaryEncPos * 5) & 0xFF;
    _rotaryNeoPixel.setPixelColor(0, wheel(_rotaryWheelPos)); // 3 rotations for full cycle with 24 step encoder?
    _rotaryNeoPixel.show();
  }

  // blink alphanum
  if (--_alphaNumBlinkyDelay < 0)
  {
    _alphaNum.clear();
    _alphaNumRawBuffer[3] = _alphaNumRawBuffer[2] = _alphaNumRawBuffer[1] = _alphaNumRawBuffer[0] = 0;
    for (int i = 0; i < 3; i++)
    {
      int index = (_alphaNumBlinkyIndex + i) % AlphaNumPathLengths[_alphaNumPathIndex];

      uint8_t digit;
      uint16_t bitmask;
      std::tie(digit, bitmask) = AlphaNumPaths[_alphaNumPathIndex][index];
      _alphaNumRawBuffer[digit] |= bitmask;
    }
    _alphaNum.writeDigitRaw(0, _alphaNumRawBuffer[0]);
    _alphaNum.writeDigitRaw(1, _alphaNumRawBuffer[1]);
    _alphaNum.writeDigitRaw(2, _alphaNumRawBuffer[2]);
    _alphaNum.writeDigitRaw(3, _alphaNumRawBuffer[3]);
    _alphaNum.writeDisplay();

    _alphaNumBlinkyIndex = (_alphaNumBlinkyIndex + 1) % AlphaNumPathLengths[_alphaNumPathIndex];
    _alphaNumBlinkyDelay = 6;
  }

  // blink neokeys
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

  // blink neoslider
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

  // blink rotary
  if (--_rotaryBlinkyDelay <= 0)
  {
    _rotaryLedState = !_rotaryLedState;
    _rotaryBlinkyDelay = random(BASE_DELAY, BASE_DELAY * 2);

    uint32_t color = _rotaryLedState ? wheel(_rotaryWheelPos + random(0, 16)) : 0;
    _rotaryNeoPixel.setPixelColor(0, color);
    _rotaryNeoPixel.show();
  }
}

void MusicMatrixTaskHandler::changeMode(int desiredMode)
{
  const int ModeChangeDelay = 150;

  if (!_ack1Init || !_neoKeyInit || !_neoSliderInit || !_rotaryInit)
  {
    log_e("Not all components initialized");
    return;
  }

  // spin-down mode stuff
  if (_isSongPlaying)
  {
    stopSong();
  }

  // mode change stuff, transition beep in
  if (desiredMode >= 0)
  {
    _mode = (Mode)(desiredMode % ModeCount);
  }
  else
  {
    _mode = (Mode)((_mode + 1) % ModeCount);
    ack1Tone(800);
    delay(100);
    ack1Tone(0);
  }

  // spin-up mode stuff
  if (_mode == Music)
  {
    log_i("Switching to Music mode");
    alphaNumShiftIn("Play");
    neoKeyDisplay('1');
    delay(ModeChangeDelay);

    neoKeyClear();
    neoSliderClear();
    rotaryClear();
  }
  else if (_mode == PlaySong)
  {
    log_i("Switching to PlaySong mode");
    alphaNumShiftIn("Song");
    neoKeyDisplay('2');
    delay(ModeChangeDelay);

    neoSliderClear();
    rotaryClear();

    _timingUnitMs = DefaultTimingUnitMs;
    showMusicMenu();
  }
  else if (_mode == Lights)
  {
    log_i("Switching to Lights mode");
    alphaNumShiftIn("Lite");
    neoKeyDisplay('3');
    delay(ModeChangeDelay);

    // neokey lights
    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      _neoKeyPixelState[i] = true;
      _neoKey.setPixelColor(i, wheel(map(i, 0, SS_NEOKEY_COUNT, 0, 255)));
    }
    _neoKey.show();

    // neoslider lights
    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      _neoSliderPixelState[i] = true;
      _neoSliderPixels.setPixelColor(i, wheel(map(i, 0, SS_NEOSLIDER_LED_COUNT, 0, 255)));
    }
    _neoSliderPixels.show();

    // rotary light
    _rotaryNeoPixel.setPixelColor(0, wheel(_rotaryWheelPos));
    _rotaryNeoPixel.show();
  }
  else if (_mode == Blinky)
  {
    log_i("Switching to Blinky mode");
    alphaNumShiftIn("Blnk");
    neoKeyDisplay('4');
    delay(ModeChangeDelay);

    // choose random animation
    alphaNumClear();
    _alphaNumPathIndex = random(0, AlphaNumPathCount);
    _alphaNumBlinkyDelay = 0;
    _alphaNumBlinkyIndex = random(0, AlphaNumPathLengths[_alphaNumPathIndex]);

    // neokey blinky
    for (int i = 0; i < SS_NEOKEY_COUNT; i++)
    {
      _neoKeyPixelState[i] = true;
      _neoKeyBlinkyDelay[i] = random(BASE_DELAY * 5, BASE_DELAY * 10);
      _neoKey.setPixelColor(i, wheel(_rotaryWheelPos + random(0, 16)));
    }
    _neoKey.show();

    // neoslider blinky
    for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
    {
      _neoSliderPixelState[i] = true;
      _neoSliderBlinkyDelay[i] = random(BASE_DELAY * 5, BASE_DELAY * 10);
      _neoSliderPixels.setPixelColor(i, wheel(_rotaryWheelPos + random(0, 32)));
    }
    _neoSliderPixels.show();

    // rotary blinky
    _rotaryLedState = true;
    _rotaryBlinkyDelay = random(BASE_DELAY * 5, BASE_DELAY * 10);
    _rotaryNeoPixel.setPixelColor(0, wheel(_rotaryWheelPos + random(0, 32)));
    _rotaryNeoPixel.show();
  }

  // transition beep out
  if (desiredMode < 0)
  {
    ack1Tone(1000 + _mode * 100);
    delay(100);
    ack1Tone(0);
  }
}

void MusicMatrixTaskHandler::playSongTask(void *parameters)
{
  while (1)
  {
    if (!_isSongPlaying)
    {
      log_d("Waiting for song to play");
      _songTaskWatchdogMillis = millis();
      delay(10);
      continue;
    }

    if (_songIndex < 0 || _songIndex >= SongCount)
    {
      log_e("Invalid song index: %d", _songIndex);
      showError("Invalid song");
      _isSongPlaying = false;
      continue;
    }

    const uint16_t *song = Songs[_songIndex];
    const char *songName = SongName4Chars[_songIndex];

    log_i("Playing song %s", songName);
    alphaNumClear();
    neoKeyClear();
    alphaNumShiftIn(SongName4Chars[_songIndex]);
    delay(500);

    if (!_isSongPlaying)
    {
      continue;
    }

    int noteIndex = 0;
    uint16_t note = song[noteIndex++];
    while (_isSongPlaying && note != END)
    {
      uint8_t octave = (note & OCTAVE_MASK) >> 12;
      uint8_t index = (note & NOTE_MASK) >> 8;
      uint8_t timing = note & TIMING_MASK;

      playNote(index, octave, timing);

      note = song[noteIndex++];
      _songTaskWatchdogMillis = millis();
    }

    // signal end of song
    showMusicMenu();
    _isSongPlaying = false;
  }
}

void MusicMatrixTaskHandler::playNote(uint8_t noteIndex, uint8_t octave, uint8_t timing)
{
  neoKeyClear(false);
  neoSliderClear(false);

  int frequency = getNoteFrequency(noteIndex, octave + _octaveAdjust);
  ack1Tone(frequency);

  // don't show note if pause
  int sliderIndex = 4 - octave / 2; // show in 4 steps on slider
  if (frequency > 0)
  {
    // TODO: smaller increments than whole octave
    uint32_t noteColor = wheel(map(noteIndex, 0, NOTE_COUNT, 0, 255));
    uint32_t octaveColor = octave % 2 == 0 ? BLUE : RED; // at each index low octave is blue, high is red

    // show note on neokey
    _neoKey.setPixelColor(noteIndex, noteColor); // only use first 12 keys (for now?)
    _neoKey.show();

    // show octave on neoslider
    _neoSliderPixels.setPixelColor(sliderIndex, octaveColor);
    _neoSliderPixels.show();

    // show note on alphanum
    const char *noteName = Notes[noteIndex];
    alphaNumShiftIn(noteName);
  }
  else
  {
    alphaNumShiftIn(" ");
  }

  // wait
  delay(timing * _timingUnitMs);

  ack1Tone(0);

  // off
  if (frequency > 0)
  {
    _neoKey.setPixelColor(noteIndex, 0);
    _neoKey.show();
    _neoSliderPixels.setPixelColor(sliderIndex, 0);
    _neoSliderPixels.show();
  }
  // timing between notes (half unit of time)
  delay(_timingUnitMs >> 1);
}

void MusicMatrixTaskHandler::startSong(int songIndex)
{
  if (_isSongPlaying)
  {
    log_e("Song already playing");
    return;
  }

  if (_songTask != NULL && millis() - _songTaskWatchdogMillis > 1000)
  {
    log_e("Song task not responding, restarting");
    showError("Restarting song task");
    vTaskDelete(_songTask);
    xTaskCreate(playSongTaskWrapper, "PlaySongTask", 4096 * 20, this, SONG_TASK_PRIORITY, &_songTask);
  }
  else if (_songTask == NULL)
  {
    xTaskCreate(playSongTaskWrapper, "PlaySongTask", 4096 * 20, this, SONG_TASK_PRIORITY, &_songTask);
  }

  // pass song to task
  log_i("Starting song");
  _songIndex = songIndex;
  _isSongPlaying = true;
}

void MusicMatrixTaskHandler::stopSong()
{
  if (!_isSongPlaying)
  {
    log_e("No song playing");
    return;
  }

  if (_songTask != NULL && millis() - _songTaskWatchdogMillis > 1000)
  {
    log_w("Song task not responding");
    showError("TASK NOT RUNNING");
  }

  log_i("Stopping song");
  _isSongPlaying = false;
  _songIndex = -1;
}

void MusicMatrixTaskHandler::showMusicMenu()
{
  alphaNumShiftIn("Song");

  for (int i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    uint32_t color = (i < SongCount) ? wheel(map(i, 0, SongCount, 0x00, 0xFF)) : 0;
    _neoKey.setPixelColor(i, color);
  }
  _neoKey.show();
}

void MusicMatrixTaskHandler::ack1Setup()
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

  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)Ack1BootMessage, strlen(Ack1BootMessage));
  log_d("ACK1 status: %d", ack1Command(ACK1_STATUS_CMD));
  _ack1Display = true;
}

bool MusicMatrixTaskHandler::ack1Command(uint8_t cmd, const uint8_t *data, size_t len)
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
      log_d("Response byte %d: %02x", i, responseBytes[i]);
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

bool MusicMatrixTaskHandler::ack1Response(uint8_t *data, size_t len)
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

void MusicMatrixTaskHandler::ack1Clear()
{
  // one clear only stops scroll, second clear clears display (not sure why?)
  ack1Command(ACK1_LEDCLR_CMD);
  ack1Command(ACK1_LEDCLR_CMD);
}

void MusicMatrixTaskHandler::ack1Tone(uint16_t freq)
{
  if (freq == 0)
  {
    ack1Command(ACK1_TONEOFF_CMD);
    return;
  }

  uint8_t toneData[2];
  toneData[0] = freq >> 8;
  toneData[1] = freq & 0xFF;
  log_d("Tone freq: %d", freq);
  log_d("Tone data: %02x %02x", toneData[0], toneData[1]);
  ack1Command(ACK1_TONEON_CMD, toneData, 2);
}

void MusicMatrixTaskHandler::ack1Wake()
{
  ack1Command(ACK1_LEDSCROLL_CMD, (uint8_t *)Ack1Message, strlen(Ack1Message));
}

void MusicMatrixTaskHandler::alphaNumSetup()
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

  _alphaNum.setBrightness(8);

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

void MusicMatrixTaskHandler::alphaNumClear()
{
  _alphaNum.clear();
  _alphaNum.writeDisplay();
}

void MusicMatrixTaskHandler::alphaNumShiftIn(const char *str, size_t len)
{
  if (!_alphaNumInit)
  {
    return;
  }

  if (len <= 0)
  {
    len = strlen(str);
  }

  for (int i = 0; i < len; i++)
  {
    _alphaNumBuffer[0] = _alphaNumBuffer[1];
    _alphaNumBuffer[1] = _alphaNumBuffer[2];
    _alphaNumBuffer[2] = _alphaNumBuffer[3];
    _alphaNumBuffer[3] = str[i];

    _alphaNum.writeDigitAscii(0, _alphaNumBuffer[0]);
    _alphaNum.writeDigitAscii(1, _alphaNumBuffer[1]);
    _alphaNum.writeDigitAscii(2, _alphaNumBuffer[2]);
    _alphaNum.writeDigitAscii(3, _alphaNumBuffer[3]);
  }

  _alphaNum.writeDisplay();

  //log_i("AlphaNum buffer: %c%c%c%c", _alphaNumBuffer[0], _alphaNumBuffer[1], _alphaNumBuffer[2], _alphaNumBuffer[3]);
}

void MusicMatrixTaskHandler::neoKeySetup()
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
    _neoKey.registerCallback(i, MusicMatrixTaskHandler::neoKeyCallbackStatic);
  }

  log_i("NeoKey initialized");
  _neoKeyInit = true;
}

void MusicMatrixTaskHandler::neoKeyRead()
{
  if (!_neoKeyInit)
  {
    return;
  }

  _neoKeyJustPressedIndex = -1;
  _neoKeyJustReleasedIndex = -1;

  _neoKey.read();
}

void MusicMatrixTaskHandler::neoKeyClear(bool show)
{
  for (int i = 0; i < SS_NEOKEY_COUNT; i++)
  {
    _neoKey.setPixelColor(i, 0x000000);
  }

  if (show)
  {
    _neoKey.show();
  }
}

void MusicMatrixTaskHandler::neoKeyDisplay(char c)
{
  c -= (uint8_t)pgm_read_byte(&Font3x4.first); // Adjust the character index

  GFXglyph *glyph = &(((GFXglyph *)pgm_read_ptr(&Font3x4.glyph))[c]);
  uint8_t *bitmap = (uint8_t *)pgm_read_ptr(&Font3x4.bitmap);

  uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
  uint8_t w = pgm_read_byte(&glyph->width);
  uint8_t h = pgm_read_byte(&glyph->height);
  // ignore offsets for single char display
  // int8_t xo = pgm_read_byte(&glyph->xOffset);
  // int8_t yo = pgm_read_byte(&glyph->yOffset);

  uint8_t x, y, bits = 0, bit = 0;
  uint32_t color = wheel(random(0, 255));

  neoKeyClear(false);

  for (y = 0; y < h; y++)
  {
    for (x = 0; x < w; x++)
    {
      if (!(bit++ & 7))
      {
        bits = pgm_read_byte(&bitmap[bo++]);
      }
      if (bits & 0x80)
      {
        uint8_t yi = (h - 1) - y; // invert y
        _neoKey.setPixelColor(x * 4 + yi, color);
      }
      bits <<= 1;
    }
  }

  _neoKey.show();
}

NeoKey1x4Callback MusicMatrixTaskHandler::neoKeyCallbackStatic(keyEvent evt)
{
  return musicMatrix.neoKeyCallback(evt);
}

NeoKey1x4Callback MusicMatrixTaskHandler::neoKeyCallback(keyEvent evt)
{
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
  {
    log_i("NeoKey just pressed: %d", evt.bit.NUM);
    _neoKeyJustPressedIndex = evt.bit.NUM;
    _lastInput = millis();
  }
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
  {
    log_i("NeoKey just released: %d", evt.bit.NUM);
    _neoKeyJustReleasedIndex = evt.bit.NUM;
    _lastInput = millis();
  }

  return 0; // without this callback loops indefinitely
}

void MusicMatrixTaskHandler::neoSliderSetup()
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

void MusicMatrixTaskHandler::neoSliderRead()
{
  if (!_neoSliderInit)
  {
    return;
  }

  _neoSliderJustChanged = false;

  uint16_t newReading = 1023 - _neoSliderSs.analogRead(SS_NEOSLIDER_SLD_PIN); // invert slider reading

  if (abs(newReading - _neoSliderReading) > 1 && millis() - _neoSliderLastChangeMillis > 10)
  {
    log_d("NeoSlider reading: %d", newReading);
    _neoSliderLastReading = _neoSliderReading;
    _neoSliderReading = newReading;
    _neoSliderJustChanged = true;
    _neoSliderLastChangeMillis = millis();
    _lastInput = millis();
  }
}

void MusicMatrixTaskHandler::neoSliderClear(bool show)
{
  for (int i = 0; i < SS_NEOSLIDER_LED_COUNT; i++)
  {
    _neoSliderPixels.setPixelColor(i, 0x000000);
  }

  if (show)
  {
    _neoSliderPixels.show();
  }
}

void MusicMatrixTaskHandler::rotarySetup()
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

  _rotaryInit = true;

  log_i("Rotary initialized");
}

void MusicMatrixTaskHandler::rotaryRead()
{
  if (!_rotaryInit)
  {
    return;
  }

  _rotaryJustPressed = false;
  _rotaryJustRotated = false;

  if (!_rotarySs.digitalRead(SS_ROTARY_BTN_PIN))
  {
    if (!_rotaryIsPressed && millis() - _rotaryLastPressMillis > 100)
    {
      log_d("Rotary button just pressed");
      _rotaryJustPressed = true;
      _rotaryIsPressed = true;
      _rotaryLastPressMillis = millis();
      _lastInput = millis();
    }
  }
  else
  {
    _rotaryIsPressed = false;
  }

  int32_t newEncPos = _rotarySs.getEncoderPosition();
  if (_rotaryEncPos != newEncPos)
  {
    log_d("Rotary encoder pos: %d", newEncPos);
    _rotaryLastEncPos = _rotaryEncPos;
    _rotaryEncPos = newEncPos;
    _rotaryJustRotated = true;
    _lastInput = millis();
  }
}

void MusicMatrixTaskHandler::rotaryClear()
{
  _rotaryNeoPixel.setPixelColor(0, 0x000000);
  _rotaryNeoPixel.show();
}