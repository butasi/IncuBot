#include "actuators/fan.h"
#include "config.h"

Fan::Fan(uint8_t en, uint8_t i1, uint8_t i2)
    : enPin(en), in1(i1), in2(i2) {}

void Fan::begin() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    ledcSetup(FAN_PWM_CHANNEL, 5000, 8);
    ledcAttachPin(enPin, FAN_PWM_CHANNEL);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);

    ledcWrite(FAN_PWM_CHANNEL, 0);
}

void Fan::update(bool enabled,
                 int speed,
                 bool moving,
                 bool emergency,
                 bool circulation) {

    const int MAX_PWM = 90;   // HARD LIMIT to avoid brownout

    bool shouldRun = enabled;

    if (!shouldRun) {
        ledcWrite(FAN_PWM_CHANNEL, 0);
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);

        running = false;
        ramping = false;
        return;
    }

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    if (!running) {
        running = true;
        ramping = true;
        rampStart = millis();
    }

    int targetPWM = moving ? 40 : speed;
    targetPWM = constrain(targetPWM, 0, MAX_PWM);

    unsigned long t = millis() - rampStart;

    if (ramping && t < 3000) {
        int pwm = map(
            t, 0, 3000,
            FAN_SOFTSTART_PWM,
            targetPWM
        );

        pwm = constrain(pwm, 0, MAX_PWM);

        ledcWrite(FAN_PWM_CHANNEL, pwm);
        return;
    }

    ramping = false;

    ledcWrite(FAN_PWM_CHANNEL, targetPWM);
}