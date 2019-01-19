void ReadTimersFromEEPROM() 
{
	for (uint16_t i = 0; i <= 1008; i += 14)
	{
		uint8_t readByte = EEPROM.read(i);
		currentEEPPROMposition = i;

		if (readByte == 255) 
		{
			currentEEPPROMposition = i;
			break;
		}
	}

	uint32_t setOn;
	uint32_t setOff;
	uint16_t currentOnMinutes;
	uint16_t currentOffMinutes;
	uint8_t activeState;

	EEPROM.get(currentEEPPROMposition + 1, activeState);
	EEPROM.get(currentEEPPROMposition + 2, setOn);
	EEPROM.get(currentEEPPROMposition + 6, setOff);
	EEPROM.get(currentEEPPROMposition + 10, currentOnMinutes);
	EEPROM.get(currentEEPPROMposition + 12, currentOffMinutes);
	lastActiveState = activeState;

	if (lastActiveState >= 2) 
	{
		lastActiveState = 0;
	}

	// If due to bug or anything, the set timers are less than a second, set the off timer to 4 weeks, and the on timer to one second
	if (setOff < 1000) 
	{
		setOff = 2419200000;
	}

	if (setOn < 1000) 
	{
		setOn = 1000;
	}

	on_Time = setOn;
	off_Time = setOff;

	// if the current timers are less than one minute, set them to set_timers
	if (currentOnMinutes == 0) 
	{
		current_On_Time = on_Time;
	}
	else 
	{
		current_On_Time = (uint32_t)currentOnMinutes * 60000;
	}

	if (currentOffMinutes == 0) 
	{
		current_Off_Time = off_Time;
	}
	else 
	{
		current_Off_Time = (uint32_t)currentOffMinutes * 60000;
	}
}

void SaveTimersToEEPROM() 
{
	currentEEPPROMposition += 14;

	if (currentEEPPROMposition >= 1009) 
	{
		// StartFromBeginning
		currentEEPPROMposition = 0;
	}

	//Erase previous position indicator
	EEPROM.write(currentEEPPROMposition - 14, 0);

	uint32_t setOn_ = on_Time;
	uint32_t setOff_ = off_Time;
	uint16_t currentOnMinutes = current_On_Time / 60000;
	uint16_t currentOffMinutes = current_Off_Time / 60000;
	uint8_t activeState = currentTimerOnOFFState;

	//Mark current position
	EEPROM.write(currentEEPPROMposition, 255);
	//Put data into EEPROM
	EEPROM.put(currentEEPPROMposition + 1, activeState);
	EEPROM.put(currentEEPPROMposition + 2, setOn_);
	EEPROM.put(currentEEPPROMposition + 6, setOff_);
	EEPROM.put(currentEEPPROMposition + 10, currentOnMinutes);
	EEPROM.put(currentEEPPROMposition + 12, currentOffMinutes);
}