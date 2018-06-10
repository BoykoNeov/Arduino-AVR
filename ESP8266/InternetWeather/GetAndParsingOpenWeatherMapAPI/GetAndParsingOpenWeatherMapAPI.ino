#include <ESP8266WiFi.h>
//#include <Wire.h>
 
 struct Forecast {
  float temp = -100;
  float tempMin = -100;
  float tempMax = -100;
  float pressure = -100;
  float seaLevel = -100;
  float groundLevel = -100;
  int16_t humidity = -100;
  char mainWeather[10];
  char mainDescription[20];
  int16_t clouds = -100;
  float windSpeed = -100;
  float windDirection = -100;
  float rain = -100;
  float snow = -100;
  byte dayOfMonth = 0;
  byte monthOfYear = 0;
  byte hourOfDay = 0;
 };

 String result;

Forecast forecasts[40];
byte forecastIndex = 0;
char bufa[500];

const char* ssid     = "****";  // SSID of local network
const char* password = "****";                    // Password on network
String APIKEY = "****";                                 
String CityID = "727011";                                 //Your City ID = Sofia

WiFiClient client;
char servername[]="api.openweathermap.org";              // remote server we will connect to                                   

void setup() 
{
  Serial.begin(9600);
  Serial.println("Connecting... in 20");

  for (byte i = 20; i> 0; i--)
  {
    Serial.println(i);
    delay(1000);
  }

  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) 
   {
     delay(500);   
   }

  Serial.println("Connected");
  delay(1000);
  getWeatherData();

//  for (uint8_t i = 0; i < 40; i++)
//  {
//    PrintForecast(&forecasts[i]);
//  }
}

void loop() 
{ 

      

     // memset (bufa, '\0' , 1000);     
}

void getWeatherData()                                //client function to send/receive GET request data.
{
  if (client.connect(servername, 80))   
       {                                         //starts client connection, checks for connection
        client.println("GET /data/2.5/forecast?id="+CityID+"&units=metric&APPID="+APIKEY);
        client.println("Host: api.openweathermap.org");
        client.println("User-Agent: ArduinoWiFi/1.1" + String(random(97,122)) + String(random(97,122)));
        client.println("Connection: close");
        client.println();
       } 
  else {
         Serial.println("connection failed");        //error message if no client connect
         Serial.println();
       }

  int openBrackets = 0;
  bool startedRealData = false;
  
  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char c = client.read();                     //gets byte from ethernet buffer
         if (c == '{')
         {
          openBrackets++;
         }
         else if (c == '}'){
          openBrackets--;
         }

         if (openBrackets > 1){
          startedRealData = true;
         }

         if (startedRealData == true && openBrackets < 2){
          result.replace('[', ' ');
          result.replace(']', ' ');
          
          result.toCharArray(bufa,500);
          ParseForecast(&forecasts[forecastIndex++], bufa);

          //debug
          Serial.println(" *** upper spacer ***");
          Serial.println(result);
          PrintForecast(&forecasts[forecastIndex-1]);
          //debug
          Serial.println(" *** lower spacer ***");
          
          result = "";
         }
         
         result = result+c;
       }



client.stop();                                      //stop client
result.replace('[', ' ');
result.replace(']', ' ');
result.toCharArray(bufa,500);
ParseForecast(&forecasts[forecastIndex++], bufa);
result = "";
}
