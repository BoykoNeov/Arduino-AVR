// Based on examples from https://github.com/gilmaimon/ArduinoWebsockets
// and https://github.com/acrobotic/Ai_Tips_ESP8266

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "FS.h"
#include "AsyncTCP.h"
#include "Passwords.h"
#include <ArduinoWebsockets.h>

const char* ssid = ssid2;
const char* password = password2;

AsyncWebServer server(80);
using namespace websockets;
WebsocketsServer WSserver;


void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  if(!SPIFFS.begin(true)) // Initialize SPIFFS
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
    request->send(SPIFFS, "/index.html", String(), false);
    });

  server.begin();

  WSserver.listen(81);
  Serial.print("Is WSserver live? ");
  Serial.println(WSserver.available());
  
}

void loop()
  {  
    WebsocketsClient client = WSserver.accept();
    if(client.available()) 
    {
    WebsocketsMessage msg = client.readBlocking();

    // log
    Serial.print("Got Message: ");
    Serial.println(msg.data());

    // return echo
    client.send("Echo: " + msg.data());

    // close the connection
    client.close();
    }

    delay(1000);
  }
