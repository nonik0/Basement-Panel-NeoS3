#pragma once
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "mazeRunner.h"
#include "task_handler.h"

extern bool display;

class MazeRunnerTaskHandler : public TaskHandler
{
private:
    const int MAZE_DELAY_MS = 250;
    const uint32_t BLACK = Adafruit_NeoPixel::Color(0, 0, 0);
    const uint32_t RED = Adafruit_NeoPixel::Color(0xFF, 0, 0);
    const uint32_t ORANGE = Adafruit_NeoPixel::Color(0xEE, 0x44, 0);
    const uint32_t YELLOWGREEN = Adafruit_NeoPixel::Color(0xCC, 0xFF, 0);
    const uint32_t WARMWHITE = Adafruit_NeoPixel::Color(0xDD, 0xCC, 0x77);
    static const uint8_t EN_PIN = 39;
    static const uint8_t DATA_PIN = 16;
    static const int WIDTH = 7;
    static const int HEIGHT = 7;

    Adafruit_NeoPixel _pixels;
    MazeRunner *_mazeRunner;

public:
    MazeRunnerTaskHandler()
        : _pixels(WIDTH * HEIGHT, DATA_PIN)
    {
    }

    bool createTask() override;

private:
    void task(void *parameters) override;
};

bool MazeRunnerTaskHandler::createTask()
{
    log_d("Starting maze runner 7x7 setup");

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
        BLACK,       // off
        ORANGE,      // wall
        YELLOWGREEN, // runner
        RED,         // sentry
        WARMWHITE,   // exit
        [this](int x, int y, uint32_t c)
        { _pixels.setPixelColor(y * WIDTH + x, c); });

    _mazeRunner->init();

    xTaskCreatePinnedToCore(taskWrapper, "MazeRunner7x7Task", 4096 * 4, this, 2, &_taskHandle, 0); // other Arduino tasks are on Core 1
    log_d("Matrix setup complete");

    return true;
}

void MazeRunnerTaskHandler::task(void *parameters)
{
    log_d("Starting MazeRunner7x7Task");

    while (1)
    {
        //digitalWrite(EN_PIN, display); // turns off LDO
        _mazeRunner->update();
        _pixels.show();
        delay(MAZE_DELAY_MS);
    }
}
