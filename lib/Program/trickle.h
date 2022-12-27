
#pragma once

#include "program.h"

class Trickle : public Program {
  public:
  
  void tick() {

    for (float i=0; i<NUM_LEDS; i++) {
      float m = millis();
      float w1 = sin(i + m/100);
      float w2 = sin(i/20 - m/600) ;

      float w3 = sin(i/5 + m/350);

      leds[(int) i].white = positive(w1 + w2) * positive(w3) * 100;
    }

  }
};
