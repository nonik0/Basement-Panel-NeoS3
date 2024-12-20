#pragma once

#include <Adafruit_LEDBackpack.h>

#include "display_task_handler.h"
#include "tunnel_runner_lib.h"

class TunnelRunner8x16TaskHandler : public DisplayTaskHandler
{
private:
    static const uint8_t I2C_ADDR = 0x72;
    static const uint8_t TASK_PRIORITY = 5;
    const int TUNNEL_DELAY_MS = 20;

    Adafruit_8x16minimatrix _matrix = Adafruit_8x16minimatrix();
    TunnelRunner *_tunnelRunner;

public:
    TunnelRunner8x16TaskHandler() {}

    bool createTask() override;

private:
    void task(void *parameters) override;
};

bool TunnelRunner8x16TaskHandler::createTask()
{
    log_i("Starting TunnelRunner8x16 setup");

    if (_taskHandle != NULL)
    {
        log_w("Task already started");
        return false;
    }

    if (!_matrix.begin(I2C_ADDR))
    {
        log_e("Matrix not found");
        return false;
    }

    _matrix.setBrightness(5);
    _matrix.setTextSize(2);

    _tunnelRunner = new TunnelRunner(
        _matrix.width(),
        _matrix.height(),
        Down,
        LED_OFF, // path
        LED_ON,  // wall
        LED_ON,  // runner
        [this](int x, int y, uint32_t c)
        { _matrix.drawPixel(x, y, c); });

    _tunnelRunner->init();

    xTaskCreatePinnedToCore(taskWrapper, "TunnelRunner8x16Task", 4096 * 4, this, 2, &_taskHandle, 0); // other Arduino tasks are on Core 1

    log_i("TunnelRunner8x16 setup complete");
    return true;
}

void TunnelRunner8x16TaskHandler::task(void *parameters)
{
    delay(10);
    log_i("Starting TunnelRunner8x16Task");

    while (1)
    {
        if (!_display)
        {
            _matrix.clear();
            _matrix.writeDisplay();
            delay(TUNNEL_DELAY_MS);
            continue;
        }

        if (_tunnelRunner->update())
        {
            _matrix.writeDisplay();
        }
        delay(TUNNEL_DELAY_MS);
    }
}
