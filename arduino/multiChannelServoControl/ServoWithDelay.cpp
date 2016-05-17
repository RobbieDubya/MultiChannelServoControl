#include "ServoWithDelay.h"

ServoWithDelay::ServoWithDelay() :
    mPositionA(45),
    mPositionB(135),
    mMode(0),
    mTransitionDelayMS(500),
    mLastButtonPressMS(0),
    mAnimationStartValue(0),
    mAnimationStartTime(0),
    mAnimationEndTime(0),
    mAnimationEndValue(0),
    mServoAt(0) {
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

    mServo.attach(mServoPin);
    mServo.write(mPositionA);
    mServoAt = mPositionA;
    digitalWrite(mLedAPin,1);
    digitalWrite(mLedBPin,0);
}

void ServoWithDelay::tick(unsigned long now) {
    byte buttonState = digitalRead(mButtonPin);
    if (buttonState == 0) {
        long diff = now - mLastButtonPressMS;
        if (diff < 0) {
            diff = -diff;
        }        
        if (diff > 50) {
            mLastButtonPressMS = now;

            digitalWrite(mMode == 0 ? mLedAPin : mLedBPin, 0);

            mMode ^= 1;

            mAnimationStartValue = mServoAt;
            // TODO: for reversals - this should adapt the animation duration...
            mAnimationEndValue = mMode == 0 ? mPositionA : mPositionB;
            mAnimationStartTime = now;
            mAnimationEndTime = now + mTransitionDelayMS;
        }
    }

    if (mAnimationStartTime != 0) {
        if (now >= mAnimationEndTime) {
            setServo(mAnimationEndValue);
            mAnimationStartTime = mAnimationEndTime = 0;
            digitalWrite(mMode == 0 ? mLedAPin : mLedBPin, 1);
        } else {
            digitalWrite(mMode == 0 ? mLedAPin : mLedBPin, now%100 > 50);

            long animationRange = mAnimationEndValue - mAnimationStartValue;
            long delta = now - mAnimationStartTime;
            if (delta < 0) {
                setServo(mAnimationEndValue);
                mAnimationStartTime = mAnimationEndTime = 0;
            } else {
                long temp = delta<<8 / mTransitionDelayMS * animationRange;
                temp >>=8;
                setServo(mAnimationStartValue + (int)(temp));
            }
        }   
    }
}

void ServoWithDelay::setServo(byte value) {
    if (mServoAt != value) {
        mServoAt = value;
        mServo.write(value);
    }
}