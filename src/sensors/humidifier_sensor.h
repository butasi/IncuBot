#ifndef HUMIDIFIER_SENSOR_H
#define HUMIDIFIER_SENSOR_H

#include <Arduino.h>

enum HumState {
    HUM_OFF,
    HUM_MODE1,
    HUM_MODE2
};

class HumidifierSensor {
public:
    HumidifierSensor(uint8_t adcPin);

    void begin();
    HumState readState();
    String stateToString(HumState state);

private:
    uint8_t pin;
};

#endif