#pragma once

#include <map>

#include "colorsweep.h"
#include "gradient.h"
#include "pixelsweep.h"
#include "rainbow1.h"
#include "rainbow2.h"
#include "spiral.h"
#include "spots.h"
#include "christmas.h"
#include "Sjoerd.h"

#define LED_TYPE   WS2813

#define COLOR_ORDER   GRB
#define DATA_PIN       GPIO_NUM_12
#define VOLTS          5
#define MAX_MA       4000

Ledgrid matrix;

const std::map<std::string, Program*> programs = {
  {"Christmas",  new Christmas()},
  {"Spots",  new Spots()},
  {"DrawRainbows2",  new DrawRainbows2()},
  {"DrawRainbows1",  new DrawRainbows1()},
  {"PixelSweep",  new PixelSweep()},
  {"ColorSweep",  new ColorSweep()},
  {"ColorGradient",  new ColorGradient()},
  {"Sjoerd", new Sjoerd()},
};
std::string currentProgram = "Christmas";

#define PROGRAM_COUNT programs.size()


void programSetup() {

  pinMode(DATA_PIN, OUTPUT);

  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(matrix.leds, matrix.nrleds())
    .setCorrection(TypicalLEDStrip);
  // updateBrightness();

}

void programLoop() {
  // readProgram();
  programs.at(currentProgram)->tick(matrix);
  
  FastLED.show();
}
