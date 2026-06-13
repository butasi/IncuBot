#pragma once
#include <Arduino.h>

class Heater {
public:
    void begin(uint8_t pin);
    void update(float temp);
    bool isOn();

private:
    uint8_t _pin;
    bool _state = false;
    uint32_t _lastSwitch = 0;
    static constexpr uint32_t MIN_INTERVAL = 30000;
};