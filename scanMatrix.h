#pragma once

#include <Wire.h>
#include "display_task_handler.h"

class ScanMatrixTaskHandler : public DisplayTaskHandler
{
private:
    static const uint8_t I2C_ADDR = 0x15;

public:
    ScanMatrixTaskHandler() {}
    bool createTask() override;
    void setDisplay(bool state) override;
    void setMessage(const char *message) override;

private:
    void task(void *parameters) override;
};

bool ScanMatrixTaskHandler::createTask()
{
    log_d("Starting ScanMatrix setup");

    if (_taskHandle != NULL)
    {
        log_w("Task already started");
        return false;
    }

    return true;
}

void ScanMatrixTaskHandler::setDisplay(bool displayState)
{
    DisplayTaskHandler::setDisplay(displayState);

    Wire.beginTransmission(I2C_ADDR);
    Wire.write((uint8_t)0x00);
    Wire.write((uint8_t)displayState);
    Wire.endTransmission();
}

void ScanMatrixTaskHandler::setMessage(const char *message)
{
    DisplayTaskHandler::setMessage(message);

    Wire.beginTransmission(I2C_ADDR);
    Wire.write((uint8_t)0x01);
    Wire.write((const uint8_t *)message, strlen(message));
    Wire.endTransmission();

    // const size_t chunkSize = 119;
    // //const size_t chunkSize = 3; // 32-byte, max chunk size for I2C transmission (-1 for end byte)
    // size_t messageLength = strlen(message);
    // for (size_t i = 0; i < messageLength; i += chunkSize)
    // {
    //     Wire.beginTransmission(I2C_ADDR);
    //     Wire.write((uint8_t)0x01);

    //     size_t remaining = messageLength - i;
    //     size_t currentChunkSize = remaining > chunkSize ? chunkSize : remaining;

    //     Wire.write((const uint8_t *)(message + i), currentChunkSize);

    //     if (i + currentChunkSize >= messageLength)
    //     {
    //         Wire.write('\0');
    //     }

    //     Wire.endTransmission();
    //     delay(10);
    // }
}

void ScanMatrixTaskHandler::task(void *parameters)
{
}
