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
  //use the last four bytes of the EEPROM to hold cycles count, these bytes wont be teared out as much in practice, becasue data will be written to them only every 100 cycles
  byte buf[4];

  Serial.begin(9600);

  buf[3] = EEPROM.read(EEPROM.length() - 1);
  buf[2] = EEPROM.read(EEPROM.length() - 2);
  buf[1] = EEPROM.read(EEPROM.length() - 3);
  buf[0] = EEPROM.read(EEPROM.length() - 4);

  cycles += (uint32_t)buf[3] << 24;
  cycles += (uint32_t)buf[2] << 16;
  cycles += (uint32_t)buf[1] << 8;
  cycles += (uint32_t)buf[0];
  
  // initialize the LED pin as an output.
  pinMode(13, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("EEPROMtearIn:");
  uint32_t oldMillis = 0;
  byte seconds = 120;

  while (seconds > 0)
  {
      lcd.setCursor(13,0);
      lcd.print(seconds);

      if (oldMillis + 1000 <= millis())
      {
        oldMillis += 1000;
        seconds--;
        lcd.setCursor(13,0);
       lcd.print("   ");
      }

      lcd.setCursor(0,1);
      lcd.print("Cyc:");
      lcd.print(cycles);
  }

Serial.println(cycles);

  // turn the LED on when we're done
    lcd.clear();
}

void loop() 
{
  lcd.setCursor(0,0);
  lcd.print("Writing 170 ");
  lcd.setCursor(0,1);
  lcd.print("Cycles:");
  lcd.print(cycles);
  digitalWrite(13, LOW);
  
  for (int i = 0 ; i < EEPROM.length() - 4; i++) 
  {
    // 170 = 0b10101010
  EEPROM.write(i, 170);
  }
  
  cycles++;
  // this should be checked here also, because if for some reason the cycle count becomes an odd number (for example using a different sketct before, while keeping the cycle count) the cycle count wont be written ever
  CheckCyclesCountAndWriteDownIfNecessary();
  
  lcd.setCursor(0,0);
  lcd.print("Writing 85 ");
  lcd.setCursor(0,1);
  lcd.print("Cycles:");
  lcd.print(cycles);
  digitalWrite(13, HIGH);

  for (int i = 0 ; i < EEPROM.length() - 4; i++) 
  {
    // 85 = 0b01010101
  EEPROM.write(i, 85);
  }
  
  cycles++;
  CheckCyclesCountAndWriteDownIfNecessary();
}

void CheckCyclesCountAndWriteDownIfNecessary()
{
  //Write current number of cycles to otherwise uncycled EEPROM cells every 100 cycles if the total value is bellow 1mln and every 1000 cycles afterwards
  bool writeCycleNumberToEEPROM = false;
  if (cycles < 1000000 && cycles % 100 == 0)
  {
    writeCycleNumberToEEPROM = true;
  }
  else if (cycles % 1000 == 0)
  {
    writeCycleNumberToEEPROM = true;
  }
  
  if (writeCycleNumberToEEPROM)
  {
    byte buf[4];
    buf[0] = (byte)cycles;
    buf[1] = (byte)(cycles >> 8);
    buf[2] = (byte)(cycles >> 16);
    buf[3] = (byte)(cycles >> 24);

    EEPROM.write(EEPROM.length() - 4, buf[0]);
    EEPROM.write(EEPROM.length() - 3, buf[1]);
    EEPROM.write(EEPROM.length() - 2, buf[2]);
    EEPROM.write(EEPROM.length() - 1, buf[3]);
  } 
}
