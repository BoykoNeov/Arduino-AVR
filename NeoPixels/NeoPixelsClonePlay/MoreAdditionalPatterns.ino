// Sorting Colors is not a trivial task
void BubbleSortColorsByAbsoluteValue(uint32_t wait)
{
      uint32_t millisAtStart = millis();
      uint32_t currentMillis = millisAtStart;

      bool anythingChanged = true;
      while(anythingChanged)
      {
        anythingChanged = false;

        for (uint8_t p1 = 0; p1 < strip.numPixels() - 1; p1++)
        {
            long lngRGB = strip.getPixelColor(p1);
            long lngRGBnext = strip.getPixelColor(p1 + 1);
            if (lngRGBnext > lngRGB)
            {
              strip.setPixelColor(p1, lngRGBnext);
              strip.setPixelColor(p1 + 1, lngRGB);
              strip.show();
              anythingChanged = true;
            }
        }
      }

      while (millisAtStart + wait > millis())
      {}
}

void FlashingAllPixels(uint8_t startingMillisecondsWait, uint8_t endingMillisecondsWait, uint16_t milisecondsToCycleEachValue, uint8_t red, uint8_t green, uint8_t blue)
{
   uint32_t millisAtStart = millis();
   uint32_t currentMillis = millisAtStart;
  
  if (startingMillisecondsWait == endingMillisecondsWait)
  {
    while (millisAtStart + milisecondsToCycleEachValue > millis())
    {
      stripSet(red, green, blue, startingMillisecondsWait);
      stripSet(0,0,0,startingMillisecondsWait);
    } 
  }
  
  while (startingMillisecondsWait < endingMillisecondsWait)
  {
    while (millisAtStart + milisecondsToCycleEachValue > millis())
    {
      stripSet(red, green, blue, startingMillisecondsWait);
      stripSet(0,0,0,startingMillisecondsWait);
    } 

    startingMillisecondsWait++;
    millisAtStart = millis();
  }

   while (startingMillisecondsWait > endingMillisecondsWait)
  {
    while (millisAtStart + milisecondsToCycleEachValue > millis())
    {
      stripSet(red, green, blue, startingMillisecondsWait);
      stripSet(0,0,0,startingMillisecondsWait);
    } 

    startingMillisecondsWait--;
    millisAtStart = millis();
  }
}

void FlashingAllPixels(uint8_t startingMillisecondsWait, uint8_t endingMillisecondsWait, uint16_t timesToCycleEachValue, int32_t color)
{
           uint8_t red = (uint8_t)((color >> 16) & 0xff);
           uint8_t green  = (uint8_t)((color >> 8) & 0xff);
           uint8_t blue = (uint8_t)(color & 0xff);

           FlashingAllPixels(startingMillisecondsWait, endingMillisecondsWait, timesToCycleEachValue, red, green, blue);
}
