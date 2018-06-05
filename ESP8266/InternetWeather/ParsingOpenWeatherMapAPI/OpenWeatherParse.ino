 void ParseForecast (Forecast* forecast, char* forecastMessage)
 {
    int16_t currentIndex = 0;
    uint16_t endingIndex;
    String fm(forecastMessage);
    String str;

    //Parse temp
    currentIndex = fm.indexOf("temp");
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->temp = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse temp_min
    currentIndex = fm.indexOf("temp_min", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->tempMin = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse temp_max
    currentIndex = fm.indexOf("temp_max", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->tempMax = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse pressure
    currentIndex = fm.indexOf("pressure", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->pressure = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse seaLevel pressure
    currentIndex = fm.indexOf("sea_level", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->seaLevel = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse ground level pressure
    currentIndex = fm.indexOf("grnd_level", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->groundLevel = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse humidity
    currentIndex = fm.indexOf("humidity", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->humidity = str.toInt();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse weather main
    currentIndex = fm.indexOf("weather", currentIndex);
    if (currentIndex > -1)
      {
      currentIndex = fm.indexOf("main", currentIndex);
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex +1, endingIndex -1);
      str.toCharArray(forecast->mainWeather, 10);
      }
    else
      {
        currentIndex = 0;
      }

    //Parse weather main description
    currentIndex = fm.indexOf("description", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex +1, endingIndex -1);
      str.toCharArray(forecast->mainDescription, 20);
      }
    else
      {
        currentIndex = 0;
      }

    //Parse clouds
    currentIndex = fm.indexOf("""clouds""", currentIndex);
    currentIndex = fm.indexOf("""all""", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->clouds = str.toInt();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse wind speed
    currentIndex = fm.indexOf("""speed""", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->windSpeed = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse wind direction
    currentIndex = fm.indexOf("""deg""", currentIndex);
    if (currentIndex > -1)
      {  
      currentIndex = fm.indexOf(':' , currentIndex);
      currentIndex++;
      endingIndex = fm.indexOf(',' , currentIndex);
      str = fm.substring(currentIndex, endingIndex);
      forecast->windDirection = str.toFloat();
      }
    else
      {
        currentIndex = 0;
      }

    //Parse rain or snow
    bool rain = false;
    bool snow = false;
    currentIndex = fm.indexOf("""rain""" , currentIndex);
    if (currentIndex < 0)
    {
      currentIndex = 0;
      currentIndex = fm.indexOf("""snow""" , currentIndex);

      if (currentIndex > -1)
      {
        snow = true;
      }
    }
    else
    {
      rain = true;
    }    
    currentIndex = fm.indexOf("""3h""", currentIndex);
    if (currentIndex > -1)
      {  
        currentIndex = fm.indexOf(':' , currentIndex);
        currentIndex++;
        endingIndex = fm.indexOf(',' , currentIndex);
        str = fm.substring(currentIndex, endingIndex);
        if (rain == true)
        {
         forecast->rain = str.toFloat(); 
        }
        else if (snow == true)
        {
         forecast->snow = str.toFloat();
        }
      
      }
    else
      {
        currentIndex = 0;
      }

    //Parse date
    currentIndex = fm.indexOf("""dt_txt""", currentIndex);
    if (currentIndex > -1)
      {
      // Parse month
      currentIndex = fm.indexOf('-' , currentIndex);
      currentIndex++;
      endingIndex = currentIndex +2;
      str = fm.substring(currentIndex, endingIndex);
      forecast-> monthOfYear = str.toInt();

      // Parse day
      currentIndex+=3;
      endingIndex+=3;
      str = fm.substring(currentIndex, endingIndex);
      forecast-> dayOfMonth = str.toInt();

      // Parse hour
      currentIndex+=3;
      endingIndex+=3;
      str = fm.substring(currentIndex, endingIndex);
      forecast-> hourOfDay = str.toInt();
      }
    else
      {
        currentIndex = 0;
      }  
 }
