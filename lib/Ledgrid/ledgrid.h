
class Ledgrid {

    enum {
        // 6 x 4 tiles
        GRIDWIDTH = 6,   
        GRIDHEIGHT = 4,  

        // size of each tile
        TILEWIDTH = 4,
        TILEHEIGHT = 4,

        // spacing between the tiles.
        Hspace = 1,
        Vspace = 1,
    };
    CRGBArray<TILEHEIGHT*TILEWIDTH*GRIDWIDTH*GRIDHEIGHT> leds;

    // Convert x,y led coordinate to the lednr on the led-string
    int xy2i(int x, int y)
    {
        // calculate tile coordinate
        int tx = x / (TILEWIDTH+Hspace);   // 0..GRIDWIDTH-1
        int ty = y / (TILEHEIGHT+Vspace);  // 0..GRIDHEIGHT-1

        if ((ty%2) == 0) {
            // flip even rows
            tx = GRIDWIDTH-1-tx;
        }

        // calculate subcoordinate of led within the tile.
        int _x = x % (TILEWIDTH+Hspace);    // 0..TILEWIDTH+Hspace-1
        int _y = y % (TILEHEIGHT+Vspace);   // 0..TILEHEIGHT+Vspace-1
        if (_y>=TILEHEIGHT) return -1;   // spacing
        if (_x>=TILEWIDTH) return -1;    // spacing
        if (_y%2) {
            // flip odd rows
            _x = TILEWIDTH-1-_x;
        }

        // calculate tile nr
        int tilenr = GRIDWIDTH*ty+tx;
        // calculate lednr within the tile
        int lednr = TILEWIDTH*_y+_x;

        // combine
        return tilenr*TILEWIDTH*TILEHEIGHT + lednr;
    }

    void draw(int x, int y, CRGB color)
    {
        int i = xy2i(x, y);
        if (i>=0)
            leds[i] = color;
    }
};
