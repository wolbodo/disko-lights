#include <ESP32Encoder.h>
#include "FastLED.h"
#include "display.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "Wolbodo";
const char* password = "darknetwork";

#define ROTARY_1 12
#define ROTARY_2 14

#define GRID_WIDTH 4 * 6
#define GRID_HEIGHT 4 * 4
#define NUM_LEDS GRID_HEIGHT * GRID_WIDTH

#define LED_TYPE   WS2813
#define COLOR_ORDER   GRB
#define DATA_PIN        13
#define VOLTS          5
#define MAX_MA       4000

Display display = Display(17, 16, 4, 2, 21, 19, 18, 5, 22, 23);
ESP32Encoder encoder;


class Matrix {
  private:
    int width;
    int height;

  public:
    CRGBArray<NUM_LEDS> leds;

    Matrix(int width, int height) {
      this->width = width;
      this->height = height;
    }

    void draw() {
      int time = millis();
      for (int i = 0; i < NUM_LEDS; i++) {
        bool on = (i + (time / 60)) % 100 < 50;
        leds[i].setHSV((time / 10 + i/10) % 255, on ? 255 : sin8(time/100+i)/2, scale8(sin8(-time / 50 + i/5), 200) + 55);
      }
    };

    void drawHorizontalLine(int y, CRGB color) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        // tile
        int tile_y = y / 4;
        int tile_x = x / 4;

        int tile;
        int i;

        if (tile_y % 2) { // odd
          tile = tile_y + 3 - tile_x;
        } else { // even
          tile = tile_y * 4 + tile_x;
        }
        i = tile * 16;

        int _x = x % 4;
        int _y = y % 4;
        if (_y % 2) {
          i += _y * 4 + 3 - _x;
        } else {
          i += _y * 4 + _x;
        }

        this->leds[i] = &color;
      }
    };
};

Matrix matrix = Matrix(GRID_WIDTH, GRID_HEIGHT);

void setup()
{

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  display.setup();

	ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachFullQuad(ROTARY_1, ROTARY_2);
  encoder.setCount(40);

  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(matrix.leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);

}

int _encoderCount;
void loop() {
  ArduinoOTA.handle();

  int encoderCount = encoder.getCount() / 2;
  display.show(encoderCount % 100);
  // display.show(millis() / encoderCount % 100);

  // if (encoderCount != _encoderCount) {
  //   Serial.println("Encoder = " + String((int32_t)encoderCount));
  // }
  // _encoderCount = encoderCount;

    // for (CRGB &pixel : leds)
    // {
    //   pixel.setHSV((time / 10) % 255, sin8(time / 33), sin8(time / 70));
    // }

  matrix.draw();

  // matrix.drawHorizontalLine((millis() / 100) % GRID_HEIGHT, CRGB::Black);

  matrix.drawHorizontalLine(7, CRGB::Black);


  FastLED.show();
}
