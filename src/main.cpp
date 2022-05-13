#include <ESP32Encoder.h>

#include "SPI.h"
#define FASTLED_ALL_PINS_HARDWARE_SPI
#include "FastLED.h"
#include "display.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ledgrid.h>
#include "program.h"

#include <eventtimer.h>

#include "colorsweep.h"
#include "gradient.h"
#include "pixelsweep.h"
#include "rainbow1.h"
#include "rainbow2.h"
#include "spots.h"

const char* ssid = "Wolbodo";
const char* password = "darknetwork";

/*
#define PANEL_WIDTH 4
#define PANEL_HEIGHT 4
#define TILE_WIDTH 6
#define TILE_HEIGHT 4
#define GRID_WIDTH PANEL_WIDTH * TILE_WIDTH
#define GRID_HEIGHT PANEL_HEIGHT * TILE_HEIGHT
#define TILE_HEIGHT 4
#define NUM_LEDS TILE_HEIGHT * PANEL_HEIGHT * TILE_WIDTH * PANEL_WIDTH
*/
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

Ledgrid matrix;

Program *programs[] = {
    new DrawRainbows2(),
    new DrawRainbows1(),
    new PixelSweep(),
    new ColorSweep(),
    new ColorGradient(),
    new Spots(),
};
enum { PROGRAM_COUNT = sizeof(programs)/sizeof(programs[0]) };


void updateBrightness() {
  int pot_value = ((float) analogRead(POT_INPUT_PIN)) / 16.062745098039215;
  FastLED.setBrightness(pot_value);
}

void wifiScan() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

}

void setup() {

  Serial.begin(115200);
  Serial.println("Booting");

  display.setup();

  pinMode(POWER_BUTTON_PIN, INPUT_PULLDOWN);

  pinMode(POT_INPUT_PIN, INPUT);
  pinMode(POT_LED_PIN, OUTPUT);

  pinMode(ROTARY_PIN_BUTTON, INPUT_PULLDOWN);

  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachFullQuad(ROTARY_PIN_A, ROTARY_PIN_B);

  WiFi.mode(WIFI_STA);

  wifiScan();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    Serial.println("Connection Failed! Retrying...");

    for (int i=0; i < 50; i++) {
      display.showWait(i/2);

      delay(10);
    }

    // ESP.restart();
  }

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

  FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(matrix.leds, matrix.nrleds())
    .setCorrection(TypicalLEDStrip);
  updateBrightness();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
}

EventTimer t_brightness(50);

void loop() {
  // Control the brightness of the grid.
  if (t_brightness.ready())
    updateBrightness();

  // The powerbutton triggers the deep sleep, being woken up by interrupts
  int powerbutton = digitalRead(POWER_BUTTON_PIN);
  if (!powerbutton) {
    FastLED.clear(true);

    esp_deep_sleep_start();
  }


//  int encoderButton = digitalRead(ROTARY_PIN_BUTTON);
//  digitalWrite(POT_LED_PIN, encoderButton);
  int programId = (encoder.getCount() / 2) % PROGRAM_COUNT;

  display.showNumber(programId);

  programs[programId]->tick(matrix);

  FastLED.show();
  ArduinoOTA.handle();
}
