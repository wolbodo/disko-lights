#pragma once
class DrawRainbows2  : public Program { 
public:
    void tick(Ledgrid&matrix)
    {
        int time = millis();
        int x, y, i;
        for (y = 0; y < matrix.height() ; y++) {
            for (x=0; x < matrix.width() ; x++) {
                matrix.drawPixel(x, y, CHSV(x * 2 + time/20, 255, 255));
            }
        }

        x = (millis() / 100 ) % matrix.width();
        for (y=0; y < matrix.height(); y++) {
            for (i=0; i<3; i++) {
                matrix.drawPixel(x+i, y, CRGB::BlueViolet);
            }
        }

        y = (millis() / 156) % matrix.height();
        for (x=0; x < matrix.width(); x++) {
            for (i=0; i<2; i++) {
                matrix.drawPixel(x+i, y, CRGB::WhiteSmoke);
            }
        }
    }
};
