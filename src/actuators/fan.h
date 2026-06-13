#pragma once
#include <Arduino.h>

class Fan {
public:
    Fan(uint8_t en, uint8_t i1, uint8_t i2);

    void begin();

    void update(bool enabled,
                int speed,
                bool moving,
                bool emergency,
                bool circulation);

private:
    uint8_t enPin, in1, in2;

    bool running = false;
    bool ramping = false;

    unsigned long rampStart = 0;
};