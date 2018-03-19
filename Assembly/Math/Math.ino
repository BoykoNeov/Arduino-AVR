extern "C" {
  unsigned int MultiplyTwo8UnsignedBitValuesUsingMul(uint8_t a, uint8_t b);
  signed int MultiplyTwo8SsignedBitValuesUsingMuls(int8_t a, int8_t b);
}

long functionToCall = 0;
long firstVariable = 0;
long secondVariable = 0;
bool functionToCallRead = false;
bool firstVariableRead = false;
bool secondVariableRead = false;
 
char inData[20]; // Allocate some space for the string
char inChar = 1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

char buffer[200];
const char messageFunction[] PROGMEM = {"Enter number of function to execute and press enter at the end: "};
const char messageFirstVariable[] PROGMEM = {"Enter first variable and press enter at the end: "};
const char messageSecondVariable[] PROGMEM = {"Enter second variable and press enter at the end: "};

const char functionMessage1[] PROGMEM = {"1 - Multiply two 8-bit unsigned integers, using mul instruction"};
const char functionMessage2[] PROGMEM = {"2 - Multiply two 8-bit signed integers, using muls instruction"};

const char * const string_table[] PROGMEM =
{
  functionMessage1,
  functionMessage2
};

void setup()
{
 Serial.begin(9600);
}

void loop() 
{
  static boolean needPrompt = true;
  char* userInput;
  
  if (needPrompt)
  {
    char myChar;
    
    if (!functionToCallRead)
    {      
        for (byte k = 0; k < strlen_P(messageFunction); k++)
          {
            myChar =  pgm_read_byte_near(messageFunction + k);
            Serial.print(myChar);
          }

          Serial.println();
    }
    else if (!firstVariableRead)
    {
        for (byte k = 0; k < strlen_P(messageFirstVariable); k++)
          {
            myChar =  pgm_read_byte_near(messageFirstVariable + k);
            Serial.print(myChar);
          }
          
          Serial.println();
    }
    else if (!secondVariableRead)
    {
        for (byte k = 0; k < strlen_P(messageSecondVariable); k++)
          {  
            myChar =  pgm_read_byte_near(messageSecondVariable + k);
            Serial.print(myChar);
          }
          
          Serial.println();
    }

    needPrompt=false;
  }
  
  userInput= serialString();
  if (userInput!=NULL)
  {
    Serial.print("You entered: ");
    Serial.println(userInput);

    if (!functionToCallRead)
    {
      functionToCall = strtol(userInput, NULL, 0);
      functionToCallRead = true;

      strcpy_P(buffer, (char*)pgm_read_word(&(string_table[functionToCall-1]))); // Necessary casts and dereferencing 
      Serial.println(buffer);
    }
    else if (!firstVariableRead)
    {
       firstVariable = strtol(userInput, NULL, 0);
       firstVariableRead = true;
    }
    else if (!secondVariableRead)
    {
       secondVariable = strtol(userInput, NULL, 0);
       secondVariableRead = true;
    }
    
    needPrompt=true;
  }

  if (functionToCallRead && firstVariableRead && secondVariableRead)
  {
    long result = 0;
 // Serial.println(buffer); //debug check
    
    switch (functionToCall)
    {
      case 1:
      {
       byte a = (byte)firstVariable;
       byte b = (byte)secondVariable;
       result = (long)(MultiplyTwo8UnsignedBitValuesUsingMul(a, b));
      }
       break;
       

      case 2:
      {
      int8_t a = (int8_t)firstVariable;
      int8_t b = (int8_t)secondVariable;
      result = (long)MultiplyTwo8SsignedBitValuesUsingMuls(a, b);
      }
      break;
    }
    
    Serial.print("result: ");
    Serial.println(result);
    Serial.println();

    functionToCallRead = false;
    firstVariableRead = false;
    secondVariableRead = false;
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

