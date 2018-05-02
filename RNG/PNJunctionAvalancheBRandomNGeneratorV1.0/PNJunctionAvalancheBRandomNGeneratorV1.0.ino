// include the library code:
#include <LiquidCrystal.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>

const byte resistorSelect = 8; //rs select
const byte enable = 9; //enable
const byte d4 = 4; //d4
const byte d5 = 5; //d5
const byte d6 = 6; //d6
const byte d7 = 7; //d7

long totalCount = 0;
int serialMaxValue = 0;
uint64_t totalReadValues = 0;
uint16_t printCount = 0;
uint32_t totalPrintCount = 0;
byte pressedButton = 0;
volatile uint32_t seed;
volatile int8_t nrot;
char randomChars[32];
byte randomCharsIndex = 0;

#define randomSeed(s) srandom(s)
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons()
{
 int adc_key_in;
 adc_key_in = analogRead(A0);      // read the value from the sensor 
 if (adc_key_in > 1000) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE;
}

LiquidCrystal lcd(resistorSelect, enable, d4, d5, d6, d7);

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(A2,INPUT);
  CreateTrulyRandomSeed();
  randomSeed(seed);
}

void loop() 
{
  byte outputByte = 0;

  for (byte j = 0; j < 8; j++)
   {
    byte firstBit = 0;
    byte secondBit = 0;
  
    // implementing Von Neumann box - the two bits must be different in order to output something
    while (firstBit == secondBit)
      {
        unsigned int first = analogRead(2);
        totalCount++;
        totalReadValues += first;
        if (first > serialMaxValue)
          {
            serialMaxValue = first;
          }

  
        unsigned int second = analogRead(2);
        totalCount++;
        totalReadValues += second;
        if (second > serialMaxValue)
          {
            serialMaxValue = second;
          }
  
       firstBit = bitRead(first, 0);
       secondBit = bitRead(second,0);
      }

    bitWrite(outputByte, j, secondBit);
   } 

  outputByte ^= random(0, 256);
  printCount++;
  Serial.println(outputByte);

  // fill an array of randomCharsForDisplay

  if (outputByte > 32 && outputByte < 127)
  {
    randomChars[randomCharsIndex] = outputByte;
    randomCharsIndex++;

    if (randomCharsIndex > 31)
    {
      randomCharsIndex = 0;
    } 
  }

//  if (printCount == 1000)
//  {
//    totalPrintCount += printCount;
//    lcd.clear();
//    printCount = 0;
//    lcd.setCursor(0,0);
//    lcd.print("T:");
//    lcd.print(totalCount);
//    lcd.setCursor(0,1);
//    lcd.print("p:");
//    lcd.print(totalPrintCount);
//    lcd.print(' ');
//    lcd.print("m:");
//    lcd.print(serialMaxValue);
//  }
 //alternatively print random chars
   if (printCount == 1000)
  {
    totalPrintCount += printCount;
    lcd.clear();
    printCount = 0;
    lcd.setCursor(0,0);

      for(byte i = 0; i < 16; i++)
      {
        lcd.print(randomChars[i]);
      }
    
    lcd.setCursor(0,1);

      for(byte i = 16; i < 32; i++)
      {
        lcd.print(randomChars[i]);
      }
  }
}

void CreateTrulyRandomSeed()
{
  seed = 0;
  nrot = 32; // Must be at least 4, but more increased the uniformity of the produced 
             // seeds entropy.
  
  // The following five lines of code turn on the watch dog timer interrupt to create
  // the seed value
  cli();                                             
  MCUSR = 0;                                         
  _WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT) | (1<<WDE); 
  _WD_CONTROL_REG = (1<<WDIE);                       
  sei();                                             
 
  while (nrot > 0);  // wait here until seed is created
 
  // The following five lines turn off the watch dog timer interrupt
  cli();                                             
  MCUSR = 0;                                         
  _WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT) | (0<<WDE); 
  _WD_CONTROL_REG = (0<< WDIE);                      
  sei();                                             
}
 
ISR(WDT_vect)
{
  nrot--;
  seed = seed << 8;
  seed = seed ^ TCNT1L;
}
