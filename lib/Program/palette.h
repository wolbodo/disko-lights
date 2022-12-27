#pragma once
#include "program.h"


class Palette : public Program {
  CHSVPalette16 palette = CHSVPalette16(CHSV(0,200,255), CHSV(120,255,100), CHSV(220,255,100));

  public:

  void tick() {
    for (int i=0; i<NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(palette, i);
    }
  }
};
