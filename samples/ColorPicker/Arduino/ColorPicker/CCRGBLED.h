//
//  CCRGBLED.h
//  CinderPicker
//
//  Created by Jean-Pierre Mouilleseaux on 8 Jul 2015.
//  Copyright 2015 Chorded Constructions. All rights reserved.
//

#ifndef CCRGBLED_H
#define CCRGBLED_H

#include <Arduino.h>

class CCRGBLED {
public:
  CCRGBLED(int redPin, int greenPin, int bluePin, bool usesCommonAnode = false);

  void setup();
  void setColor(int red, int green, int blue);

private:
  int mRedPin;
  int mGreenPin;
  int mBluePin;
  bool mUsesCommonAnode;
};

#endif
