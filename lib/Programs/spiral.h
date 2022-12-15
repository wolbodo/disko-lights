#pragma once

#include "program.h"
#include "ledgrid.h"
#include "FastLED.h"

class Spiral : public Program {
    int width = 1;
    int position = 0;
    int colorspeed = 300;
    int colorspread = 5;
    bool direction = false;
public:
    void setup() {
      width = random8(20)+1;
      colorspeed = random16(400)+50;
      colorspread = random8(20);
      direction = random() %2;

      Serial.printf("Written data %d, %d, %d, %d", width, colorspeed, colorspread, direction);
    }
    void tick(Ledgrid&matrix) {

        CRGB background = CHSV(millis()/colorspeed, 200, 50);

        matrix.fill(background);

        for (int x=0; x<matrix.width(); x+= width*2)
          matrix.drawRect(
            (x+position) % matrix.width(), 0, width, matrix.height(), 
            CHSV(millis()/(colorspeed/2) + colorspread*x, 255, 128)
          );
        EVERY_N_MILLISECONDS(100) {
          position = direction
            ? (++position > matrix.width() ? 0 : position )
            : (--position < 0 ? matrix.width()-1 : position );
        }

        EVERY_N_SECONDS(30) {
          setup();
        }
        //     for (i=0; i<PIXEL_COUNT; i++) {
        //     if (i == change_pixel && random8() < 25) {
        //         pixels[i].x = random16() % matrix.width();
        //         pixels[i].y = random16() % matrix.height();
        //         pixels[i].color = CHSV(hue, 255, 255);
        //         pixels[i].speed = random(4, 40);
        //         change_pixel = (change_pixel + 1) % PIXEL_COUNT;
        //     } else {
        //         if ( pixels[i].color.getLuma() == 0) {
        //         // pixels[i].color = CHSV(hue + 10, 170, 180);
        //             pixels[i].color = CHSV(hue, 255, 255);
        //         } else {
        //             pixels[i].color = pixels[i].color.fadeToBlackBy(pixels[i].speed);
        //         }
        //     }
        //     }
        // }

        // for (i=0; i<PIXEL_COUNT; i++) {
        //     matrix.drawPixel(pixels[i].x, pixels[i].y, pixels[i].color);
        // }
    }
};