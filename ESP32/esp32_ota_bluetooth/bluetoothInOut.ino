char* ReadBluetooth()
{
  static char str[51]; // For strings of max length=20
  
  if (!SerialBT.available())
  {
    return NULL;
  }
  
  memset(str,0,sizeof(str)); // clear str
  byte count=0;
  
  while (SerialBT.available())
  {
    char c=SerialBT.read();
    
    if (c>=32 && count < sizeof(str)-1)
    {
      str[count]=c;
      count++;
    }
    
    delay(2);
  }
  
  str[count]='\0'; // make it a zero terminated string
  return str;
}

void WriteBluetooth(char* message)
{
}
