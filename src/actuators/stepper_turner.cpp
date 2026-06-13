#include "actuators/stepper_turner.h"
#include "config.h"

StepperTurner::StepperTurner(
    uint8_t i1,
    uint8_t i2,
    uint8_t i3,
    uint8_t i4
)
: stepper(STEPS_PER_REV, i1, i2, i3, i4),
  in1(i1),
  in2(i2),
  in3(i3),
  in4(i4)
{
}

void StepperTurner::begin() {

    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    pinMode(LIMIT_TOP, INPUT);
    pinMode(LIMIT_BOTTOM, INPUT);

    stepper.setSpeed(10);

    releaseStepper();
}

void StepperTurner::start() {

    active = true;
    movingUp = true;
    swingsDone = 0;

    Serial.println("TURN START");
}

void StepperTurner::stop() {

    active = false;

    releaseStepper();

    Serial.println("TURN STOP");
}

bool StepperTurner::isMoving() const {
    return active;
}

void StepperTurner::releaseStepper() {

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void StepperTurner::update() {

    if (!active)
        return;

    bool topHit = !digitalRead(LIMIT_TOP);
    bool bottomHit = !digitalRead(LIMIT_BOTTOM);

    if (movingUp) {

        if (topHit) {

            Serial.println("TOP HIT");

            releaseStepper();
            delay(500);

            movingUp = false;
            swingsDone++;
        }
        else {

            stepper.step(STEPS_PER_CHUNK);
        }
    }
    else {

        if (bottomHit) {

            Serial.println("BOTTOM HIT");

            releaseStepper();
            delay(500);

            movingUp = true;
            swingsDone++;
        }
        else {

            stepper.step(-STEPS_PER_CHUNK);
        }
    }

    if (swingsDone >= 2) {

        Serial.println("TURN COMPLETE");

        active = false;

        releaseStepper();
    }
}