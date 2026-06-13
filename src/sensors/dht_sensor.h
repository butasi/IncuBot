#pragma once

#include <DHT.h>

class DHTSensor {
public:
    DHTSensor(uint8_t pin, uint8_t type);

    void begin();
    void update();

    float getTemperature() const;
    float getHumidity() const;

private:
    DHT dht;

    float temperature = 0.0f;
    float humidity    = 0.0f;
};