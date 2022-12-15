#pragma once

#include <graphlib.h>

struct Ledgrid : public GraphicsPrimitives {
    enum {
        // 6 x 4 tiles
        PANEL_WIDTH = 6,   
        PANEL_HEIGHT = 4,  

        // size of each tile
        TILE_WIDTH = 4,
        TILE_HEIGHT = 4,

        // spacing between the tiles.
        Hspace = 3,
        Vspace = 3,
    };
    CRGBArray<TILE_HEIGHT*TILE_WIDTH*PANEL_WIDTH*PANEL_HEIGHT> leds;

    int  nrleds() { return leds.size(); }

    int16_t  width() override { return PANEL_WIDTH*(TILE_WIDTH+Vspace); }
    int16_t  height() override { return PANEL_HEIGHT*(TILE_HEIGHT+Hspace); }

     // Convert x,y led coordinate to the lednr on the led-string
    int xy2i(int x, int y)
    {
        // calculate tile coordinate
        int tx = x / (TILE_WIDTH+Hspace);   // 0..PANEL_WIDTH-1
        int ty = y / (TILE_HEIGHT+Vspace);  // 0..PANEL_HEIGHT-1

        if ((ty%2) == 0) {
            // flip even rows
            tx = PANEL_WIDTH-1-tx;
        }

        // calculate subcoordinate of led within the tile.
        int _x = x % (TILE_WIDTH+Hspace);    // 0..TILE_WIDTH+Hspace-1
        int _y = y % (TILE_HEIGHT+Vspace);   // 0..TILE_HEIGHT+Vspace-1
        if (_y>=TILE_HEIGHT) return -1;   // spacing
        if (_x>=TILE_WIDTH) return -1;    // spacing
        if (_y%2) {
            // flip odd rows
            _x = TILE_WIDTH-1-_x;
        }

        // calculate tile nr
        int tilenr = PANEL_WIDTH*ty+tx;
        // calculate lednr within the tile
        int lednr = TILE_WIDTH*_y+_x;

        // combine
        return tilenr*TILE_WIDTH*TILE_HEIGHT + lednr;
    }

    void drawPixel(int16_t x, int16_t y, CRGB color) override
    {
        int i = xy2i(x, y);
        if (i>=0)
            leds[i] = color;
    }


    // should turn on all pixels in a single tile
    // x = 0 .. PANEL_WIDTH-1
    // y = 0 .. PANEL_HEIGHT-1
    void paintTile(int x, int y, CRGB color)
    {
        for (int dx = 0 ; dx<TILE_WIDTH ; dx++)
        for (int dy = 0 ; dy<TILE_HEIGHT ; dy++)
            drawPixel(x*(TILE_WIDTH+Hspace)+dx, y*(TILE_HEIGHT+Vspace)+dy, color);
    }

    // should turn on one pixel on all tiles
    // x = 0 .. TILE_WIDTH-1
    // y = 0 .. TILE_HEIGHT-1
    void tilePixel(int x, int y, CRGB color)
    {
        for (int dx = 0 ; dx<PANEL_WIDTH ; dx++)
        for (int dy = 0 ; dy<PANEL_HEIGHT ; dy++)
            drawPixel(x+dx*(TILE_WIDTH+Hspace), y+dy*(TILE_HEIGHT+Vspace), color);
    }

    // fill entire panel with the same color
    void fill(CRGB color)
    {
        for (int y = 0 ; y<height() ; y++)
        for (int x = 0 ; x<width() ; x++)
            drawPixel(x, y, color);
    }

    void fill_gradient_horizontal( uint16_t x0, uint16_t y0, CHSV startcolor,
                    uint16_t x1, uint16_t y1,   CHSV endcolor,
                    TGradientDirectionCode directionCode  = SHORTEST_HUES ) {
        // if the points are in the wrong order, straighten them
        if( x1 < x0 ) {
            uint16_t t = x1;
            CHSV tc = endcolor;
            endcolor = startcolor;
            x1 = x0;
            x0 = t;
            startcolor = tc;
        }
    
        // If we're fading toward black (val=0) or white (sat=0),
        // then set the endhue to the starthue.
        // This lets us ramp smoothly to black or white, regardless
        // of what 'hue' was set in the endcolor (since it doesn't matter)
        if( endcolor.value == 0 || endcolor.saturation == 0) {
            endcolor.hue = startcolor.hue;
        }
    
        // Similarly, if we're fading in from black (val=0) or white (sat=0)
        // then set the starthue to the endhue.
        // This lets us ramp smoothly up from black or white, regardless
        // of what 'hue' was set in the startcolor (since it doesn't matter)
        if( startcolor.value == 0 || startcolor.saturation == 0) {
            startcolor.hue = endcolor.hue;
        }
    
        saccum87 huedistance87;
        saccum87 satdistance87;
        saccum87 valdistance87;
    
        satdistance87 = (endcolor.sat - startcolor.sat) << 7;
        valdistance87 = (endcolor.val - startcolor.val) << 7;
    
        uint8_t huedelta8 = endcolor.hue - startcolor.hue;
    
        if( directionCode == SHORTEST_HUES ) {
            directionCode = FORWARD_HUES;
            if( huedelta8 > 127) {
                directionCode = BACKWARD_HUES;
            }
        }
    
        if( directionCode == LONGEST_HUES ) {
            directionCode = FORWARD_HUES;
            if( huedelta8 < 128) {
                directionCode = BACKWARD_HUES;
            }
        }
    
        if( directionCode == FORWARD_HUES) {
            huedistance87 = huedelta8 << 7;
        }
        else /* directionCode == BACKWARD_HUES */
        {
            huedistance87 = (uint8_t)(256 - huedelta8) << 7;
            huedistance87 = -huedistance87;
        }
    
        uint16_t pixeldistance = x1 - x0;
        int16_t divisor = pixeldistance ? pixeldistance : 1;
    
        saccum87 huedelta87 = huedistance87 / divisor;
        saccum87 satdelta87 = satdistance87 / divisor;
        saccum87 valdelta87 = valdistance87 / divisor;
    
        huedelta87 *= 2;
        satdelta87 *= 2;
        valdelta87 *= 2;
    
        accum88 hue88 = startcolor.hue << 8;
        accum88 sat88 = startcolor.sat << 8;
        accum88 val88 = startcolor.val << 8;
        for( uint16_t x = x0; x <= x1; ++x) {
            for (uint16_t y=y0; y<=y1; y++) {
                drawPixel(x, y, CHSV( hue88 >> 8, sat88 >> 8, val88 >> 8));
            }
            hue88 += huedelta87;
            sat88 += satdelta87;
            val88 += valdelta87;
        }
    }
};
