#pragma once

#include "program.h"
#include "FastLED.h"

class ColorGradient : public Program {
    int count;
public:
    void tick(Ledgrid&matrix);
};
