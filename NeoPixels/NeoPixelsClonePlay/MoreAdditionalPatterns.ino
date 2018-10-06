// Sorting Colors is not a trivial task
void BubbleSortColorsByAbsoluteValue(uint32_t wait)
{
	uint32_t millisAtStart = millis();

	bool anythingChanged = true;
	while (anythingChanged)
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
	{
	}
}

void FlashingAllPixels(uint8_t startingMillisecondsWait, uint8_t endingMillisecondsWait, uint16_t milisecondsToCycleEachValue, uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t millisAtStart = millis();

	if (startingMillisecondsWait == endingMillisecondsWait)
	{
		while (millisAtStart + milisecondsToCycleEachValue > millis())
		{
			stripSet(red, green, blue, startingMillisecondsWait);
			stripSet(0, 0, 0, startingMillisecondsWait);
		}
	}

	while (startingMillisecondsWait < endingMillisecondsWait)
	{
		while (millisAtStart + milisecondsToCycleEachValue > millis())
		{
			stripSet(red, green, blue, startingMillisecondsWait);
			stripSet(0, 0, 0, startingMillisecondsWait);
		}

		startingMillisecondsWait++;
		millisAtStart = millis();
	}

	while (startingMillisecondsWait > endingMillisecondsWait)
	{
		while (millisAtStart + milisecondsToCycleEachValue > millis())
		{
			stripSet(red, green, blue, startingMillisecondsWait);
			stripSet(0, 0, 0, startingMillisecondsWait);
		}

		startingMillisecondsWait--;
		millisAtStart = millis();
	}
}

void FlashingAllPixels(uint8_t startingMillisecondsWait, uint8_t endingMillisecondsWait, uint16_t timesToCycleEachValue, int32_t color)
{
	uint8_t red = (uint8_t)((color >> 16) & 0xff);
	uint8_t green = (uint8_t)((color >> 8) & 0xff);
	uint8_t blue = (uint8_t)(color & 0xff);

	FlashingAllPixels(startingMillisecondsWait, endingMillisecondsWait, timesToCycleEachValue, red, green, blue);
}

void SequentalRandomChange(uint32_t wait, uint32_t delayBetweenPixelsInMicros)
{
	uint32_t millisAtStart = millis();

	while (millisAtStart + wait > millis()) 
	{
		for (size_t r = 0; r < strip.numPixels(); r++)
		{
			strip.setPixelColor(r, random(0, 256), random(0, 256), random(0, 256));
			strip.show();
			delayMicroseconds(delayBetweenPixelsInMicros);
		}
	}
}

void RandomAndDarkPixels(uint8_t chanceOfDarkPixels) 
{
	for (size_t i = 0; i < strip.numPixels(); i++)
	{
		bool dark = false;
		uint8_t rnd = random(0, 101);

		if (rnd < chanceOfDarkPixels) 
		{
			dark = true;
		}

		if (dark) 
		{
			strip.setPixelColor(i, 0, 0, 0);
		}
		else if (random(0, 10) < 4) 
		{
			strip.setPixelColor(i, random(0, 2) * 255, random(0, 2) * 255, random(0, 2) * 255);
		}
		else if (random(0, 2) == 0)
		{
			strip.setPixelColor(i, random(0, 2) * 255, random(0, 2) * 255, 0);
		}
		else 
		{
			strip.setPixelColor(i, random(0, 256), random(0, 256), random(0, 256));
		}
	}
}

void RearrangeDarkPixelsSequentally(uint16_t waitBetweenRearrangements) 
{
	byte initialBrightness = strip.getBrightness();
	bool anythingChanged = true;
	while (anythingChanged)
	{
		anythingChanged = false;
		for (size_t ic = 0; ic < NumberOfPixels - 1; ic++)
		{
			bool changeThisCycle = false;
			// without this setBrightness nonsense, some pixels disappear weight brightness is low
			strip.setBrightness(255);
			long lngRGB = strip.getPixelColor(ic);
			long lngRGBnext = strip.getPixelColor(ic + 1);

			if (lngRGBnext == 0 && lngRGB != 0)
			{
				strip.setPixelColor(ic, lngRGBnext);
				strip.setPixelColor(ic + 1, lngRGB);
				strip.setBrightness(initialBrightness);
				strip.show();
				anythingChanged = true;
				changeThisCycle = true;
			}		

			if (changeThisCycle) 
			{
				delay(waitBetweenRearrangements);
			}
			else 
			{
				delay(waitBetweenRearrangements / 3);
			}
		}
	}

	strip.setBrightness(initialBrightness);
	strip.show();
}

void RearrangeDarkPixelsSequentallyLightFirst(uint16_t waitBetweenRearrangements)
{
	byte initialBrightness = strip.getBrightness();
	bool anythingChanged = true;
	while (anythingChanged)
	{
		anythingChanged = false;
		for (size_t ic = NumberOfPixels - 1; ic > 0; ic--)
		{
			bool changeThisCycle = false;
			// without this setBrightness nonsense, some pixels disappear weight brightness is low
			strip.setBrightness(255);
			long lngRGB = strip.getPixelColor(ic);
			long lngRGBnext = strip.getPixelColor(ic - 1);

			if (lngRGBnext == 0 && lngRGB != 0)
			{
				strip.setPixelColor(ic, lngRGBnext);
				strip.setPixelColor(ic - 1, lngRGB);
				strip.setBrightness(initialBrightness);
				strip.show();
				anythingChanged = true;
				changeThisCycle = true;
			}

			if (changeThisCycle)
			{
				delay(waitBetweenRearrangements);
			}
			else
			{
				delay(waitBetweenRearrangements / 3);
			}
		}
	}

	strip.setBrightness(initialBrightness);
	strip.show();
}