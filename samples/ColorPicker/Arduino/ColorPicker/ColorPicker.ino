//
//  ColorPicker.ino
//  CinderPicker
//
//  Created by Jean-Pierre Mouilleseaux on 8 Jul 2015.
//  Copyright 2015 Chorded Constructions. All rights reserved.
//

#include "CCButton.h"
#include "CCRGBLED.h"

const int BUTTON_PIN = 2;
CCButton button = CCButton(BUTTON_PIN, 0);

const int RGBLED_PINS[3] = {11, 10, 9};
CCRGBLED led = CCRGBLED(RGBLED_PINS[0], RGBLED_PINS[1], RGBLED_PINS[2]);

int buffer[5];
int bufferIndex = 0;

void setup() {
  button.setup();

  led.setup();
  led.setColor(0, 0, 0);

  Serial.begin(115200);
}

void loop() {
  // serial input
  while (Serial.available() > 0) {
    buffer[bufferIndex] = Serial.read();
    bufferIndex++;

    // scan buffer for message when appropraitely large
    if (bufferIndex == 5) {
      if (buffer[bufferIndex-2] == '\r' && buffer[bufferIndex-1] == '\n') {
        led.setColor(buffer[bufferIndex-5], buffer[bufferIndex-4], buffer[bufferIndex-3]);
      }
      bufferIndex = 0;
    }
  }

  // hardware input
  if (button.read()) {
    Serial.write(button.state());
    Serial.write('\r');
    Serial.write('\n');
  }
}
