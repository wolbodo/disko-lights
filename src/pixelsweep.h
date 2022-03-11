#pragma once
class PixelSweep : public Program {
    enum { PANEL_SWEEP, PIXEL_SWEEP };
    int sweepstate = PANEL_SWEEP;
    enum { TURN_ON, TURN_OFF };
    int pixelstate = TURN_ON;
    int x = 0;
    int y = 0;
public:
    void tick(Ledgrid&matrix)
    {
        switch (sweepstate) {
        case PANEL_SWEEP:
            if (!panel_step(matrix)) {
                x = y = 0;
                pixelstate = TURN_ON;
                sweepstate = PIXEL_SWEEP;
            }
            break;

        case PIXEL_SWEEP:
            if (!pixel_step(matrix)) {
                x = y = 0;
                pixelstate = TURN_ON;
                sweepstate = PANEL_SWEEP;
            }
            break;
        }
    }

    bool panel_step(Ledgrid&matrix)
    {
        switch (pixelstate) {
        case TURN_ON:
            matrix.paintTile(x, y, CRGB(31*(x+1), 31*(y+1), 31*(x+y+1)));
            pixelstate = TURN_OFF;
            return true;
        case TURN_OFF:
            matrix.paintTile(x, y, CRGB(0, 0, 0));
            pixelstate = TURN_ON;
            return next_panel();
        }
    }
    bool next_panel()
    {
        if (++x == Ledgrid::PANEL_WIDTH) {
            x = 0;
            if (++y == Ledgrid::PANEL_HEIGHT) {
                y = 0;
                return false;
            }
        }
        return true;
    }

    bool pixel_step(Ledgrid&matrix)
    {
        switch (pixelstate) {
        case TURN_ON:
            matrix.tilePixel(x, y, CRGB(31*(x+1), 31*(y+1), 31*(x+y+1)));
            pixelstate = TURN_OFF;
            return true;
        case TURN_OFF:
            matrix.tilePixel(x, y, CRGB(0, 0, 0));
            pixelstate = TURN_ON;
            return next_panel();
        }
    }
    bool next_pixel()
    {
        if (++x == Ledgrid::TILE_WIDTH) {
            x = 0;
            if (++y == Ledgrid::TILE_HEIGHT) {
                y = 0;
                return false;
            }
        }
        return true;
    }

};
