#pragma once

#include "program.h"
#include "ledgrid.h"

class Sjoerd  : public Program { 
public:
    void tick(Ledgrid&matrix)
    {
        matrix.fill(CHSV(millis() / 100, 150, 100));
        matrix.drawCircle(
            matrix.width(), matrix.height(), (millis()/10) % matrix.height,
            CHSV(millis()/250, 255, 255)
        );
    }
};
      
      
