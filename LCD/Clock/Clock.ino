// include the library code:
#include <LiquidCrystal.h>
const byte resistorSelect = 8; //rs select
const byte enable = 9; //enable
const byte d4 = 4; //d4
const byte d5 = 5; //d5
const byte d6 = 6; //d6
const byte d7 = 7; //d7

unsigned int hours = 0;
unsigned int minutes = 0;
unsigned int seconds = 0;

unsigned int month = 0;
unsigned int monthDay = 1;
unsigned int year = 2018;

char* monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
byte monthLengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned long oldMillis = 0;
unsigned long currentMillis = 0;
bool isLeapYear = false;

const byte blackChar[8] = {
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
int editActiveSecondsCounter = 0;

LiquidCrystal lcd(resistorSelect, enable, d4, d5, d6, d7);

#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   4
#define btnLEFT   3
#define btnSELECT 5
#define btnNONE   0

byte oldPressedButton = 0;
byte currentPressedButton = 0;

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.createChar(0, blackChar);
  
  CheckForLeapYear();
  Serial.begin(9600);
}

void loop() 
{
   if (editSwitch)
   {
     lcd.setCursor(12,0);
     lcd.print("EDIT");
   }
   else
   {
    lcd.setCursor(12,0);
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

  if (seconds % 2 == 0)
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

  if(minutes < 10)
  {
    lcd.print('0');
  }
 
 lcd.print(minutes);
 lcd.print(':');

 if (seconds < 10)
 {
  lcd.print('0');
 }
 
 lcd.print(seconds);

  // prints the character defined as 0
  // lcd.write((byte)0);

  //Update time
  currentMillis = millis();

  if (currentMillis >= oldMillis + 1000)
  {
    seconds++;
    oldMillis += 1000;

    if (editSwitch)
    {
         editActiveSecondsCounter++; 
    }
  }

//Deactivate edit switch after 20 seconds
  if (editActiveSecondsCounter >= 20)
  {
    editActiveSecondsCounter = 0;
    editSwitch = false;
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
  }

  if (month > 11)
   {
    year++;
    month = 0;
    CheckForLeapYear();
   }
}

void EditDateTime(byte pressedButtonNumber)
{
  int changingValue;

  if (blackCursorRow == 0)
  {
    if (blackCursorColumn <= 4)
    {
     ChangeValue(year, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 8)
    {
      ChangeValue(month, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 10)
    {
      ChangeValue(monthDay, pressedButtonNumber);
    }
    else
    {
      return;
    }
  }
  else if (blackCursorRow == 1)
  {
    if (blackCursorColumn <= 2)
    {
      ChangeValue(hours, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 5)
    {
      ChangeValue(minutes, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 7)
    {
      ChangeValue(seconds, pressedButtonNumber);
    }
    else
    {
      return;
    }
  }
  else
  {
    return;
  }
}

void ChangeValue(unsigned int &value, byte pressedButton)
{
    if (pressedButton == 0)
  {
    return;
  }
  else if(pressedButton == 1) //right
  {
    return;
  }
  else if(pressedButton == 2) // up
  {
    value++;
  }
  else if(pressedButton == 3) // left
  {
    return;
  }
  else if(pressedButton == 4) // down
  {
    value--;
  }
  else if(pressedButton == 5) // select
  {   
    selectSwitch = true;
    editSwitch = false;
  }
}

void MoveSelector(byte pressedButtonNumber)
{
  if (pressedButtonNumber == 0)
  {
    return;
  }
  else if(pressedButtonNumber == 1) //right
  {
    blackCursorColumn++;
    FixBlackCursorColumn();   
  }
  else if(pressedButtonNumber == 2) // up
  {
    blackCursorRow--;
    FixBlackCursorRow();
  }
  else if(pressedButtonNumber == 3) // left
  {
    blackCursorColumn--;
    FixBlackCursorColumn();   
  }
  else if(pressedButtonNumber == 4) // down
  {
    blackCursorRow++;
    FixBlackCursorRow();
  }
  else if(pressedButtonNumber == 5) // select
  {   
    selectSwitch = !selectSwitch;

    if (!selectSwitch)
    {
      ClearLastBlackCursor();
      editSwitch = !editSwitch;
    }
    else
    {
      editSwitch = false;
    }
  }
}

 void ClearLastBlackCursor()
 {
    lcd.setCursor(lastBlackCursorColumn, lastBlackCursorRow);
    lcd.print(' ');
 }


void FixBlackCursorColumn()
{  
  if (blackCursorColumn < 0)
  {
     blackCursorColumn = 15;
  }

  if (blackCursorColumn > 15)
  {
    blackCursorColumn = 0;
  }
}

void FixBlackCursorRow()
{  
  if (blackCursorRow < 0)
  {
     blackCursorRow = 1;
  }

  if (blackCursorRow > 1)
  {
    blackCursorRow = 0;
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
  else
  {
    monthLengths[1] = 28;
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
