#pragma once

#include "program.h"
#include "FastLED.h"
#include "eventtimer.h"

class PixelSweep : public Program {
    enum { PANEL_SWEEP, PIXEL_SWEEP };
    int sweepstate = PANEL_SWEEP;
    enum { TURN_ON, TURN_OFF };
    int pixelstate = TURN_ON;
    int x = 0;
    int y = 0;

    EventTimer _t = 100;
public:
    void tick(Ledgrid&matrix);
    bool panel_step(Ledgrid&matrix);
    bool next_panel();
    bool pixel_step(Ledgrid&matrix);
    bool next_pixel();
};
