#include <ESP32Encoder.h>
#include "twinkle.h"
#include "display.h"

#define ROTARY_1 12
#define ROTARY_2 14

Display display = Display(17, 16, 4, 2, 21, 19, 18, 5, 22, 23);
ESP32Encoder encoder;

CRGBArray<NUM_LEDS> leds;

CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;


void setup()
{
  delay( 1000 ); //safety startup delay
	Serial.begin(9600);
  display.setup();

	ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachFullQuad(ROTARY_1, ROTARY_2);
  encoder.setCount(40);

  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);

  chooseNextColorPalette(gTargetPalette);
}

int _encoderCount;
void runTwinkles() {

  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) { 
    chooseNextColorPalette( gTargetPalette ); 
  }
  
  EVERY_N_MILLISECONDS( 10 ) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12);
  }

  drawTwinkles( gCurrentPalette, leds);
  
  FastLED.show();

}


void loop()
{
  int encoderCount = encoder.getCount() / 2;
  display.show(encoderCount % 100);
  // display.show(millis() / encoderCount % 100);

  // if (encoderCount != _encoderCount) {
  //   Serial.println("Encoder = " + String((int32_t)encoderCount));
  // }
  // _encoderCount = encoderCount;

  runTwinkles();
}
