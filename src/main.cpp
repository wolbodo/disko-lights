#include <map>

#include "wifisetup.h"

#include "colorsweep.h"
#include "gradient.h"
#include "pixelsweep.h"
#include "rainbow1.h"
#include "rainbow2.h"
#include "spiral.h"
#include "spots.h"
#include "christmas.h"
#include "Sjoerd.h"
#include "Sjoerd2.h"
#include "scene.h"

#include "webserver.h"

#define LED_TYPE   WS2813

#define COLOR_ORDER   GRB
#define DATA_PIN       GPIO_NUM_12
#define VOLTS          5
#define MAX_MA       4000


Ledgrid matrix;

class MainPrograms : public MainControl {
    std::map<std::string, Program*> programs;
    Program *cur;
public:
    MainPrograms()
        : cur(nullptr)
    {
      programs["Christmas"] = new Christmas();
      programs["Spots"] = new Spots();
      programs["DrawRainbows2"] = new DrawRainbows2();
      programs["DrawRainbows1"] = new DrawRainbows1();
      programs["PixelSweep"] = new PixelSweep();
      programs["ColorSweep"] = new ColorSweep();
      programs["ColorGradient"] = new ColorGradient();
      programs["Sjoerd"] = new Sjoerd();
      programs["Sjoerd2"] = new Sjoerd2();
      programs["scene"] = new Scene();
      programs["Spiral"] = new Spiral();
    }

    bool setCurrentProgram(const std::string& name) override
    {
        auto i = programs.find(name);
        if (i == programs.end())
            return false;

        cur = i->second;
        return true;
    }
    std::vector<std::string> getPrograms() override
    {
        std::vector<std::string> v;
        for (auto kv : programs)
            v.emplace_back(kv.first);
        return v;
    }

    Program* current()
    {
        return cur;
    }

    void loop()
    {
      // readProgram();
        if (cur) {
            cur->tick(matrix);
            FastLED.show();
        }
    }
};


MainPrograms programs;

DiskoServer webserver(&programs);

void ledsSetup() {

  pinMode(DATA_PIN, OUTPUT);

  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(matrix.leds, matrix.nrleds())
    .setCorrection(TypicalLEDStrip);
  // updateBrightness();

}


void setup() {
  delay( 300 ); //safety startup delay

  Serial.begin(115200);
  while (!Serial);

  delay(200);

	Serial.println("Setting up wifi");
	wifiSetup();
	
	// Serial.println("Setting up server");
	webserver.setup();

	Serial.println("Setting up leds");
	ledsSetup();

    programs.setCurrentProgram("Christmas");
}


void loop(){
	wifiLoop();
	webserver.loop();

	programs.loop();
}
