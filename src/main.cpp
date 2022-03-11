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

#define PROGRAM_COUNT 5


Display display = Display(SEGMENT_PINS);
ESP32Encoder encoder;
int programId = 0;

Ledgrid matrix;


void drawRainbows1() {
  int time = millis();
  for (int i = 0; i < matrix.leds.size() ; i++) {
    bool on = (i + (time / 60)) % 100 < 50;
    matrix.leds[i].setHSV(
      (time / 10 + i/10) % 255,
      on ? 255 : sin8(time/100+i)/2,
      scale8(sin8(-time / 50 + i/5), 200) + 55
    );
  }
};

void drawRainbows2() {
  int time = millis();
  int x, y, i;
  for (y = 0; y < matrix.height() ; y++) {
    for (x=0; x < matrix.width() ; x++) {
      matrix.drawPixel(x, y, CHSV(x * 2 + time/20, 255, 255));
    }
  }

  x = (millis() / 100 ) % matrix.width();
  for (y=0; y < matrix.height(); y++) {
    for (i=0; i<3; i++) {
      matrix.drawPixel(x+i, y, CRGB::BlueViolet);
    }
  }

  y = (millis() / 156) % matrix.height();
  for (x=0; x < matrix.width(); x++) {
    for (i=0; i<2; i++) {
      matrix.drawPixel(x+i, y, CRGB::WhiteSmoke);
    }
  }
}

void setBrightness() {
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
  setBrightness();

  programId = 0;

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
}

void PixelSweep()
{
    for (int y=0 ; y<Ledgrid::PANEL_HEIGHT ; y++ )
    for (int x=0 ; x<Ledgrid::PANEL_WIDTH ; x++)
    {
        matrix.paintTile(x, y, CRGB(31*(x+1), 31*(y+1), 31*(x+y+1)));
        FastLED.show();
        delay(200);
        matrix.paintTile(x, y, CRGB(0, 0, 0));
        FastLED.show();
    }

    for (int y=0 ; y<Ledgrid::TILE_HEIGHT; y++)
    for (int x=0 ; x<Ledgrid::TILE_WIDTH ; x++)
    {
        matrix.tilePixel(x, y, CRGB(31*(x+1), 31*(y+1), 31*(x+y+1)));
        FastLED.show();
        delay(200);
        matrix.tilePixel(x, y, CRGB(0, 0, 0));
        FastLED.show();
    }
}

byte count = 0;
void runProgram() {
  int time = millis();
  switch (programId) {

    case 0:
      drawRainbows2(); break;
    case 1:
      count ++;
      for (int i = 0; i < matrix.nrleds() ; i++) {
        matrix.leds[i] = CHSV(count, 255, ((count/8) % 2) * 255);
      }
      break;

    case 2:
      drawRainbows1();
      break;
    case 3:
      count ++;
      for (int i = 0; i < matrix.nrleds() ; i++) {
        byte row = i / (matrix.width() * Ledgrid::PANEL_HEIGHT) + time / 50 % 2;
        matrix.leds[i] = CHSV(count, row, ((count/8) % 2) * 255);
      }
      break;
    case 4:
      PixelSweep();
      break;

    default:
      PixelSweep();
      //drawRainbows1();
      break;
      

    // matrix.drawHorizontalLine((millis() / 1000)+1 % TILE_HEIGHT, CRGB::Black);

    // matrix.drawHorizontalLine(7, CRGB::Black);
  }
  // matrix.drawHorizontalLine((millis() / 1000) % TILE_HEIGHT, CRGB::Black);
}


int _encoderCount;
void loop() {
  int powerbutton = digitalRead(POWER_BUTTON_PIN);


  if (!powerbutton) {
    FastLED.clear(true);

    esp_deep_sleep_start();
  }


//  int encoderButton = digitalRead(ROTARY_PIN_BUTTON);
  programId = (encoder.getCount() / 2) % PROGRAM_COUNT;

  if (millis() / 100 % 2) {
    setBrightness();
  }

  display.showNumber(programId);

//  digitalWrite(POT_LED_PIN, encoderButton);

  runProgram();
  //drawRainbows2();
  FastLED.show();
  ArduinoOTA.handle();

}
