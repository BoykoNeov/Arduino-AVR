#include <mem.h>

char inData[20]; // Allocate some space for the string
char inChar = 1; // Where to store the character read
// uint8_t index = 0; // Index into array; where to store the character
int16_t intvar = 0;
void setup()
{
 Serial.begin(9600);
}

void loop() 
{
  static boolean needPrint = false;
  static boolean needPrompt = true;
  String userInput;
  
  if (needPrompt)
  {
    Serial.print("Please enter inputs and press enter at the end:\n");
    needPrompt=false;
  }
  
  userInput = serialString();

  delay(1000);
  
  if (userInput != "")
  {
    Serial.print("You entered: ");
    Serial.println(userInput);
    intvar = userInput.toInt();
    needPrompt = true;
    needPrint = true;
  }

  if (needPrint = true)
  {  
    // Declaration
    int* myArray = 0;
    int myArraySize = 0;

    myArraySize = intvar;
    
    // Allocation (let's suppose size contains some value discovered at runtime,
    // e.g. obtained from some external source)
    if (myArray != 0) 
      {
          myArray = (int*) os_realloc(myArray, myArraySize * sizeof(int));
      } 
    else 
      {
          myArray = (int*) os_malloc(myArraySize * sizeof(int));
      }

      for (uint32_t i = 0; i < myArraySize ; i++)
      {
        myArray[i] = i;
        Serial.print(myArray[i]);
        yield();
      }

      Serial.println();
      Serial.println(ESP.getFreeHeap());

     needPrint = false;
     os_free(myArray);
  }

  TestThis();

  Serial.println("Nachalo");
  Serial.println(ESP.getFreeHeap());
  TestThis2();
  Serial.println("FINAL");
  Serial.println(ESP.getFreeHeap());
  delay(900);
}

String serialString()
{
  static char str[21]; // For strings of max length=20
  
  if (!Serial.available())
  {
    return String("");
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
  return String(str);
}

