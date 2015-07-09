//
//  CCRGBLED.cpp
//  CinderPicker
//
//  Created by Jean-Pierre Mouilleseaux on 8 Jul 2015.
//  Copyright 2015 Chorded Constructions. All rights reserved.
//

#include "CCRGBLED.h"

CCRGBLED::CCRGBLED(int redPin, int greenPin, int bluePin, bool usesCommonAnode) : mRedPin(redPin), mGreenPin(greenPin), mBluePin(bluePin), mUsesCommonAnode(usesCommonAnode) {}

void CCRGBLED::setup() {
  pinMode(mRedPin, OUTPUT);
  pinMode(mGreenPin, OUTPUT);
  pinMode(mBluePin, OUTPUT);
}

void CCRGBLED::setColor(int red, int green, int blue) {
  if (mUsesCommonAnode) {
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  }
  analogWrite(mRedPin, red);
  analogWrite(mGreenPin, green);
  analogWrite(mBluePin, blue);
}
