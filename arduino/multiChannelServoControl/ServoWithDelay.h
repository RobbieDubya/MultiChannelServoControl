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
    void setServo(byte value);

    byte mServoPin;
    byte mLedAPin;
    byte mLedBPin;
    byte mButtonPin;

    byte mPositionA;
    byte mPositionB;

    byte mAnimationStartValue;
    unsigned long mAnimationStartTime;
    unsigned long mAnimationEndTime;
    byte mAnimationEndValue;
    byte mServoAt;

    byte mMode;

    Servo mServo;

    int mTransitionDelayMS;

    unsigned long mLastButtonPressMS;

    bool mDown;
};

#endif