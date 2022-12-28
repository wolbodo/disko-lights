#pragma once

#include "program.h"
#include "FastLED.h"

class ColorSweep : public Program {
    int count;
public:
    void tick(Ledgrid&matrix)
    {
        count++;
        matrix.fill(CHSV(count, 255, ((count/8) % 2) * 255));
    }
};
