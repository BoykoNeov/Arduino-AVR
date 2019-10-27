#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "Passwords.h"
 
const char* ssid = ssid2;
const char* password =  password2;
 
AsyncWebServer server(80);

// As explained in this blog post (https://esp32.com/viewtopic.php?t=2737), in the ESP32 constant data is automatically stored in FLASH memory and can be accessed directly from FLASH memory without previously copying it to RAM. So, there is no need to use the PROGMEM keyword below and it is only defined due to compatibility with other platforms.
// const char HTML[] = "<form onSubmit=\"event.preventDefault()\"><label class=\"label\">Network Name</label><input type=\"text\" name=\"ssid\"/><br/><label>Password</label><input type=\"text\" name=\"pass\"/><br/><input type=\"submit\" value=\"Submit\"></form>";
 
void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
   
    Serial.println(WiFi.localIP());
  
    if(!SPIFFS.begin(true)) // Initialize SPIFFS
    {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
    }
   
  //  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //  {
  //    request->send(200, "text/html", HTML);
  //  });
  
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
    request->send(SPIFFS, "/index.html", String(), false);
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
    request->send(SPIFFS, "/style.css", "text/css");
    });
   
    server.begin();
}

void loop()
{
}
