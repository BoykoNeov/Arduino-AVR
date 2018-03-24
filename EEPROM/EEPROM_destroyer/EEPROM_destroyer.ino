#include <EEPROM.h>
#include <LiquidCrystal.h>

byte resistorSelect = 8; //rs select
byte enable = 9; //enable
byte d4 = 4; //d4
byte d5 = 5; //d5
byte d6 = 6; //d6
byte d7 = 7; //d7
uint32_t cycles = 0;

LiquidCrystal lcd(resistorSelect, enable, d4, d5, d6, d7);

void setup() 
{
  // initialize the LED pin as an output.
  pinMode(13, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("EEPROM tear in:");
  uint32_t oldMillis = 0;
  byte seconds = 120;

  while (seconds > 0)
  {
      lcd.setCursor(0,1);
      lcd.print(seconds);

      if (oldMillis + 1000 <= millis())
      {
        oldMillis += 1000;
        seconds--;
        lcd.setCursor(0,1);
       lcd.print("   ");
      }
  }

  // turn the LED on when we're done
    lcd.clear();
}

void loop() {
lcd.setCursor(0,0);
lcd.print("Cycles:");
lcd.setCursor(0,1);
  digitalWrite(13, HIGH);
//   for (int i = 0 ; i < EEPROM.length() ; i++) 
//   {
//   EEPROM.write(i, 0);
//   }

  digitalWrite(13, LOW);
// lcd.print(cycles);
//   cycles++;

//   for (int i = 0 ; i < EEPROM.length() ; i++) 
//   {
//   EEPROM.write(i, 255);
//   }

cycles++;
lcd.print(cycles);
}
