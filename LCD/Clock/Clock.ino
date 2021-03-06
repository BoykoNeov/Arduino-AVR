// include the library code:
#include <LiquidCrystal.h>

// EEPROM operation library
#include <EEPROM.h>

const byte resistorSelect = 8; //rs select
const byte enable = 9; //enable
const byte d4 = 4; //d4
const byte d5 = 5; //d5
const byte d6 = 6; //d6
const byte d7 = 7; //d7

int hours = 0;
int minutes = 14;
int seconds = 0;

int month = 2;
int monthDay = 27;
int year = 2018;

String monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
String daysOfWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
byte monthLengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned long oldMillis = 0;
unsigned long currentMillis = 0;
bool isLeapYear = false;
bool timeCorrectedToday = false;

// Cursor and editing/selecting variables
const byte blackChar[8] = 
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
int blackCursorColumn = 15;
int blackCursorRow = 0;
int lastBlackCursorColumn = 15;
int lastBlackCursorRow = 0;
bool blackCursorActive = false;
bool selectSwitch = false;
bool editSwitch = false;
byte editActiveSecondsCounter = 0;
byte selectActiveSecondsCounter = 0;

LiquidCrystal lcd(resistorSelect, enable, d4, d5, d6, d7);

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   4
#define btnLEFT   3
#define btnSELECT 5
#define btnNONE   0

byte oldPressedButton = 0;
byte currentPressedButton = 0;

byte sentinelBit = 0; // For determining last write EEPROM location
unsigned int EEPROMFreeIndex = 0;

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(0, blackChar);

  //serial com is for debugging
  EEPROMFreeIndex = DetermineFirstFreeEEPROMIndex();
  
  Serial.begin(9600);
  Serial.println("EEPROM free index:");
  Serial.println(EEPROMFreeIndex);

//because we dont need the last starting index ot free EEPROM, but the actual data, we substract 6 (Date/Time is 6 bytes long);
  if (EEPROMFreeIndex < EEPROM.length() + 1)
  {
    ReadDateTimeFromEEPROM(EEPROMFreeIndex - 6);
  }
  else
  {
    TrySetDateTimeFromLastCells();
    ResetEEPROMIndex();
  }
}

void loop() 
{
   if (editSwitch)
   {
     lcd.setCursor(12,1);
     lcd.print("EDIT");
   }
   else
   {
     lcd.setCursor(12,1);
     lcd.print("    ");
   }

  currentPressedButton = read_LCD_buttons();

  if (currentPressedButton != oldPressedButton)
  {
    oldPressedButton = currentPressedButton;

    if (!editSwitch)
    {
       MoveSelector(currentPressedButton);
    }
    else
    {
      EditDateTime(currentPressedButton);
    }
  }

if (selectSwitch)
{
  ClearLastBlackCursor();
  lastBlackCursorColumn = blackCursorColumn;
  lastBlackCursorRow = blackCursorRow;

  lcd.setCursor(blackCursorColumn, blackCursorRow);

  if (micros() % 2 == 0)
  {
      lcd.write((byte)0);
  }
  else
  {
    lcd.print(' ');
  }
}
  
  lcd.setCursor(0,0);
  lcd.print(year);
  lcd.print("/");
  lcd.print(monthNames[month]);
  lcd.print("/");
  
  PrintNumericValue(monthDay);

  byte dayOfWeek = ReturnDayOfWeek(year, month, monthDay);
  lcd.print(' ');
  lcd.print(daysOfWeek[dayOfWeek] + " ");
  lcd.setCursor(15,0);
  lcd.print(' ');
  
  lcd.setCursor(0, 1);
 
  PrintNumericValue(hours);
  lcd.print(':');

  PrintNumericValue(minutes);
 
  lcd.print(':');

  PrintNumericValue(seconds);

  UpdateTime();
  UpdateLeapYear();
}

void PrintNumericValue(int value)
{
    if (value >= 0)
    {
      if (value < 10)
      {
        lcd.print('0');
      }
      
      lcd.print(value);
    }
    else
    {
      return;
    }
}

// read the buttons
byte read_LCD_buttons()
{
 int adc_key_in = analogRead(0);
 
 if (adc_key_in > 1000)
 {
  return btnNONE;
 }

 if (adc_key_in < 50)  // approx 0
 {
  return btnRIGHT;
 }

 if (adc_key_in < 250)  //aprox 141, 142
 {
   return btnUP;
 }
 
 if (adc_key_in < 450) //aprox 326
{
   return btnDOWN;
}
 
 if (adc_key_in < 650)   //approx 501, 502
 {
   return btnLEFT;
 }
 
 if (adc_key_in < 850)   //aprox 739/740
 {
  return btnSELECT;
 }

 return btnNONE;  // when all others fail, return this... 
}
