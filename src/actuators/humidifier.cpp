#include "actuators/humidifier.h"
#include "config.h"
#include <esp_task_wdt.h>

Humidifier::Humidifier(uint8_t p,
                       uint8_t s)
    : pin(p), sensePin(s) {}

void Humidifier::begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Humidifier::pulse(int ms) {
    digitalWrite(pin, HIGH);
    delay(ms);
    digitalWrite(pin, LOW);
}

HumState Humidifier::getState() const {
    return state;
}

void Humidifier::forcePulse(bool on) {
    pulse(on ? 200 : 200);
}

void Humidifier::update(bool autoMode,
                        float humidity) {

    unsigned long now = millis();

    // --- simple control logic (same behavior) ---
    if (autoMode) {
        if (humidity < HUM_LOW)
            desired = true;

        if (humidity > HUM_HIGH)
            desired = false;
    }

    // --- ADC-based real state read is simplified here ---
    float v = analogRead(sensePin) *
              3.3f / 4095.0f;

    state = (v > 3.0f)
            ? HUM_OFF
            : HUM_MODE1;

    assumedOn = (state != HUM_OFF);

    // --- burst protection ---
    if (assumedOn && onSince == 0)
        onSince = now;

    if (assumedOn &&
        now - onSince > HUM_BURST_MS) {

        desired = false;
        cooldown = true;
        offSince = now;
        onSince = 0;
    }

    if (cooldown &&
        now - offSince > HUM_COOLDOWN_MS) {

        cooldown = false;
    }

    // --- switching logic ---
    if (cooldown) return;

    if (desired && !assumedOn) {
        pulse(200);
        onSince = now;
    }

    if (!desired && assumedOn) {
        pulse(200);
        cooldown = true;
        offSince = now;
        onSince = 0;
    }

    esp_task_wdt_reset();
}