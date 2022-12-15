#pragma once

#include "program.h"
#include "ledgrid.h"

class DrawRainbows1  : public Program { 
public:
    void tick(Ledgrid&matrix)
    {
        int time = millis();
        for (int i = 0; i < matrix.leds.size() ; i++) {
            bool on = (i + (time / 60)) % 100 < 50;
            matrix.leds[i].setHSV(
                (time / 10 + i/10) % 255,
                on ? 255 : sin8(time/100+i)/2,
                scale8(sin8(-time / 50 + i/5), 200) + 55
            );
        }
    }
};
      
      
