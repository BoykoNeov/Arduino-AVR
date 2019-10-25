#define DEBUG

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define LED_BUILTIN 2

const char* ssid;
const char* password;

BluetoothSerial SerialBT;

void setup() 
{
  AssignWifiPassword();
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
    {
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
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  ArduinoOTA.handle();

  char* bluetoothInput; // max length of message - 50
  bluetoothInput = ReadBluetooth();

  if (bluetoothInput != NULL)
  {
    Serial.println(bluetoothInput);
  }

//  if (SerialBT.available())
//  {
//    byteRead = SerialBT.read();
//    delay(20);
//  }
//
//  if (byteRead == 48)
//  {
//    digitalWrite(LED_BUILTIN, LOW);
//    Serial.println("0");
//  }
//  else if (byteRead == 49)
//  {
//    digitalWrite(LED_BUILTIN, HIGH);
//    Serial.println("1");
//  }
//  else if (byteRead == 50)
//  {
//    #ifdef DEBUG
//    Serial.println(ESP.getFreeHeap());
//    #endif
//    
//    uint32_t num = ESP.getFreeHeap();
//    char cstr[16];
//    itoa(num, cstr, 10);
//
//    char outputchar;
//
//    for (byte i = 0; i < strlen(cstr); i++)
//    {
//       outputchar = cstr[i];
//       SerialBT.write(outputchar);
//    }
//  }
}
