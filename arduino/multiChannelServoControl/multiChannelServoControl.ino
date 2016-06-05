#include <Servo.h>
#include "ServoWithDelay.h"

const byte SERVO_COUNT = 1;

ServoWithDelay servos[SERVO_COUNT];

// 6 + 7 LED
// 8  = switch
// 9 = servo

void setup() {
    servos[0].attach(9,6,7,8);
    // servos[1].attach(5,6,7,8);
    // Serial.begin(9600);
}

void loop() {
    unsigned long now = millis();

    for (byte i=0; i < SERVO_COUNT; ++i) {
        servos[i].tick(now);
    }


}
