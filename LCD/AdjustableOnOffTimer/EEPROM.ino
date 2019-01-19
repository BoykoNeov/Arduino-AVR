//First 28 bytes of EEPROM will be used for set timer values
void LoadSetTimersFromEEPROM()
{
	// initial value for the sentinel bit at EEPROM start
	setTimerSentinelBit = EEPROM.read(0) >> 7;
	setTimerEEPROMPosition = 0;

	for (uint8_t i = 4; i < 24; i+=4)
	{
		if (EEPROM.read(i) >> 7 != setTimerSentinelBit) 
		{
			setTimerEEPROMPosition = i - 4;
			break;
		}

		// If all sentinel bits are the same, flip the current
		setTimerSentinelBit ^= 1;
	}

	uint16_t OnTimerFromEEPROM;
	uint16_t OffTimerFromEEPROM;
	EEPROM.get(setTimerEEPROMPosition, OnTimerFromEEPROM);
	EEPROM.get(setTimerEEPROMPosition + 2, OffTimerFromEEPROM);

	//Remove the sentinel bit
	OnTimerFromEEPROM = (OnTimerFromEEPROM << 1) >> 1;

	// Set SetTimers to values got from EEPROM (precise to minutes)
	on_Time = OnTimerFromEEPROM * 60000;
	off_Time = OffTimerFromEEPROM * 60000;
}

void SaveSetTimersToEEPROM() 
{
	setTimerEEPROMPosition += 4;
	
	if (setTimerEEPROMPosition >= 24) 
	{
		setTimerEEPROMPosition = 0;
		setTimerSentinelBit ^= 1;
	}

	// include the sentinel bit
	uint16_t sentinelBit = setTimerSentinelBit;
	sentinelBit = sentinelBit << 15;
	uint16_t onTimerToEEPROM = (uint16_t)(on_Time / 60000) + sentinelBit;

	EEPROM.put(setTimerEEPROMPosition, onTimerToEEPROM);
	EEPROM.put(setTimerEEPROMPosition + 2, (uint16_t)(off_Time / 60000));
}

// the remaining of the EEPROM will be used for active timers
void LoadActiveTimersAndLastONOFFstateFromEEPROM() 
{
	// initial value for the sentinel bit at EEPROM start
	activeTimerSentinelBit = EEPROM.read(24) >> 7;
	activeTimerEEPROMPosition = 24;

	for (uint8_t i = 28; i < 1024; i += 4)
	{
		if (EEPROM.read(i) >> 7 != activeTimerSentinelBit)
		{
			activeTimerEEPROMPosition = i - 4;
			break;
		}

		// If all sentinel bits are the same, flip the current
		activeTimerSentinelBit ^= 1;
	}

	uint16_t OnTimerFromEEPROM;
	uint16_t OffTimerFromEEPROM;
	EEPROM.get(setTimerEEPROMPosition, OnTimerFromEEPROM);
	EEPROM.get(setTimerEEPROMPosition + 2, OffTimerFromEEPROM);

	//Remove the sentinel bit
	OnTimerFromEEPROM = (OnTimerFromEEPROM << 1) >> 1;

	// Set Active state to the data from EEPROM
	lastActiveState = OffTimerFromEEPROM >> 15;

	//Remove the On/Off state bit
	OffTimerFromEEPROM = (OffTimerFromEEPROM << 1) >> 1;

	current_On_Time = OnTimerFromEEPROM * 60000;
	current_Off_Time = OffTimerFromEEPROM * 60000;
}

void SaveActiveTimersToEEPROM()
{
	activeTimerEEPROMPosition += 4;

	if (activeTimerEEPROMPosition >= 1021)
	{
		activeTimerEEPROMPosition = 0;
		activeTimerSentinelBit ^= 1;
	}

	// include the sentinel bit
	uint16_t sentinelBit = activeTimerSentinelBit;
	sentinelBit = sentinelBit << 15;
	uint16_t onTimerToEEPROM = on_Time + sentinelBit;

	EEPROM.put(activeTimerEEPROMPosition, onTimerToEEPROM);


	uint8_t activePosition = currentTimerOnOFFState;

	if (activePosition != 0 || activePosition != 1) 
	{
		activePosition = 0;
	}

	activePosition = activePosition << 15;

	uint16_t offTimerToEEPROM = (uint16_t)(current_Off_Time / 60000) + activePosition;
	EEPROM.put(activeTimerEEPROMPosition + 2, (uint16_t)(offTimerToEEPROM / 60000));
}