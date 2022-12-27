
#pragma once

#include "program.h"

class Tree : public Program {
  CHSVPalette16 palette = CHSVPalette16(CHSV(80,200,50), CHSV(130,255,100), CHSV(80,200,255));
  int speed1 = 1230;
  int speed2 = 136;
  int speed3 = 500;

  public:
  void setConfig(JsonObject data) {
    this->palette = CHSVPalette16(data["color1"], data["color2"], data["color3"]);
    this->speed1 = data["speed1"];
    this->speed2 = data["speed2"];
    this->speed3 = data["speed3"];
  }
  void getConfig(JsonObject data) {
    data["color1"] = this->palette[0];
    data["color2"] = this->palette[7];
    data["color3"] = this->palette[15];
    data["speed1"] = this->speed1;
    data["speed2"] = this->speed2;
    data["speed3"] = this->speed3;
  }
  
  void tick() {
    for (int i=0; i<NUM_LEDS; i++) {
      float fi = (float) i;
      float m = millis();
      float w1 = sin(fi/15 + m/speed1);
      float w2 = sin(fi/40 - m/speed2);

      float w3 = sin(fi/50 + m/speed3);

      // leds[i] = ColorFromPalette(palette, positive(w1 + w2)*255);
      leds[i] = ColorFromPalette(palette, positive(w1 + w2) * positive(w3) * 255);
    }

  }
};
