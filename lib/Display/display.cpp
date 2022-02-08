#include <Arduino.h>
#include "display.h"

Display::Display(
      char pinA,
      char pinB,
      char pinC,
      char pinD,
      char pinE,
      char pinF,
      char pinG,
      char pinP,
      char drainA,
      char drainB
    ) {
      this->pinA = pinA;
      this->pinB = pinB;
      this->pinC = pinC;
      this->pinD = pinD;
      this->pinE = pinE;
      this->pinF = pinF;
      this->pinG = pinG;
      this->pinP = pinP;
      this->drainA = drainA;
      this->drainB = drainB;
};

void Display::setup() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinP, OUTPUT);

  pinMode(drainA, OUTPUT);
  pinMode(drainB, OUTPUT);
}

void Display::show(char value) {
  char digit;
  output(0);

  if (state) {
    state = false;
    digitalWrite(drainA, 0);
    digitalWrite(drainB, 1);
    digit = value % 10;
  } else {
    state = true;
    digitalWrite(drainA, 1);
    digitalWrite(drainB, 0);
    digit = (value / 10) % 10;
  }

  //  222
  // 0   3
  // 0   3
  //  bbb
  // 4   6
  // 4   6
  //  555 7

  switch (digit) {
    case 0:
      output(0b01111101);
      break;
    case 1:
      output(0b01001000);
      break;
    case 2:
      output(0b00111110);
      break;
    case 3:
      output(0b01101110);
      break;
    case 4:
      output(0b01001011);
      break;
    case 5:
      output(0b01100111);
      break;
    case 6:
      output(0b01110111);
      break;
    case 7:
      output(0b01001100);
      break;
    case 8:
      output(0b01111111);
      break;
    case 9:
      output(0b01101111);
      break;
  }
}
  
void Display::output(char bits) {
  digitalWrite(pinA, bits & 1);
  digitalWrite(pinB, bits >> 1 & 1);
  digitalWrite(pinC, bits >> 2 & 1);
  digitalWrite(pinD, bits >> 3 & 1);
  digitalWrite(pinE, bits >> 4 & 1);
  digitalWrite(pinF, bits >> 5 & 1);
  digitalWrite(pinG, bits >> 6 & 1);
  digitalWrite(pinP, bits >> 7 & 1);
}
