#include <ArduinoOTA.h>

#ifdef ESP32
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#endif

#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

class MainControl {
public:
    virtual bool setCurrentProgram(const std::string& name) = 0;
    virtual std::vector<std::string> getPrograms() = 0;
};
class DiskoServer {

    // SKETCH BEGIN
    AsyncWebServer server; // (80);
    AsyncEventSource events; // ("/events");
    const char *hostName; // = "disko-grid";

    AsyncCallbackJsonWebHandler* handler;
    MainControl *main;
public:
    DiskoServer(MainControl *main)
        : server(80)
        , events("/events")
        , hostName("disko-grid")
        , main(main)
    {
        handler = new AsyncCallbackJsonWebHandler("/color", [this](AsyncWebServerRequest *request, JsonVariant &json) {
          JsonObject jsonObj = json.as<JsonObject>();

          // scene->setConfig(jsonObj);
          request->send(200, "application/json", "{test: \"ok\"}");
        });
    }
    void setup()
    {

      //Send OTA events to the browser
      ArduinoOTA.onStart([this]() { events.send("Update Start", "ota"); });
      ArduinoOTA.onEnd([this]() { events.send("Update End", "ota"); });
      ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
        char p[32];
        sprintf(p, "Progress: %u%%\n", (progress/(total/100)));
        events.send(p, "ota");
      });
      ArduinoOTA.onError([this](ota_error_t error) {
        if(error == OTA_AUTH_ERROR) events.send("Auth Failed", "ota");
        else if(error == OTA_BEGIN_ERROR) events.send("Begin Failed", "ota");
        else if(error == OTA_CONNECT_ERROR) events.send("Connect Failed", "ota");
        else if(error == OTA_RECEIVE_ERROR) events.send("Recieve Failed", "ota");
        else if(error == OTA_END_ERROR) events.send("End Failed", "ota");
      });
      ArduinoOTA.setHostname(hostName);
      ArduinoOTA.begin();

        if(!MDNS.begin(hostName)) {
            Serial.println("Error starting mDNS");
            return;
        } else {
            Serial.println("mDNS should be up");
        }

      MDNS.addService("http","tcp",80);

      events.onConnect([this](AsyncEventSourceClient *client){
        client->send("hello!",NULL,millis(),1000);
      });
      
      DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type");
      DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

      server.addHandler(&events);
      server.addHandler(handler);

      // Send a POST request to <IP>/program to set the program
      server.on("/program", HTTP_POST, [this](AsyncWebServerRequest *request){
        if (request->hasParam("program", true)) {
            std::string program = request->getParam("program", true)->value().c_str();
            if (main->setCurrentProgram(program)) {
              return request->send(200, "text/plain");
            }
          return request->send(400, "text/plain", "Invalid program");
        }
        request->send(400, "text/plain", "No program selected");
      });
      server.on("/program", HTTP_GET, [this](AsyncWebServerRequest *request){
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(1024);

        for(auto &name : main->getPrograms()) {
          json.add(name);
        }
        serializeJson(json, *response);
        response->setCode(200);
        request->send(response);
      });

      server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

      server.onNotFound([this](AsyncWebServerRequest *request){
        Serial.printf("NOT_FOUND: ");
        if(request->method() == HTTP_GET)
          Serial.printf("GET");
        else if(request->method() == HTTP_POST)
          Serial.printf("POST");
        else if(request->method() == HTTP_DELETE)
          Serial.printf("DELETE");
        else if(request->method() == HTTP_PUT)
          Serial.printf("PUT");
        else if(request->method() == HTTP_PATCH)
          Serial.printf("PATCH");
        else if(request->method() == HTTP_HEAD)
          Serial.printf("HEAD");
        else if(request->method() == HTTP_OPTIONS) {
          Serial.printf("OPTIONS");
          request->send(200);
          return;
        } else
          Serial.printf("UNKNOWN");
        Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

        if(request->contentLength()){
          Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
          Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
        }

        int headers = request->headers();
        int i;
        for(i=0;i<headers;i++){
          AsyncWebHeader* h = request->getHeader(i);
          Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
        }

        int params = request->params();
        for(i=0;i<params;i++){
          AsyncWebParameter* p = request->getParam(i);
          if(p->isFile()){
            Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
          } else if(p->isPost()){
            Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
          } else {
            Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
          }
        }

        request->send(404);
      });
      server.begin();
    }

    void loop(){
      ArduinoOTA.handle();
    }
};
