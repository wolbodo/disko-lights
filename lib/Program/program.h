#pragma once
#include "FastLED.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "RGBW.h"

class Program {
  public:

  virtual void tick();
  virtual void setConfig(JsonObject);
  virtual void getConfig(JsonObject);
  
};


float positive(float value) {
	return value > 0 ? value : 0;
}

