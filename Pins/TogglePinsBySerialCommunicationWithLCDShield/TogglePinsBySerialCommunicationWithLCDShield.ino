#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

char inData[20]; // Allocate some space for the string
char inChar = 1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
byte pinToToggle;

void setup() {

pinMode(A0, OUTPUT);
digitalWrite(A0, LOW);

pinMode(A1, OUTPUT);
digitalWrite(A1, LOW);

pinMode(A2, OUTPUT);
digitalWrite(A2, LOW);

pinMode(A3, OUTPUT);
digitalWrite(A3, LOW);

pinMode(A4, OUTPUT);
digitalWrite(A4, LOW);

pinMode(A5, OUTPUT);
digitalWrite(A5, LOW);

Serial.begin(9600);
lcd.begin(16, 2);             
lcd.setCursor(0,0);
}

void loop() 
{
  lcd.print(millis() / 1000);
  lcd.setCursor(0,0);
  static boolean needPrompt = true;
  char* userInput;
  
  if (needPrompt)
  {
    Serial.print("Please enter inputs and press enter at the end:\n");
    needPrompt=false;
  }
  
  userInput = serialString();
  if (userInput!=NULL)
  {
    Serial.print("You entered: ");
    Serial.println(userInput);
    pinToToggle = strtol(userInput, NULL, 0);
    pinMode(pinToToggle, OUTPUT);
    digitalWrite(pinToToggle, !digitalRead(pinToToggle));
    Serial.print("Pin ");
    Serial.print(pinToToggle);
    Serial.print(" is ");
    Serial.println(digitalRead(pinToToggle));
    needPrompt=true;
  }
}

char* serialString()
{
  static char str[21]; // For strings of max length=20
  
  if (!Serial.available())
  {
    return NULL;
  }
  
  delay(64); // wait for all characters to arrive
  memset(str,0,sizeof(str)); // clear str
  byte count=0;
  
  while (Serial.available())
  {
    char c=Serial.read();
    if (c>=32 && count<sizeof(str)-1)
    {
      str[count]=c;
      count++;
    }
  }
  
  str[count]='\0'; // make it a zero terminated string
  return str;
}
