// #include <ESP32Encoder.h>
// #include "display.h"

// #include <ESPAsyncE131.h>

#include "programs.h"
#include "wifisetup.h"
#include "server.h"

// #define SEGMENT_PINS GPIO_NUM_32, GPIO_NUM_26, GPIO_NUM_13, GPIO_NUM_12, GPIO_NUM_14, GPIO_NUM_27, GPIO_NUM_25, GPIO_NUM_33, GPIO_NUM_2, GPIO_NUM_15

// #define POT_INPUT_PIN GPIO_NUM_36
// #define POT_LED_PIN GPIO_NUM_16

// #define ROTARY_PIN_A GPIO_NUM_34
// #define ROTARY_PIN_B GPIO_NUM_35
// #define ROTARY_PIN_BUTTON GPIO_NUM_19

// #define POWER_BUTTON_PIN GPIO_NUM_4

// #define UNIVERSE 10
// #define UNIVERSE_COUNT 10                // Total number of Universes to listen for, starting at UNIVERSE

// Display display = Display(SEGMENT_PINS);
// ESP32Encoder encoder;


// ESPAsyncE131 instance with UNIVERSE_COUNT buffer slots
// ESPAsyncE131 e131(UNIVERSE_COUNT);


// void updateBrightness() {
//   int pot_value = ((float) analogRead(POT_INPUT_PIN)) / 16.062745098039215;
//   FastLED.setBrightness(pot_value);
// }

// void setupOld() {
//   delay(500);
//   Serial.begin(115200);
//   Serial.println("Booting");

//   display.setup();

//   // pinMode(POWER_BUTTON_PIN, INPUT_PULLDOWN);

//   // pinMode(POT_INPUT_PIN, INPUT);
//   // pinMode(POT_LED_PIN, OUTPUT);

//   // pinMode(ROTARY_PIN_BUTTON, INPUT_PULLDOWN);

//   // ESP32Encoder::useInternalWeakPullResistors=UP;
//   // encoder.attachFullQuad(ROTARY_PIN_A, ROTARY_PIN_B);

//   // wifiManager.setConfigPortalBlocking(false);
//   // if (wifiManager.autoConnect(ssid)) {
//   //   Serial.println("Ready");
//   //   Serial.print("IP address: ");
//   //   Serial.println(WiFi.localIP());
//   // } else {
//   //   Serial.println("Configportal running");
//   // }

//   // // Choose one to begin listening for E1.31 data
//   // // if (e131.begin(E131_UNICAST))                               // Listen via Unicast
//   // if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT))   // Listen via Multicast
//   //     Serial.println(F("Listening for data..."));
//   // else 
//   //     Serial.println(F("*** e131.begin failed ***"));

//   // ESP.restart();

//   // ArduinoOTA
//   //   .onStart([]() {
//   //     String type;
//   //     if (ArduinoOTA.getCommand() == U_FLASH)
//   //       type = "sketch";
//   //     else // U_SPIFFS
//   //       type = "filesystem";

//   //     // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
//   //     Serial.println("Start updating " + type);
//   //   })
//   //   .onEnd([]() {
//   //     Serial.println("\nEnd");
//   //   })
//   //   .onProgress([](unsigned int progress, unsigned int total) {
//   //     Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
//   //   })
//   //   .onError([](ota_error_t error) {
//   //     Serial.printf("Error[%u]: ", error);
//   //     if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
//   //     else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
//   //     else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
//   //     else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
//   //     else if (error == OTA_END_ERROR) Serial.println("End Failed");
//   //   });

//   // ArduinoOTA.begin();

//   // FastLED.setMaxPowerInVoltsAndMilliamps( VOLTS, MAX_MA);
//   // FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(matrix.leds, matrix.nrleds())
//   //   .setCorrection(TypicalLEDStrip);
//   // updateBrightness();

//   // esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, 1);
// }

void setup() {
  delay( 300 ); //safety startup delay

  Serial.begin(115200);
  while (!Serial);

  delay(200);

	Serial.println("Setting up wifi");
	wifiSetup();
	
	// Serial.println("Setting up server");
	serverSetup();

	Serial.println("Setting up leds");
	programSetup();
}


// void readProgram() {
//   int selectedProgram = mod8(encoder.getCount() / 2, PROGRAM_COUNT);

//   if (selectedProgram != programId) {
//     programId = selectedProgram;
//     programs[programId]->setup();
//     Serial.printf("Selected program: %d\n", programId);
//   }
//   display.showNumber(selectedProgram);

// }

// EventTimer t_brightness(50);

// void loopOld() {
//   // wifiManager.process();

//   // Control the brightness of the grid.
//   // if (t_brightness.ready())
//   //   updateBrightness();

//   // The powerbutton triggers the deep sleep, being woken up by interrupts
//   // int powerbutton = digitalRead(POWER_BUTTON_PIN);
//   // if (!powerbutton) {
//   //   FastLED.clear(true);

//   //   esp_deep_sleep_start();
//   // }

//   // int encoderButton = digitalRead(ROTARY_PIN_BUTTON);
//   // digitalWrite(POT_LED_PIN, encoderButton);

//   // readProgram();
//   programs[programId]->tick(matrix);
//   FastLED.show();

//   // if (!e131.isEmpty()) {
//   //     e131_packet_t packet;
//   //     e131.pull(&packet);     // Pull packet from ring buffer

//   //     Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u\n",
//   //             htons(packet.universe),                 // The Universe for this packet
//   //             htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
//   //             e131.stats.num_packets,                 // Packet counter
//   //             e131.stats.packet_errors,               // Packet error counter
//   //             packet.property_values[0]);             // Dimmer data for Channel 1


//   //     programId = packet.property_values[0];
//   //     encoder.setCount(programId * 2);
//   // }

//   // ArduinoOTA.handle();
// }

void loop(){
	wifiLoop();
	serverLoop();

	programLoop();
}