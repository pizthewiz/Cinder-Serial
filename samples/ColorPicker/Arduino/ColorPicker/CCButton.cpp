//
//  CCButton.cpp
//  CinderPicker
//
//  Created by Jean-Pierre Mouilleseaux on 8 Jul 2015.
//  Copyright 2015 Chorded Constructions. All rights reserved.
//

#include "CCButton.h"

#define DEBOUNCE_DELAY 50

CCButton::CCButton(int pin, int identifier, bool pullUp) : mPin(pin), mIdentifier(identifier), mPullUp(pullUp), mState(LOW), mLastState(LOW), mLastTime(0) {}

void CCButton::setup() {
  pinMode(mPin, INPUT);
  if (mPullUp) {
    digitalWrite(mPin, HIGH);
  }
}

bool CCButton::read() {
  long currentTime = millis();
  bool didChange = false;

  int val = !mPullUp ? digitalRead(mPin) : (digitalRead(mPin) == HIGH ? LOW : HIGH);
  // read button and check if changed
  if (val != mLastState) {
    mLastTime = currentTime;
  }
  mLastState = val;

  // check if value has stabilized
  if ((currentTime - mLastTime) > DEBOUNCE_DELAY) {
    // check if the value has changed
    if (val != mState) {
      mState = val;
      didChange = true;
    }
  }

  return didChange;
}

int CCButton::state() {
  return mState;
}

int CCButton::identifier() {
  return mIdentifier;
}
