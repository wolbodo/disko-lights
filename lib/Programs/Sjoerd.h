#pragma once

#include "program.h"
#include "ledgrid.h"

class Sjoerd  : public Program { 
public:
    void tick(Ledgrid&matrix)
    {
        matrix.fill(CHSV(millis() / 100, 150, 100));
        matrix.fillCircle(
            matrix.width()/2, matrix.height()/2, sin8(millis() / 10) / 20,
            CHSV(millis()/250, 255, 255)
        );
        matrix.drawCircle(
            matrix.width()/2, matrix.height()/2, sin8(millis() / 10) / 20,
            CHSV(millis()/320, 255, 255)
        );
        matrix.drawCircle(
            matrix.width()/2, matrix.height()/2, sin8(millis() / 40) / 20,
            CHSV(millis()/120, 255, 255)
        );
        matrix.drawCircle(
            matrix.width()/2, matrix.height()/2, sin8(millis() / 5) / 20,
            CHSV(millis()/50, 255, 255)
        );
    }
};
      
      
