#ifndef _SERVOWITHDELAY_H
#define _SERVOWITHDELAY_H

#include <arduino.h>
#include <inttypes.h>
#include <Servo.h>

class ServoWithDelay {
public:
    ServoWithDelay();

    void attach(byte servoPin, byte ledAPin, byte ledBPin, byte buttonPin, bool setPinDirections = true);
    void setPositions(byte a, byte b);
    void setTransitionDelayMS(int delay);

    void tick(unsigned long now);

private:
    byte mServoPin;
    byte mLedAPin;
    byte mLedBPin;
    byte mButtonPin;

    byte positionA;
    byte positionB;

    byte mode;

    Servo servo;

    int transitionDelayMS;

    unsigned long lastButtonPressMS;
};

#endif