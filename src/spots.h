
#include "FastLED.h"

#define PIXEL_COUNT 50
#pragma once

struct pixel {
  char x;
  char y;
  CRGB color;
  CRGB initial;
  char speed;
};
class Spots : public Program {
    int count = 0;
    int change_pixel = 0;
    struct pixel pixels[PIXEL_COUNT];
public:
    void tick(Ledgrid&matrix) {
        int i;
        int hue = millis()/200;
        CRGB background = CHSV(hue + sin8(millis()/50), 200, 205);
    
        for (i=0; i<matrix.width(); i++) {
            matrix.drawFastVLine(i, 0, matrix.height(), CRGB(background).fadeToBlackBy(sin8(millis()/300 + i*40)));
        }

        EVERY_N_MILLISECONDS(20) {
            for (i=0; i<PIXEL_COUNT; i++) {
            if (i == change_pixel && random8() < 25) {
                pixels[i].x = random16() % matrix.width();
                pixels[i].y = random16() % matrix.height();
                pixels[i].initial = pixels[i].color = CHSV(hue, 255, 255);
                pixels[i].speed = random(4, 40);
                change_pixel = (change_pixel + 1) % PIXEL_COUNT;
            } else {
                if ( pixels[i].color.getLuma() == 0) {
                // pixels[i].color = CHSV(hue + 10, 170, 180);
                pixels[i].color = pixels[i].initial;
                } else {
                pixels[i].color = pixels[i].color.fadeToBlackBy(pixels[i].speed);
                }
            }
            }
        }

        for (i=0; i<PIXEL_COUNT; i++) {
            matrix.drawPixel(pixels[i].x, pixels[i].y, pixels[i].color);
        }

        FastLED.show();
    }
};