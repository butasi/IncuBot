#include "sensors/dht_sensor.h"

DHTSensor::DHTSensor(uint8_t pin,
                     uint8_t type)
    : dht(pin, type) {}

void DHTSensor::begin() {
    dht.begin();
}

void DHTSensor::update() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t)) {
        temperature = t;
    }

    if (!isnan(h)) {
        humidity = h;
    }
}

float DHTSensor::getTemperature() const {
    return temperature;
}

float DHTSensor::getHumidity() const {
    return humidity;
}