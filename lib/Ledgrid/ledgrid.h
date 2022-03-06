
struct Ledgrid {

    enum {
        // 6 x 4 tiles
        PANEL_WIDTH = 6,   
        PANEL_HEIGHT = 4,  

        // size of each tile
        TILE_WIDTH = 4,
        TILE_HEIGHT = 4,

        // spacing between the tiles.
        Hspace = 1,
        Vspace = 1,
    };
    CRGBArray<TILE_HEIGHT*TILE_WIDTH*PANEL_WIDTH*PANEL_HEIGHT> leds;

    int  nrleds() { return leds.size(); }

    int  width() { return PANEL_WIDTH*(TILE_WIDTH+1); }
    int  height() { return PANEL_HEIGHT*(TILE_HEIGHT+1); }
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

    void drawPixel(int x, int y, CRGB color)
    {
        int i = xy2i(x, y);
        if (i>=0)
            leds[i] = color;
    }
};
