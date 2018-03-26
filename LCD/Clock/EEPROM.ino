// Determines last index of free EEPROM, after saved data in the EEPROM. If it could not be determined, returns EEPROM length + 1;
unsigned int DetermineFirstFreeEEPROMIndex()
{
  unsigned int EEPROMlength = EEPROM.length();
  unsigned int resultEEPROM = 0;
  bool resultFound = false;

 sentinelBit = EEPROM.read(5) >> 7;

  //determine starting sentinel bit
  for (uint16_t index = 11; index < EEPROMlength - 6; index+= 6)
  {
    if ((EEPROM.read(index) >> 7) != sentinelBit)
    {
      resultFound = true;
      resultEEPROM = index - 5;
      break;
    }
  }

// This method, of searching of zeros and erasing the EEPROM to 0 after filling up works, but let's try to do it with a sentinel bit embedded in the minutes, in order to diminish the number of write cycles
//  for(unsigned int index = 5; index < EEPROMlength; index += 6)
//  {
//    // for debugging
//    byte byte1 = EEPROM.read(index - 5);
//    byte byte2 = EEPROM.read(index - 4);
//    byte byte3 = EEPROM.read(index - 3);
//    byte byte4 = EEPROM.read(index - 2);
//    byte byte5 = EEPROM.read(index - 1);
//    byte byte6 = EEPROM.read(index);
//    
//    if ( byte1 == 0 &&
//         byte2 == 0 &&
//         byte3 == 0 &&
//         byte4 == 0 &&
//         byte5 == 0 &&
//         byte6 == 0)
//        {
//          resultFound = true;
//          result = index - 5;
//          break;
//        }
//  }

  if (resultFound)
  {
    return resultEEPROM;
  }
  else
  {
    return EEPROMlength + 1;
  }
}

void ResetEEPROMIndex()
{
  sentinelBit ^= 1;
  EEPROMFreeIndex = 0;
}

void WriteDateTimeToEEPROM(unsigned int startingIndex)
{
  byte yearHighByte = highByte(year);
  byte yearLowByte = lowByte(year);
  byte monthByte = (byte)month;
  byte dayByte = (byte)monthDay;
  byte hoursByte = (byte)hours;
  byte minutesByte = 0;
  minutesByte += sentinelBit << 7;
  minutesByte += (byte)minutes;

  EEPROM.write(startingIndex, yearHighByte);
  EEPROM.write(startingIndex + 1, yearLowByte);
  EEPROM.write(startingIndex + 2, monthByte);
  EEPROM.write(startingIndex + 3, dayByte);
  EEPROM.write(startingIndex + 4, hoursByte);
  EEPROM.write(startingIndex + 5, minutesByte);

  EEPROMFreeIndex += 6;
}

void ReadDateTimeFromEEPROM(unsigned int startingIndex)
{
  byte yearHighByte = EEPROM.read(startingIndex);
  byte yearLowByte = EEPROM.read(startingIndex + 1);

  year = (int)(((unsigned int)yearHighByte << 8) + yearLowByte);

  month = EEPROM.read(startingIndex + 2);
  monthDay = EEPROM.read(startingIndex + 3);
  hours = EEPROM.read(startingIndex + 4);
  byte minutesByte = EEPROM.read(startingIndex + 5);
  minutesByte &= 0b01111111;   //clear the sentinel bit
  minutes = minutesByte;
}

//void EraseEEPROM()
//{
//  int EEPROMlength = EEPROM.length();
//  
//  for(int index = 0; index < EEPROMlength; index++)
//  {
//    EEPROM.write(index, 0);
//  }
//}
