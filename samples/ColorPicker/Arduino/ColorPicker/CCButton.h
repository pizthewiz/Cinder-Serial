//
//  CCButton.h
//  CinderPicker
//
//  Created by Jean-Pierre Mouilleseaux on 8 Jul 2015.
//  Copyright 2015 Chorded Constructions. All rights reserved.
//

// simple debouncing button class adapted and abstracted from http://arduino.cc/en/Tutorial/Debounce

#ifndef CCBUTTON_H
#define CCBUTTON_H

#include <Arduino.h>

class CCButton {
public:
  CCButton(int pin, int identifier = 0, bool pullUp = false);

  void setup();
  int identifier();
  bool read();
  int state();

private:
  int mPin;
  int mIdentifier;
  bool mPullUp;
  int mState;
  int mLastState;
  long mLastTime;
};

#endif
