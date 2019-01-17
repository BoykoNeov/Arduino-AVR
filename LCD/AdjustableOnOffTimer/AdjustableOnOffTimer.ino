#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

uint32_t on_Time = 0;
uint32_t off_Time = 0;
uint8_t lastKeyPressed = btnNONE;
uint32_t timeOfLastChange = 0;

void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Push the buttons"); // print a simple message
}
 
void loop()
{
 lcd.setCursor(0,0);            // move cursor to second line "1" and 9 spaces over

  uint32_t on_Time_in_Seconds = on_Time / 1000;
  uint8_t on_Time_Hours = on_Time_in_Seconds / 3600;
  uint8_t remainingMinutes = (on_Time_in_Seconds % 3600) / 60;
  uint8_t remainingSeconds = (on_Time_in_Seconds % 3600) % 60;
 
 PrintNumericValue(on_Time_Hours);
 lcd.print(":");
 PrintNumericValue(remainingMinutes);
 lcd.print(":");
 PrintNumericValue(remainingSeconds);

 lcd.setCursor(0,1);       
    
 lcd_key = read_LCD_buttons();

   uint8_t multiplier = 1;

 if ((lastKeyPressed != lcd_key) && (millis() - timeOfLastChange > 200))
    {
      multiplier = 10;
      lastKeyPressed = btnNONE;
    }

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
    if (lastKeyPressed == lcd_key){
      break;
    }
    
   case btnRIGHT:
     {
     on_Time += 60 * 1000 * multiplier;
     lcd.print("up");
     lastKeyPressed = lcd_key;
     timeOfLastChange = millis();
     break;
     }
   case btnLEFT:
     {
     on_Time -= 60* 1000 * multiplier;
     lcd.print("down");
     lastKeyPressed = lcd_key;
     timeOfLastChange = millis();
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     lastKeyPressed = lcd_key;
     timeOfLastChange = millis();
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     lastKeyPressed = lcd_key;
     timeOfLastChange = millis();
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     lastKeyPressed = lcd_key;
     timeOfLastChange = millis();
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }
 }
}
