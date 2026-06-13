#pragma once

#include <Arduino.h>
#include <Stepper.h>

class StepperTurner {
public:
    StepperTurner(
        uint8_t i1,
        uint8_t i2,
        uint8_t i3,
        uint8_t i4
    );

    void begin();
    void update();

    void start();
    void stop();

    bool isMoving() const;

private:
    void releaseStepper();

    Stepper stepper;

    uint8_t in1;
    uint8_t in2;
    uint8_t in3;
    uint8_t in4;

    bool active = false;
    bool movingUp = true;

    int swingsDone = 0;
};