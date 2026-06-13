#include "heater.h"

constexpr float HEATER_ON_TEMP  = 37.2f;
constexpr float HEATER_OFF_TEMP = 37.5f;

void Heater::begin(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _state = false;
    _lastSwitch = millis();
}

void Heater::update(float temp) {

    uint32_t now = millis();

    if (now - _lastSwitch < MIN_INTERVAL) return;

    if (!_state && temp <= HEATER_ON_TEMP) {
        _state = true;
        _lastSwitch = now;
        digitalWrite(_pin, HIGH);
    }

    else if (_state && temp >= HEATER_OFF_TEMP) {
        _state = false;
        _lastSwitch = now;
        digitalWrite(_pin, LOW);
    }
}

bool Heater::isOn() {
    return _state;
}