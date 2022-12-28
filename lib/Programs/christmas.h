#pragma once

#include "program.h"
#include "FastLED.h"

class Christmas : public Program {
    CHSVPalette32 palette;
    uint8_t color1Hue = 0;
    bool color1direction = true;
public:
    void setup() {
      tickGradientColors();
    }
    void tickGradientColors() {
      color1direction ? color1Hue++ : color1Hue--;
      color1direction ^= color1Hue == 0 || color1Hue == 250;

      palette = CHSVPalette32(CHSV(color1Hue, 255, 255), CHSV(color1Hue+120, 255, 255));
    }
    void tick(Ledgrid&matrix) {
      float ms = ((float) millis()) / 1000;
      for (int i=0; i<matrix.height(); i++) {
        uint8_t wave = (sin(((float) i/10) + ms)+1) * 10;

        matrix.fill_gradient_horizontal(
          0, i, ColorFromPalette(palette, wave),
          matrix.width()-1, i, ColorFromPalette(palette, 255-wave),
          FORWARD_HUES
        );

      }

      EVERY_N_MILLIS(50) {
        tickGradientColors();
      }
    }
    
};
