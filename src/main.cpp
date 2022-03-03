#include <ESP32Encoder.h>

 #include "SPI.h"
#define FASTLED_ALL_PINS_HARDWARE_SPI
#include "FastLED.h"
#include "display.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


const char* ssid = "Wolbodo";
const char* password = "darknetwork";

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  1        /* Time ESP32 will go to sleep (in seconds) */

#define TILE_WIDTH 4
#define TILE_HEIGHT 4
#define GRID_WIDTH 6
#define GRID_HEIGHT 4
#define NUM_LEDS GRID_HEIGHT * TILE_HEIGHT * GRID_WIDTH * TILE_WIDTH

#define LED_TYPE   WS2813
#define COLOR_ORDER   GRB
#define DATA_PIN       GPIO_NUM_23
#define VOLTS          5
#define MAX_MA       4000

#define SEGMENT_PINS GPIO_NUM_32, GPIO_NUM_26, GPIO_NUM_13, GPIO_NUM_12, GPIO_NUM_14, GPIO_NUM_27, GPIO_NUM_25, GPIO_NUM_33, GPIO_NUM_2, GPIO_NUM_15

#define POT_INPUT_PIN GPIO_NUM_36
#define POT_LED_PIN GPIO_NUM_16

#define ROTARY_PIN_A GPIO_NUM_34
#define ROTARY_PIN_B GPIO_NUM_35
#define ROTARY_PIN_BUTTON GPIO_NUM_19

#define POWER_BUTTON_PIN GPIO_NUM_4


Display display = Display(SEGMENT_PINS);
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
        int tile_y = y / TILE_HEIGHT;
        int tile_x = x / TILE_WIDTH;

        int tile;
        int i;

        if (tile_y % 2) { // odd
          tile_x = tile_x;
        } else { // even
          tile_x = GRID_WIDTH - tile_x - 1;
        }

        tile = tile_y * GRID_WIDTH + tile_x;
        i = tile * TILE_HEIGHT * TILE_WIDTH;

        int _x = x % TILE_WIDTH;
        int _y = y % TILE_HEIGHT;
        if (_y % 2) {
          i += _y*TILE_WIDTH + TILE_WIDTH - _x - 1;
        }
        else
        {
          i += _y*TILE_WIDTH + _x;
        }

        this->leds[i] = color;
      }
    };
};

Matrix matrix = Matrix(GRID_WIDTH, GRID_HEIGHT);

void setBrightness() {
  int pot_value = ((float) analogRead(POT_INPUT_PIN)) / 16.062745098039215;
  FastLED.setBrightness(pot_value);
}

void setup()
{

  Serial.begin(115200);
  Serial.println("Booting");
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //   Serial.println("Connection Failed! Rebooting...");
  //   delay(5000);
  //   ESP.restart();
  // }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  // ArduinoOTA
  //   .onStart([]() {
  //     String type;
  //     if (ArduinoOTA.getCommand() == U_FLASH)
  //       type = "sketch";
  //     else // U_SPIFFS
  //       type = "filesystem";

  //     // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  //     Serial.println("Start updating " + type);
  //   })
  //   .onEnd([]() {
  //     Serial.println("\nEnd");
  //   })
  //   .onProgress([](unsigned int progress, unsigned int total) {
  //     Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  //   })
  //   .onError([](ota_error_t error) {
  //     Serial.printf("Error[%u]: ", error);
  //     if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  //     else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  //     else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  //     else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  //     else if (error == OTA_END_ERROR) Serial.println("End Failed");
  //   });

  // ArduinoOTA.begin();

  // Serial.println("Ready");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  display.setup();

  pinMode(POWER_BUTTON_PIN, INPUT_PULLDOWN);

  pinMode(POT_INPUT_PIN, INPUT);
  pinMode(POT_LED_PIN, OUTPUT);

  pinMode(ROTARY_PIN_BUTTON, INPUT_PULLDOWN);
  // pinMode(ROTARY_PIN_A, INPUT_PULLUP);
  // pinMode(ROTARY_PIN_B, INPUT_PULLUP);

	ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachFullQuad(ROTARY_PIN_A, ROTARY_PIN_B);
  encoder.setCount(40);

  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(matrix.leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);
  setBrightness();


  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
}


int _encoderCount;
void loop() {
  int powerbutton = digitalRead(POWER_BUTTON_PIN);


  if (!powerbutton) {
    FastLED.clear(true);

    esp_deep_sleep_start();
  }

  // ArduinoOTA.handle();

  int encoderCount = encoder.getCount() / 2;
  int encoderButton = digitalRead(ROTARY_PIN_BUTTON);

  if (millis() / 100 % 2) {
    setBrightness();
  }

  display.showNumber(encoderCount % 100);

  digitalWrite(POT_LED_PIN, encoderButton);
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

  // matrix.drawHorizontalLine((millis() / 1000) % GRID_HEIGHT, CRGB::Black);
  // matrix.drawHorizontalLine((millis() / 1000)+1 % GRID_HEIGHT, CRGB::Black);

  // matrix.drawHorizontalLine(7, CRGB::Black);


  FastLED.show();
}
