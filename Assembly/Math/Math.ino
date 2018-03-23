#include <math.h>

extern "C" {
  uint16_t MultiplyTwo8UnsignedBitValuesUsingMul(uint8_t a, uint8_t b);
  int16_t MultiplyTwo8SsignedBitValuesUsingMuls(int8_t a, int8_t b);
  int16_t Multiply8bitSignedWithUnsignedUsingMulsu(int8_t a, uint8_t b);
  uint16_t Multiply8BitBy2UsingLslAndRol(uint8_t a, uint8_t b);
  uint16_t ManualMultiplyTwo8BitNumbers(uint8_t a, uint8_t b);
  uint32_t MultiplyTwo16BitUnsignedIntsUsingMul(uint16_t a, uint16_t b);
  uint32_t ManualMultiplyTwo16BitUnsignedInts(uint16_t a, uint16_t b);
  uint64_t ManuallyMultiplyTwo32BitUnsignedInts(uint32_t a, uint32_t b);
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
const char functionMessage3[] PROGMEM = {"3 - Multiply one 8-bit signed int with one 8-bit unsigned int, using mulsu instruction"};
const char functionMessage4[] PROGMEM = {"4 - Multiply one 8-bit unsigned int by 2 using lsl and rol instrucions - second variable - how many times to repeat the operation"};
const char functionMessage5[] PROGMEM = {"5 - Multiply two 8-bit unsigned ints manually"};
const char functionMessage6[] PROGMEM = {"6 - Multiply two 16-bit unsigned ints using mul)"};
const char functionMessage7[] PROGMEM = {"7 - Multiply two 16-bit unsigned ints manually"};
const char functionMessage8[] PROGMEM = {"8 - Multiply two 32-bit unsigned ints manually"};

const char * const string_table[] PROGMEM =
{
  functionMessage1,
  functionMessage2,
  functionMessage3,
  functionMessage4,
  functionMessage5,
  functionMessage6,
  functionMessage7,
  functionMessage8
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
    bool answerIsSigned = false;
    uint64_t unsignedResult = 0;
    int64_t signedResult = 0;
    
    switch (functionToCall)
    {
      case 1:
      {
       byte a = (byte)firstVariable;
       byte b = (byte)secondVariable;
       unsignedResult = MultiplyTwo8UnsignedBitValuesUsingMul(a, b);
      }
       break;
       
      case 2:
      {
      int8_t a = (int8_t)firstVariable;
      int8_t b = (int8_t)secondVariable;
      signedResult = MultiplyTwo8SsignedBitValuesUsingMuls(a, b);
      answerIsSigned = true;
      break;
      }

      case 3:
      {
        int8_t a = (int8_t)firstVariable;
        uint8_t b = (uint8_t)secondVariable;
        signedResult = Multiply8bitSignedWithUnsignedUsingMulsu(a, b);
        answerIsSigned = true;
        break;
      }
      
      case 4:
      {
        uint8_t a = (uint8_t)firstVariable;
        uint8_t b = (uint8_t)secondVariable;
        unsignedResult = Multiply8BitBy2UsingLslAndRol(a, b);
        break;
      }

      case 5:
      {
        uint8_t a = (uint8_t)firstVariable;
        uint8_t b = (uint8_t)secondVariable;
        unsignedResult = ManualMultiplyTwo8BitNumbers(a, b);
        break;
      }

      case 6:
      {
        uint16_t a = (uint16_t)firstVariable;
        uint16_t b = (uint16_t)secondVariable;
        unsignedResult = MultiplyTwo16BitUnsignedIntsUsingMul(a, b);
        break;
      }

      case 7:
      {
        uint16_t a = (uint16_t)firstVariable;
        uint16_t b = (uint16_t)secondVariable;
        unsignedResult = ManualMultiplyTwo16BitUnsignedInts(a, b);
        break;
      }

      case 8:
      {
       uint32_t a = (uint32_t)firstVariable;
       uint32_t b = (uint32_t)secondVariable;
       unsignedResult = ManuallyMultiplyTwo32BitUnsignedInts(a, b);
        break;
      }

      default:
      break;
    }
    
    Serial.print("result: ");

    if (answerIsSigned)
    {      
      if (signedResult < 0)
      {
        unsignedResult = (uint64_t)(signedResult * -1);
        Serial.print('-');
      }
      else
      {
        unsignedResult = (uint64_t)(signedResult);
      }
    }

    SerialPrintBig(unsignedResult);
    Serial.println();
      
    // the code bellow doesnt work
    // int length = snprintf( NULL, 0, "%u", unsignedResult );
    // char* str = malloc( length + 1 );
    // snprintf( str, length + 1, "%u", unsignedResult);
    // Serial.print(str);    
    // Serial.println(unsignedResult);

    functionToCallRead = false;
    firstVariableRead = false;
    secondVariableRead = false;
  }
}

void SerialPrintBig(uint64_t value)
{
    const int NUM_DIGITS    = log10(value) + 1;
    char sz[NUM_DIGITS + 1];
    
    sz[NUM_DIGITS] =  0;
    for ( size_t i = NUM_DIGITS; i--; value /= 10)
    {
        sz[i] = '0' + (value % 10);
    }
    
    Serial.print(sz);
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

