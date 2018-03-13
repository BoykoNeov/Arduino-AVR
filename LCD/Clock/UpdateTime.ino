void UpdateTime()
{
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
    else
    {
      // Make it zero just in case if the editing mode is off
      editActiveSecondsCounter = 0;
    }

    //Repeat with the select switch
    if (selectSwitch)
    {
      selectActiveSecondsCounter++;
    }
    else
    {
      selectActiveSecondsCounter = 0;
    }
  }

//Deactivate edit switch after 20 seconds if active
  if (editActiveSecondsCounter >= 20)
  {
    editActiveSecondsCounter = 0;
    editSwitch = false;
  }

  if (selectActiveSecondsCounter >= 20)
  {
    selectActiveSecondsCounter = 0;
    selectSwitch = false;
  }

  if (seconds > 59)
  {
    seconds = 0;
    minutes++;

    //this usually means that a minute has passed naturally in clock operation, but to avoid writing to EEPROM when manually editing, first check that editSwitch is off
    // Write Date/Time every 2 minutes
    if (!editSwitch && minutes % 2 == 0)
    {
      // if this is true, means EEPROM is fully written
      // if EEPROM.length() returns 1024 (as it is on 382p), the last byte is 1023, and we need at least 6 bytes for the Date/Time, this is byte 1018, so 1024 - 6 = 
      if (EEPROMFreeIndex > EEPROM.length() - 6)
        {
          EraseEEPROM();
          EEPROMFreeIndex = 0;
        }

       WriteDateTimeToEEPROM(EEPROMFreeIndex);
    }
  }
  else if (seconds < 0)
  {
    seconds = 59;
    minutes--;
  }

  if (minutes > 59)
  {
    minutes = 0;
    hours++;
  }
  else if(minutes < 0)
  {
    minutes = 59;
    hours--;
  }
    
  if (hours > 23)
  {
    hours = 0;
    monthDay++;
  }
  else if(hours < 0)
  {
    hours = 23;
    monthDay--;
  }

  if (monthDay > monthLengths[month])
  {
    month++;
    monthDay = 1;
  }
  else if (monthDay < 1)
  {
    month--;

    if (month < 0)
    {
      monthDay = monthLengths[11];
    }
    else
    {
      monthDay = monthLengths[month];
    }
  }

  if (month > 11)
   {
    year++;
    month = 0;
   }
   else if (month < 0)
   {
    year--;
    month = 11;
   }
}

void UpdateLeapYear()
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

bool IsLeapYear(int year)
{
  bool result = false;

  bool yearCanBeDividedBy4 = year % 4 == 0;
  bool yearCanBeDividedBy100 = year % 100 == 0;
  bool yearCanBeDividedBy400 = year % 400 == 0;
  
  if (yearCanBeDividedBy400)
  {
    result = true;
  }
  else if (yearCanBeDividedBy100)
  {
    result = false;
  }
  else if (yearCanBeDividedBy4)
  {
    result = true;  
  }
  else
  {
    result = false;
  }

  return result;
}

// determines the day of week for a given date, 0 - sun, 6 - sat;
byte ReturnDayOfWeek(int givenYear, int givenMonth, int givenDay)
{
  byte monthLengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  bool leapYear = IsLeapYear(givenYear);
  int result = WhatDayIsFirstJan(givenYear);

  for (byte i = 0; i < givenMonth; i++)
  {
    result += monthLengths[i];
  }

//because if we add all the days, we count one day twice
  result += givenDay - 1;

  if (leapYear && givenMonth > 1)
  {
      result++;
  }
  
  result %= 7;
  return (byte)result;
}

// determines the day of week for first January of a given date, 0 - sun, 6 - sat;
byte WhatDayIsFirstJan(int givenYear)
{
  unsigned long year = (unsigned long)givenYear;
  byte day = (byte)((year * 365 + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400)) % 7);

  return day;
}

