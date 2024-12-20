
#pragma once

#include <Adafruit_LEDBackpack.h>

#include "display_task_handler.h"

class Matrix8x16TaskHandler : public DisplayTaskHandler
{
private:
    static const uint8_t I2C_ADDR = 0x72;
    static const uint8_t TASK_PRIORITY = 5;
    const int DELAY_MS = 60;

    Adafruit_8x16minimatrix _matrix = Adafruit_8x16minimatrix();
    int _scroll = 0;
    uint16_t _messageWidth;

public:
    Matrix8x16TaskHandler() {}
    bool createTask() override;
    void setMessage(const char *message) override;

private:
    void task(void *parameters) override;
    void initializeMessage();
};

bool Matrix8x16TaskHandler::createTask()
{
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

    strcpy(_message, "Stella and Beau and ");

    _matrix.setBrightness(5);
    _matrix.setTextSize(2);
    _matrix.setTextWrap(false);
    _matrix.setTextColor(LED_ON);

    initializeMessage();

    xTaskCreate(taskWrapper, "Matrix8x16Task", 4096, this, TASK_PRIORITY, &_taskHandle);
    log_d("Matrix initialized and task started");

    return true;
}

void Matrix8x16TaskHandler::setMessage(const char *message)
{
    DisplayTaskHandler::setMessage(message);
    initializeMessage();
}

void Matrix8x16TaskHandler::task(void *parameters)
{
    log_d("Starting Matrix8x16Task");

    while (1)
    {
        if (!_display)
        {
            _matrix.clear();
            _matrix.writeDisplay();
            delay(DELAY_MS);
            continue;
        }

        _matrix.clear();
        _matrix.setCursor(_scroll, 1);
        _matrix.print(_message);
        _matrix.writeDisplay();

        if (--_scroll < -_messageWidth)
        {
            _scroll = _matrix.width();
        }

        delay(DELAY_MS);
    }
}

void Matrix8x16TaskHandler::initializeMessage()
{
    int16_t d1;
    uint16_t d2;
    _matrix.getTextBounds(String(_message), 0, 0, &d1, &d1, &_messageWidth, &d2);
    _scroll = _matrix.width();
}