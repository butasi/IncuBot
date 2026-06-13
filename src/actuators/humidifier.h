#pragma once
#include <Arduino.h>
#include "sensors/humidifier_sensor.h"

class Humidifier {
public:
    Humidifier(uint8_t pin,
               uint8_t sensePin);

    void begin();

    void update(bool autoMode,
                float humidity);

    void forcePulse(bool on);

    HumState getState() const;

private:
    void pulse(int ms = 200);

    uint8_t pin;
    uint8_t sensePin;

    bool desired = false;
    bool assumedOn = false;

    bool cooldown = false;

    unsigned long onSince = 0;
    unsigned long offSince = 0;

    HumState state = HUM_OFF;
};