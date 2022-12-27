
#pragma once

#include "program.h"
#include "FastLED.h" 
#include "RGBW.h"


class Wave : public Program {
  CRGBW *leds;
  CHSVPalette16 palette = CHSVPalette16(CHSV(0,200,255), CHSV(120,255,100), CHSV(220,255,100));

	int pos = 0;
	int width = 10;
	int speed = 10;
	int speed_i =0;
	int color_i =0;

	public:
  Wave(CRGBW *leds) {
    this->leds = leds;
    setup();
    pos = random(NUM_LEDS);
  }
	void setup() {
		pos = 0;
		width = random8(10) + 1;
		color_i = random8();
		speed = random8(width) + 1;
		speed_i = speed;
	}

	void tick() {
    float step = M_PI / width;

    for (int i=0; i<width; i++) {
      leds[i + pos % NUM_LEDS] = ColorFromPalette(palette, color_i, sin(i * step) * 255);
    }

		if (pos > NUM_LEDS) {
			setup();
		}

		if (--speed_i == 0) {
			pos ++;
			speed_i = speed;
		}
	}
};