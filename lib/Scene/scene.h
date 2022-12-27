#pragma once
#include <map>
#include <string>

#include "program.h"

#include "waves.h"
#include "trickle.h"
#include "redtrickle.h"
#include "palette.h"
#include "tree.h"
#include "twinkle.h"

class Scene {
  std::map<std::string, Program*> programs;
  boolean shouldSave = false;

  public:
    Scene(std::map<std::string, Program*> programs) {
      this->programs = programs;
    }
    void add(std::string name, Program *program) {
      programs[name] = program;
    }
    void setConfig(JsonObject data) {
      for (JsonPair kv: data) {
        std::string name = kv.key().c_str();
        Serial.printf("Setting config for %s\n", name.c_str());
        if (this->programs.find(name) != this->programs.end()) {
          this->programs[name]->setConfig(kv.value());
        }
      }
      shouldSave = true;
    }
    void getConfig( DynamicJsonDocument &config) {

      for(auto &[name, program]: programs) {
        JsonObject obj = config.createNestedObject(name.c_str());
        program->getConfig(obj);
      }
    }
    void storeConfig(const char *filename) {
      DynamicJsonDocument config(2048);

      getConfig(config);

      // Open file for writing
      File file = FileFS.open(filename, "w");

      if (!file) {
        Serial.println("Failed to open config file for writing");
        return;
      }
      // Serialize JSON to file
      if (serializeJson(config, file) == 0) {
        Serial.println(F("Failed to write to file"));
      }

      // Close the file
      file.close();
    }
    void loadConfig(const char *filename) {
      DynamicJsonDocument config(2048);
      // Open file for writing
      Serial.println("Loading led config");
      File file = FileFS.open(filename);

      if (!file) {
        Serial.println("Failed to open config file for writing");
        return;
      }
      // Serialize JSON to file
      DeserializationError error = deserializeJson(config, file);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        file.close();
        return;
      }

      Serial.println("Applying led config");
      setConfig(config.as<JsonObject>());
    
      // Close the file
      file.close();

    }
    Program* get(std::string name) {
      return programs[name];
    }
     
    void setup() {
      loadConfig("/config.json");
    }
    void tick() {
      EVERY_N_SECONDS(1) {
        if (shouldSave) {
          storeConfig("/config.json");
          shouldSave = false;
        }
      }
      FastLED.clearData();

      for(auto &[name, program]: programs) {
        program->tick();
      }

      FastLED.show();
    }
};

// #include "ArduinoJson.h"

// namespace ARDUINOJSON_NAMESPACE {
// template <>
// struct Converter<Scene> {
//   static void toJson(const CRGB& src, VariantRef dst) {
//     dst["r"] = src.r;
//     dst["g"] = src.g;
//     dst["b"] = src.b;
//   }

//   static CRGB fromJson(VariantConstRef src) {
//     return CRGB(src["r"], src["g"], src["b"]);
//   }

//   static bool checkJson(VariantConstRef src) {
//     return src["r"].is<uint8_t>() && src["g"].is<uint8_t>() && src["b"].is<uint8_t>();
//   }
// };
// }