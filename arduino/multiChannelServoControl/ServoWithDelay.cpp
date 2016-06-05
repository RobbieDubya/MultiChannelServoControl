#include "ServoWithDelay.h"

ServoWithDelay::ServoWithDelay() :
    mPositionA(45),
    mPositionB(135),
    mMode(0),
    mTransitionDelayMS(2000),
    mLastButtonPressMS(0),
    mAnimationStartValue(0),
    mAnimationStartTime(0),
    mAnimationEndTime(0),
    mAnimationEndValue(0),
    mServoAt(0),
    mDown(false) {
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
        // For initial entry.
        if (mLastButtonPressMS == 0) {
            mLastButtonPressMS = now;
        }

        long diff = now - mLastButtonPressMS;

        if (diff < 0) {
            diff = -diff;
        }        
        if (!mDown && diff > 10) {
            mDown = true;
            mLastButtonPressMS = now;

            digitalWrite(mMode == 0 ? mLedAPin : mLedBPin, 0);

            mMode ^= 1;

            // Serial.print("AS:");
            // Serial.println(mServoAt);
            mAnimationStartValue = mServoAt;
            // TODO: for reversals - this should adapt the animation duration...
            mAnimationEndValue = mMode == 0 ? mPositionA : mPositionB;
            // Serial.print("AE:");
            // Serial.println(mAnimationEndValue);
            mAnimationStartTime = now;
            mAnimationEndTime = now + mTransitionDelayMS;

        }
    } else if (buttonState == 1) {
        if (now - mLastButtonPressMS > 50) {
            mDown = false;
        }
    }

    if (mAnimationStartTime != 0) {
        if (now >= mAnimationEndTime) {
            setServo(mAnimationEndValue);
            mAnimationStartTime = mAnimationEndTime = 0;
            digitalWrite(mMode == 0 ? mLedAPin : mLedBPin, 1);
        } else {
            digitalWrite(mMode == 0 ? mLedAPin : mLedBPin, now% 500 > 250);

            long animationRange = mAnimationEndValue - mAnimationStartValue;
            long delta = now - mAnimationStartTime;
            if (delta < 0) {
                setServo(mAnimationEndValue);
                mAnimationStartTime = mAnimationEndTime = 0;
            } else {
                long progress = animationRange * delta / mTransitionDelayMS;
                setServo(mAnimationStartValue + (int)(progress));
            }
        }   
    }
}

void ServoWithDelay::setServo(byte value) {
    // Serial.print(value);
    // Serial.println();

    if (mServoAt != value) {
        mServoAt = value;
        mServo.write(value);
    }
}

void ServoWithDelay::setTransitionDelayMS(int value) {
    mTransitionDelayMS = value;
}

void ServoWithDelay::setPositions(byte a, byte b) {
    mPositionA = a;
    mPositionB = b;
}