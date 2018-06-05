 char bufa[1000];
 char currentChar;

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

 Forecast forecasts[40];
 byte forecastIndex = 0;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  static uint16_t charIndex = 0;
  
    if (Serial.available())
    {
      currentChar = Serial.read();      
      bufa[charIndex++] = currentChar;
    }

    if (currentChar == '`')
    {
      Forecast frcast;
      ParseForecast(&frcast, bufa);
      Serial.println(String("free heap: ") + ESP.getFreeHeap());
      Serial.println(String("temp ") + frcast.temp);
      Serial.println(String("t min ") + frcast.tempMin);
      Serial.println(String("t max ") + frcast.tempMax);
      Serial.println(String("pressure ") + frcast.pressure);
      Serial.println(String("sea level ") + frcast.seaLevel);
      Serial.println(String("ground level ") + frcast.groundLevel);
      Serial.println(String("humidity ") + frcast.humidity);
      Serial.println(frcast.mainWeather);
      Serial.println(frcast.mainDescription);
      Serial.println(String("cloud cover ") + frcast.clouds);
      Serial.println(String ("wind speed ") + frcast.windSpeed);
      Serial.println(String ("wind direction ") + frcast.windDirection);
      Serial.println(String("rain ") + frcast.rain);
      Serial.println(String("snow ") + frcast.snow);
      Serial.println(String("day of month ") + frcast.dayOfMonth);
      Serial.println(String("month ") + frcast.monthOfYear);
      Serial.println(String("hour ") + frcast.hourOfDay);
      memset (bufa, '\0' , 1000);
      charIndex = 0;
      currentChar = 0;
      forecasts[forecastIndex++] = frcast;
      
      if (forecastIndex > 40){
        forecastIndex = 0;
      }
    }

    if (currentChar == '~')
    {
      currentChar = 0;
      for (uint8_t i = 0; i < 40; i++)
      {
       Serial.println (String(i) + " " + forecasts[i].temp);
      }
    }
}


