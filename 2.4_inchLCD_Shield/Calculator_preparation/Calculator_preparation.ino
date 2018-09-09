#define LCD_CS A3    
#define LCD_CD A2    
#define LCD_WR A1   
#define LCD_RD A0    
// you can also just connect RESET to the arduino RESET pin
#define LCD_RESET A4
//Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontoller:
// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

#include "math.h"
#include "TFTLCD.h"
#include "BigNumber.h"

TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) 
{
  Serial.begin(9600);

  delay(10);
  tft.reset();
  delay(10);
  tft.initDisplay();
  delay(10);
  tft.setRotation(1);
  delay(10);
}

void loop(void) 
{
  tft.fillScreen(BLACK);
//  tft.setCursor(150, 150);
//  tft.setTextColor(random(0,65000));

  BigNumber::begin ();  // initialize library
  BigNumber b = "2000000", c;
  BigNumber::setScale (30);
  c = b.sqrt ();
  PrintBignumToTFT (c, 1);
  Serial.println(sqrt(2000000));

  delay (2000);
  tft.fillScreen(BLACK);   


}
