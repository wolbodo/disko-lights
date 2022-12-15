#pragma once

#include "program.h"
#include "FastLED.h"

class Christmas : public Program {
    CHSV color1;
    fract8 color1blend = 0;
    bool color1direction = true;
    CHSV color2;
    fract8 color2blend = 0;
    bool color2direction = true;
public:
    void setup() {
      tickGradientColors();
    }
    void tickGradientColors() {
      color1 = blend(CHSV(0, 255, 255), CHSV(180, 255, 255), color1blend);
      color2 = blend(CHSV(120, 255, 255), CHSV(180, 255, 255), color2blend);

      color1direction ? color1blend++ : color1blend--;
      color1direction ^= color1blend == 0 || color1blend == 250;
      color2direction ? color2blend++ : color2blend--;
      color2direction ^= color2blend == 0 || color2blend == 150;
    }
    void tick(Ledgrid&matrix) {
      
      matrix.fill_gradient_horizontal(
        0, 0, color1,
        matrix.width()-1, matrix.height()-1, color2,
        FORWARD_HUES
      );

      EVERY_N_MILLIS(50) {
        tickGradientColors();
      }
    }
    
};
