#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
const int NumberOfPixels = 40;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumberOfPixels, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  FlashingAllPixels(10, 50, 31, 255, 255, 255);
  FlashingAllPixels(50, 10, 31, 255, 255, 255);

 twinkleRand(40 ,strip.Color(random(0,255),random(0,255),random(0,255)),strip.Color(random(0,255), random(0, 255), random(0,255)),90); 

GradualRandomChange(1000);
delay (10000);

WarmRandomSwitching(1000);
BubbleSortColorsByAbsoluteValue(1000);

delay(1000);
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  
//  colorWipe(strip.Color(255, 0, 0), 100); // Red
//  colorWipe(strip.Color(0, 255, 0), 100); // Green
//  colorWipe(strip.Color(0, 0, 255), 100); // Blue
//  colorWipe(strip.Color(0, 0, 0, 255), 100); // White RGBW
//  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 100); // White
//  theaterChase(strip.Color(127, 0, 0), 100); // Red
//  theaterChase(strip.Color(0, 0, 127), 100); // Blue
//
  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
  WarmRandomSwitching(1000);
  FlashingAllPixels(20, 50, 30, 255, 255, 255);
  FlashingAllPixels(50, 20, 30, 255, 255, 255);
  theaterChaseRainbow(50);
  rainbowCycle(20);
}

// number, twinkle color, background color, delay
// twinkleRand(5,strip.Color(255,255,255),strip.Color(255, 0, 100),90);

// twinkle random number of pixels
void twinkleRand(int num,uint32_t c,uint32_t bg,int wait) {
  // set background
   stripSet(bg,0);
   // for each num
   for (int i=0; i<num; i++) {
     strip.setPixelColor(random(strip.numPixels()),c);
   }
  strip.show();
  delay(wait);
}

 // very simple wave: velocity, cycles,delay between frames
 // simpleWave(0.03,5,10);
void simpleWave(float rate,int cycles, int wait) {
   float pos=0.0;
  // cycle through x times
  for(int x=0; x<(strip.numPixels()*cycles); x++)
  {
      pos = pos+rate;
      for(int i=0; i<strip.numPixels(); i++) {
        // sine wave, 3 offset waves make a rainbow!
        float level = sin(i+pos) * 127 + 128;
        // set color level 
        strip.setPixelColor(i,(int)level,0,0);
      }
         strip.show();
         delay(wait);
  }
}

void GradualRandomChange(uint32_t wait)
    {
      uint8_t targetColor[120];
      for (uint8_t k = 0; k < 120; k++)
      {
        targetColor[k] = random(0, 20);
      }
  
      uint32_t millisAtStart = millis();
      uint32_t currentMillis = millisAtStart;

      while (millisAtStart + wait > millis())
      {
        for (uint8_t k = 0; k < strip.numPixels(); k++)
        {
        uint8_t c[3]; 
        long lngRGB = strip.getPixelColor(k);
        c[0] = (uint8_t)((lngRGB >> 16) & 0xff);
        c[1] = (uint8_t)((lngRGB >> 8) & 0xff);
        c[2] = (uint8_t)(lngRGB & 0xff);

          for (uint8_t j = 0; j < 3; j++)
          {
            if (c[j] == targetColor[j + k * 3])
            {
              if (j == 2)
              {
                targetColor[j + k * 3] = random(0, 40);
              }
              else if (j == 1)
              {
                targetColor[j + k * 3] = random(0, 117);
              }
              else
              {
                targetColor[j + k * 3] = random(0, 256);
              }
            }
            else if (c[j] > targetColor[j + k * 3])
            {
              c[j]--;
            }
            else
            {
              c[j]++;
            }
          }

        strip.setPixelColor(k, c[0], c[1], c[2]);
        strip.show();
        }
      }
    }

void WarmRandomSwitching(uint32_t wait){
  for (uint8_t i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, (uint8_t)random(0,256), (uint8_t)random(0, 256), (uint8_t)random(0,256));
      strip.show();
    }
  
   uint32_t millisAtStart = millis();
   uint32_t currentMillis = millisAtStart;

   while (millisAtStart + wait > millis())
   {
    for (uint8_t i = 0; i < strip.numPixels(); i++)
    {
      uint8_t c[3]; 
      long lngRGB = strip.getPixelColor(i);
      c[0] = (uint8_t)((lngRGB >> 16) & 0xff),
      c[1] = (uint8_t)((lngRGB >> 8) & 0xff),
      c[2] = (uint8_t)(lngRGB & 0xff);

      for (uint8_t j = 0; j < 3; j++)
      {
        c[j] += random(-20,21);
      }

      if (c[2] >= 80)
      {
        c[2] -= 80;
      }

      strip.setPixelColor(i, c[0], c[1], c[2]);
      strip.show();
    }
   }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// quickly set the entire strip a color
void stripSet(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, red, green, blue);
  }
  // move the show outside the loop
  strip.show();
  delay(wait);
}

// quickly set the entire strip a color
void stripSet(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
  }
  // move the show outside the loop
  strip.show();
  delay(wait);
}
