#include <Servo.h>
#include "ServoWithDelay.h"

const byte SERVO_COUNT = 2;

ServoWithDelay servos[SERVO_COUNT];

void setup() {
    servos[0].attach(1,2,3,4);
    servos[1].attach(5,6,7,8);
}

void loop() {
    unsigned long now = millis();

    for (byte i=0; i < SERVO_COUNT; ++i) {
        servos[i].tick(now);
    }


}
