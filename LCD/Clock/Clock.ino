// include the library code:
#include <LiquidCrystal.h>
const byte resistorSelect = 8; //rs select
const byte enable = 9; //enable
const byte d4 = 4; //d4
const byte d5 = 5; //d5
const byte d6 = 6; //d6
const byte d7 = 7; //d7

byte hours = 21;
byte minutes = 55;
byte seconds = 0;

byte month = 3;
byte monthDay = 11;
unsigned int year = 2018;

char* monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
byte monthLengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned long oldMillis = 0;
unsigned long currentMillis = 0;
bool isLeapYear = false;

LiquidCrystal lcd(resistorSelect, enable, d4, d5, d6, d7);

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  CheckForLeapYear();
}

void loop() 
{
  // set the cursor to column 0, line 1

lcd.setCursor(0,0);
  lcd.print(year);
  lcd.print("/");
  lcd.print(monthNames[month]);
  lcd.print("/");

  if (monthDay < 10)
  {
    lcd.print('0');
  }
  
  lcd.print(monthDay);

  lcd.setCursor(0, 1);

  if(hours < 10)
  {
    lcd.print('0');
  }
 
 lcd.print(hours);
 lcd.print(':');
 lcd.print(minutes);
 lcd.print(':');

 if (seconds < 10)
 {
  lcd.print('0');
 }
 
 lcd.print(seconds);
 
  
  // print the number of milis since reset:
 // lcd.print(millis());
  lcd.print(" ");

  //Update time
  currentMillis = millis();

  if (currentMillis >= oldMillis + 1000)
  {
    seconds++;
    oldMillis += 1000;
  }

  if (seconds > 59)
  {
    seconds = 0;
    minutes++;
  }

  if (minutes > 59)
  {
    minutes = 0;
    hours++;
  }

  if (hours > 23)
  {
    hours = 0;
    monthDay++;
  }

  if (monthDay > monthLengths[month])
  {
    month++;
    monthDay = 1;

    if (month > 12)
    {
      year++;
      month = 1;
      CheckForLeapYear();
    }
  }
}

void CheckForLeapYear()
{
  bool yearCanBeDividedBy4 = year % 4 == 0;
  bool yearCanBeDividedBy100 = year % 100 == 0;
  bool yearCanBeDividedBy400 = year % 400 == 0;

  if (yearCanBeDividedBy400)
  {
    isLeapYear = true;
  }
  else if (yearCanBeDividedBy100)
  {
    isLeapYear = false;
  }
  else if (yearCanBeDividedBy4)
  {
    isLeapYear = true;  
  }
  else
  {
    isLeapYear = false;
  }

  if (isLeapYear)
  {
    monthLengths[1] = 29;
  }
}
