#pragma once

#include "program.h"
#include "FastLED.h"

class ColorSweep : public Program {
    int count;
public:
    void tick(Ledgrid&matrix);
};
