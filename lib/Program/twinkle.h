#pragma once

#include "program.h"

class Point {
  public:
  int pos;
  uint8_t value;

  Point () {
    reset();
  }

  void reset() {

    pos = random(NUM_LEDS);
    value = random8(100);
  }
  
};
#define TWINKLE_POINTS 150
#define TWINKLE_REFRESH_TIME 50
class Twinkle : public Program {
  Point *points[TWINKLE_POINTS];
  int index = 0;
  uint8_t count = 30;

  public:
  Twinkle() {
    for (int i=0; i<TWINKLE_POINTS; i++) {
      points[i] = new Point();
    }
  }

  void setConfig(JsonObject data) {
    this->count = data["count"];
  }
  void getConfig(JsonObject data) {
    data["count"] = this->count;
  }
  
  void tick() {
    if (!count) return;

    EVERY_N_MILLIS(TWINKLE_REFRESH_TIME) {
      points[index++]->reset();

      index %= count;
    }
    
    for (int i=0; i<count; i++) {
      Point *point = points[i];
      if (point->value == 255) {
        point->value = random8(100);
      } else if (point->value > 5) {
        point->value -= 4;
      } else {
        point->value = 255;
      }
      
      leds[point->pos].white = point->value;
    }
  }
};
