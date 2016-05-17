#include "ServoWithDelay.h"

ServoWithDelay::ServoWithDelay() :
    positionA(45),
    positionB(135),
    mode(0),
    transitionDelayMS(50),
    lastButtonPressMS(0) {
}

void ServoWithDelay::attach(byte servoPin, byte ledAPin, byte ledBPin, byte buttonPin, bool setPinDirections) {
    mServoPin = servoPin;
    mLedAPin = ledAPin;
    mLedBPin = ledBPin;
    mButtonPin = buttonPin;

    if (setPinDirections) {
        pinMode(mServoPin,OUTPUT);
        pinMode(mLedAPin,OUTPUT);
        pinMode(mLedBPin,OUTPUT);
        pinMode(mButtonPin,INPUT);
        digitalWrite(mButtonPin,1);
    }

    servo.attach(mServoPin);

    servo.write(positionA);
    digitalWrite(mLedAPin,1);
    digitalWrite(mLedBPin,0);
}

void ServoWithDelay::tick(unsigned long now) {
    byte buttonState = digitalRead(mButtonPin);
    if (buttonState == 0) {
        long diff = now - lastButtonPressMS;
        if (diff&0x80000000) {
            diff = ~diff + 1;
        }

        if (diff > 50) {
            lastButtonPressMS = now;

            mode ^= 1;

            servo.write(mode==0 ? positionA : positionB);
        }
    }
}