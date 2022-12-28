#pragma once

#include "program.h"
#include "FastLED.h"

class ColorGradient : public Program {
    int count;
public:
    void tick(Ledgrid&matrix)
    {
        count ++;
        for (int y = 0 ; y<matrix.height() ; y++)
        for (int x = 0 ; x<matrix.width() ; x++)
        matrix.drawPixel(x, y, CHSV(count, y, ((count/8) % 2) * 255));
    }
};
