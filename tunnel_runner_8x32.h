#pragma once

#include <Adafruit_LEDBackpack.h>

#include "display_task_handler.h"
#include "tunnel_runner_lib.h"

class TunnelRunner8x32TaskHandler : public DisplayTaskHandler
{
private:
    static const uint8_t I2C_ADDR = 0x72;
    static const uint8_t I2C_ADDR_2 = 0x73;
    static const uint8_t TASK_PRIORITY = 5;
    static const uint8_t PIXEL_GAP = 4;
    const int TUNNEL_DELAY_MS = 20;

    Adafruit_8x16minimatrix _matrix = Adafruit_8x16minimatrix();
    Adafruit_8x16minimatrix _matrix2 = Adafruit_8x16minimatrix();
    TunnelRunner *_tunnelRunner;

public:
    TunnelRunner8x32TaskHandler() {}

    bool createTask() override;

private:
    void task(void *parameters) override;
    void drawPixel(int x, int y, uint32_t c);
};

bool TunnelRunner8x32TaskHandler::createTask()
{
    log_i("Starting TunnelRunner8x16 setup");

    if (_taskHandle != NULL)
    {
        log_w("Task already started");
        return false;
    }

    if (!_matrix.begin(I2C_ADDR) || !_matrix2.begin(I2C_ADDR_2))
    {
        log_e("Matrix 1 or 2 not found");
        return false;
    }

    _matrix.setRotation(2);
    _matrix2.setRotation(2);
    _matrix.setBrightness(5);
    _matrix2.setBrightness(5);

    _tunnelRunner = new TunnelRunner(
        _matrix.width(),
        _matrix.height() * 2 + PIXEL_GAP,
        Up,
        LED_OFF, // path
        LED_ON,  // wall
        LED_ON,  // runner
        [this](int x, int y, uint32_t c) { drawPixel(x, y, c); });

    _tunnelRunner->init();

    xTaskCreatePinnedToCore(taskWrapper, "TunnelRunner8x32Task", 4096 * 4, this, TASK_PRIORITY, &_taskHandle, 0); // other Arduino tasks are on Core 1

    log_i("TunnelRunner8x16 setup complete");
    return true;
}

void TunnelRunner8x32TaskHandler::task(void *parameters)
{
    delay(10);
    log_i("Starting TunnelRunner8x32Task");

    while (1)
    {
        if (!_display)
        {
            _matrix.clear();
            _matrix2.clear();
            _matrix.writeDisplay();
            _matrix2.writeDisplay();
            delay(TUNNEL_DELAY_MS);
            continue;
        }

        if (_tunnelRunner->update())
        {
            _matrix.writeDisplay();
            _matrix2.writeDisplay();
        }
        delay(TUNNEL_DELAY_MS);
    }
}

void TunnelRunner8x32TaskHandler::drawPixel(int x, int y, uint32_t c)
{
    if (y < _matrix.height())
    {
        _matrix.drawPixel(x, y, c);
    }
    else if (y >= _matrix.height() + PIXEL_GAP)
    {
        _matrix2.drawPixel(x, y - _matrix.height() - PIXEL_GAP, c);
    }
}
