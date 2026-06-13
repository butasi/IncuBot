#include "humidifier_sensor.h"
#include "config.h"
#include <esp_task_wdt.h>

HumidifierSensor::HumidifierSensor(uint8_t adcPin)
    : pin(adcPin) {}

void HumidifierSensor::begin() {
    pinMode(pin, INPUT);
    analogSetPinAttenuation(pin, ADC_11db);
}

HumState HumidifierSensor::readState() {

    float minV = 999;
    float maxV = 0;
    float sumV = 0;

    for (int i = 0; i < HUM_SAMPLE_COUNT; i++) {

        int raw = analogRead(pin);
        float v = raw * 3.3f / 4095.0f;

        sumV += v;
        if (v < minV) minV = v;
        if (v > maxV) maxV = v;

        delay(HUM_SAMPLE_DELAY);
        esp_task_wdt_reset();
    }

    float avg = sumV / HUM_SAMPLE_COUNT;
    float fluct = maxV - minV;

    if (avg > 3.0f && fluct < 0.5f)
        return HUM_OFF;

    if (avg > 2.35f)
        return HUM_MODE2;

    return HUM_MODE1;
}

String HumidifierSensor::stateToString(HumState state) {
    switch (state) {
        case HUM_OFF:   return "OFF";
        case HUM_MODE1: return "MODE1";
        case HUM_MODE2: return "MODE2";
        default:        return "UNKNOWN";
    }
}