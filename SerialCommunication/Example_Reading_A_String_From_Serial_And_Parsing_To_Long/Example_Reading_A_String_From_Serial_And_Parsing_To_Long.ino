char inData[20]; // Allocate some space for the string
char inChar = 1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

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
    Serial.print("Please enter inputs and press enter at the end:\n");
    needPrompt=false;
  }
  
  userInput= serialString();
  if (userInput!=NULL)
  {
    Serial.print("You entered: ");
    Serial.println(userInput);
    long longvar = atol(userInput);
    longvar /= 2;
    Serial.print("result divided by 2: ");
    Serial.println(longvar);
    Serial.println();
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

