#pragma once

#include "program.h"
#include "ledgrid.h"

class Sjoerd  : public Program { 
public:
    void tick(Ledgrid&matrix)
    {
        matrix.fill(CHSV(millis() / 100, 150, 100));
        matrix.drawCircle(
            matrix.width()/2, matrix.height()/2, (sin8(millis() / 10) / 10) + 1,
            CHSV(millis()/250, 255, 255)
        );
    }
};
      
      
