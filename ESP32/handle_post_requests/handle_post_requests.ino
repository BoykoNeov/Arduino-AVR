#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Passwords.h"
 
const char* ssid = ssid2;
const char* password = password2;
 
AsyncWebServer server(80);
 
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println(WiFi.localIP());
 
  server.on(
    "/post",  //Must send a POST request to currentIP/post in order to test
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
 
      for (size_t i = 0; i < len; i++) {
        Serial.write(data[i]);
      }
 
      Serial.println();
 
      request->send(200);
  });
 
  server.begin();
}
 
void loop() {}
