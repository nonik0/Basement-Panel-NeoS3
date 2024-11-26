#pragma once

#include <Adafruit_NeoPixel.h>

#include "display_task_handler.h"
#include "maze_runner.h"

class MazeRunner7x7TaskHandler : public DisplayTaskHandler
{
private:
    const int MAZE_DELAY_MS = 60;
    const uint32_t BLACK = Adafruit_NeoPixel::Color(0x00, 0x00, 0x00);
    const uint32_t RED = Adafruit_NeoPixel::Color(0xFF, 0x00, 0x00);
    const uint32_t ORANGE = Adafruit_NeoPixel::Color(0xCC, 0x44, 0x00);
    const uint32_t YELLOW = Adafruit_NeoPixel::Color(0xFF, 0xFF, 0x00);
    const uint32_t YELLOWGREEN = Adafruit_NeoPixel::Color(0xCC, 0xFF, 0x00);
    const uint32_t GREEN = Adafruit_NeoPixel::Color(0x00, 0xFF, 0x00);
    const uint32_t BLUE = Adafruit_NeoPixel::Color(0x00, 0x00, 0x77);
    const uint32_t PURPLE = Adafruit_NeoPixel::Color(0x77, 0x00, 0x77);
    static const uint8_t EN_PIN = 39;
    static const uint8_t DATA_PIN = 16;
    static const int WIDTH = 7;
    static const int HEIGHT = 7;

    Adafruit_NeoPixel _pixels;
    MazeRunner *_mazeRunner;

public:
    MazeRunner7x7TaskHandler() : _pixels(WIDTH * HEIGHT, DATA_PIN) {}

    bool createTask() override;
    void setDisplay(bool display) override;

private:
    void task(void *parameters) override;
};

bool MazeRunner7x7TaskHandler::createTask()
{
    log_d("Starting MazeRunner7x7 setup");

    if (_taskHandle != NULL)
    {
        log_w("Task already started");
        return false;
    }

    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, true);

    _pixels.begin();
    _pixels.setBrightness(10);

    _mazeRunner = new MazeRunner(
        WIDTH,
        HEIGHT,
        BLACK,  // off
        ORANGE, // wall
        YELLOW, // runner
        RED,    // sentry
        PURPLE, // exit
        [this](int x, int y, uint32_t c)
        { _pixels.setPixelColor(y * WIDTH + x, c); });

    _mazeRunner->init();

    log_d("Starting MazeRunner7x7Task");
    xTaskCreatePinnedToCore(taskWrapper, "MazeRunner7x7Task", 4096 * 4, this, 2, &_taskHandle, 0); // other Arduino tasks are on Core 1

    log_d("MazeRunner7x7 setup complete");
    return true;
}

void MazeRunner7x7TaskHandler::setDisplay(bool displayState)
{
    DisplayTaskHandler::setDisplay(displayState);

    log_d("Setting display to %s", displayState ? "on" : "off");
    digitalWrite(EN_PIN, displayState); // turns off LDO for 7x7 matrix
}

unsigned long lastMillis = 0;
void MazeRunner7x7TaskHandler::task(void *parameters)
{
    while (1)
    {
        if (millis() - lastMillis > 15000) {
            log_d("MazeRunner7x7Task running");
            lastMillis = millis();
        }

        if (_display && _mazeRunner->update())
        {
            _pixels.show();
        }
        delay(MAZE_DELAY_MS);
    }
}
