#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// starts with 1 hour off and on
uint32_t on_Time = 3600000;
uint32_t off_Time = 3600000;
uint32_t current_On_Time = on_Time;
uint32_t current_Off_Time = off_Time;
uint8_t lastKeyPressed = btnNONE;
uint32_t timeOfLastButtonAction = 0;
uint32_t timeChangeMultiplier = 1;
uint8_t currentTimerOnOFFState = 2; // 1 on, 0 off, 2 - off , timers not counting
uint32_t millisOfLastTimeChange = 0;

// left pointing dense arrow 
const byte customChar[] PROGMEM = {
  B00001,
  B00011,
  B00111,
  B01111,
  B01111,
  B00111,
  B00011,
  B00001
};

void setup()
{
	delay(60);
	lcd.begin(16, 2);
	delay(60);
	lcd.clear();
	delay(60);

	uint8_t customCharOne[8];
	for (size_t i = 0; i < 8; i++)
	{
		customCharOne[i] = pgm_read_word_near(customChar + i);
	}

	lcd.createChar(0, customCharOne);
	Serial.begin(9600);
}

void loop()
{
	// Change Timers
	uint32_t currentMillis = millis();
	if (currentMillis - millisOfLastTimeChange >= 1000)
	{
		millisOfLastTimeChange = currentMillis;

		if (currentTimerOnOFFState == 0)
		{
			if (current_Off_Time > 1000)
			{
				current_Off_Time -= 1000;
			}
			else
			{
				current_Off_Time = off_Time;
				currentTimerOnOFFState = 1;
			}
		}
		else if (currentTimerOnOFFState == 1)
		{
			if (current_On_Time > 1000)
			{
				current_On_Time -= 1000;
			}
			else
			{
				current_On_Time = on_Time;
				currentTimerOnOFFState = 0;
			}
		}
	}

	// If currently the timers are running, they are diplsayed and they are modified
	if (currentTimerOnOFFState == 0 || currentTimerOnOFFState == 1)
	{
		lcd.setCursor(14, 0);
		lcd.print("ON");
		lcd.setCursor(13, 1);
		lcd.print("OFF");

		lcd.setCursor(0, 0);

		uint32_t current_On_Time_in_Seconds = current_On_Time / 1000;
		uint16_t current_On_Time_Hours = current_On_Time_in_Seconds / 3600;
		uint8_t current_Remaining_On_Minutes = (current_On_Time_in_Seconds % 3600) / 60;
		uint8_t current_Remaining_On_Seconds = (current_On_Time_in_Seconds % 3600) % 60;

		uint32_t current_Off_Time_in_Seconds = current_Off_Time / 1000;
		uint16_t current_Off_Time_Hours = current_Off_Time_in_Seconds / 3600;
		uint8_t currentRemainingOFFMinutes = (current_Off_Time_in_Seconds % 3600) / 60;
		uint8_t currentRemainingOFFSeconds = (current_Off_Time_in_Seconds % 3600) % 60;

		if (current_On_Time_Hours < 100)
		{
			lcd.print(" ");
		}
		PrintNumericValue(current_On_Time_Hours);
		lcd.print(":");
		PrintNumericValue(current_Remaining_On_Minutes);
		lcd.print(":");
		PrintNumericValue(current_Remaining_On_Seconds);

		lcd.setCursor(0, 1);
		if (current_Off_Time_Hours < 100)
		{
			lcd.print(" ");
		}
		PrintNumericValue(current_Off_Time_Hours);
		lcd.print(":");
		PrintNumericValue(currentRemainingOFFMinutes);
		lcd.print(":");
		PrintNumericValue(currentRemainingOFFSeconds);
	} // if timers are not running the timer reset time is displayed and modified
	else 
	{
		lcd.setCursor(10, 0);
		lcd.print("SET ON");
		lcd.setCursor(10, 1);
		lcd.print("SET OF");

		lcd.setCursor(0, 0);

		uint32_t on_Time_in_Seconds = on_Time / 1000;
		uint16_t on_Time_Hours = on_Time_in_Seconds / 3600;
		uint8_t remainingONMinutes = (on_Time_in_Seconds % 3600) / 60;
		uint8_t remainingONSeconds = (on_Time_in_Seconds % 3600) % 60;

		uint32_t off_Time_in_Seconds = off_Time / 1000;
		uint16_t off_Time_Hours = off_Time_in_Seconds / 3600;
		uint8_t remainingOFFMinutes = (off_Time_in_Seconds % 3600) / 60;
		uint8_t remainingOFFSeconds = (off_Time_in_Seconds % 3600) % 60;

		if (on_Time_Hours < 100)
		{
			lcd.print(" ");
		}
		PrintNumericValue(on_Time_Hours);
		lcd.print(":");
		PrintNumericValue(remainingONMinutes);
		lcd.print(":");
		PrintNumericValue(remainingONSeconds);

		lcd.setCursor(0, 1);
		if (off_Time_Hours < 100)
		{
			lcd.print(" ");
		}
		PrintNumericValue(off_Time_Hours);
		lcd.print(":");
		PrintNumericValue(remainingOFFMinutes);
		lcd.print(":");
		PrintNumericValue(remainingOFFSeconds);
	}

	if (currentTimerOnOFFState == 0)
	{
		lcd.setCursor(10, 0);
		lcd.print(" ");
		lcd.setCursor(10, 1);
		lcd.write(uint8_t(0));
	}
	else if (currentTimerOnOFFState == 1)
	{
		lcd.setCursor(10, 0);
		lcd.write(byte(0));
		lcd.setCursor(10, 1);
		lcd.print(" ");
	}
	else
	{
		lcd.setCursor(10, 0);
		lcd.print(" ");
		lcd.setCursor(10, 1);
		lcd.print(" ");
	}

	lcd_key = read_LCD_buttons();

	if (lastKeyPressed == btnNONE && lcd_key != lastKeyPressed)
	{
		timeChangeMultiplier = 1;
	}

	if (millis() - timeOfLastButtonAction < 300)
	{
		return;
	}

	if (lastKeyPressed == lcd_key && lastKeyPressed != btnNONE)
	{
		// 100 hours
		if (timeChangeMultiplier < 360000)
		{
			if (timeChangeMultiplier <= 20)
			{
				timeChangeMultiplier += 3;
			}
			else if (timeChangeMultiplier <= 1200)
			{
				timeChangeMultiplier += 60;
			}
			else
			{
				(timeChangeMultiplier += 3600);
			}
		}
	}

	switch (lcd_key)
	{

	case btnRIGHT:
	{
		uint32_t newValue = 0;
		uint32_t *pointer_to_currently_displayed_timers;

		if (currentTimerOnOFFState != 2) 
		{
			// if timers on not running, modify the current timer values
			// pointer points to curren on time
			pointer_to_currently_displayed_timers = &current_On_Time;
		}
		else 
		{
			// else modify reset values
			pointer_to_currently_displayed_timers = &on_Time;
		}

		newValue = *pointer_to_currently_displayed_timers + (1000 * timeChangeMultiplier);

		// 2419200000 == 4 weeks in miliseconds
		if (newValue < *pointer_to_currently_displayed_timers || newValue > 2419200000)
		{
			// cap/modify the value where the pointer is pointing
			*pointer_to_currently_displayed_timers = 2419200000;
		}
		else
		{
			*pointer_to_currently_displayed_timers = newValue;
		}

		lastKeyPressed = lcd_key;
		timeOfLastButtonAction = millis();
		break;
	}
	case btnLEFT:
	{
		uint32_t newValue = 0;
		uint32_t *pointer_to_currently_displayed_timers;

		if (currentTimerOnOFFState != 2)
		{
			pointer_to_currently_displayed_timers = &current_On_Time;
		}
		else
		{
			pointer_to_currently_displayed_timers = &on_Time;
		}

		newValue = *pointer_to_currently_displayed_timers - (1000 * timeChangeMultiplier);

		if (newValue > *pointer_to_currently_displayed_timers)
		{
			*pointer_to_currently_displayed_timers = 1000;
		}
		else
		{
			*pointer_to_currently_displayed_timers = newValue;
		}

		lastKeyPressed = lcd_key;
		timeOfLastButtonAction = millis();
		break;
	}
	case btnUP:
	{
		uint32_t newValue = 0;
		uint32_t *pointer_to_currently_displayed_timers;

		if (currentTimerOnOFFState != 2)
		{
			pointer_to_currently_displayed_timers = &current_Off_Time;
		}
		else
		{
			pointer_to_currently_displayed_timers = &off_Time;
		}

		newValue = *pointer_to_currently_displayed_timers + (1000 * timeChangeMultiplier);

		if (newValue < *pointer_to_currently_displayed_timers || newValue > 2419200000)
		{
			*pointer_to_currently_displayed_timers = 2419200000;
		}
		else
		{
			*pointer_to_currently_displayed_timers = newValue;
		}

		lastKeyPressed = lcd_key;
		timeOfLastButtonAction = millis();
		break;
	}
	case btnDOWN:
	{
		uint32_t newValue = 0;
		uint32_t *pointer_to_currently_displayed_timers;

		if (currentTimerOnOFFState != 2)
		{
			pointer_to_currently_displayed_timers = &current_Off_Time;
		}
		else
		{
			pointer_to_currently_displayed_timers = &off_Time;
		}

		newValue = *pointer_to_currently_displayed_timers - (1000 * timeChangeMultiplier);

		if (newValue > *pointer_to_currently_displayed_timers)
		{
			*pointer_to_currently_displayed_timers = 1000;
		}
		else
		{
			*pointer_to_currently_displayed_timers = newValue;
		}

		lastKeyPressed = lcd_key;
		timeOfLastButtonAction = millis();
		break;
	}
	case btnSELECT:
	{
		if (lastKeyPressed == btnSELECT)
		{
			break;
		}

		currentTimerOnOFFState--;

		if (currentTimerOnOFFState > 2)
		{
			currentTimerOnOFFState = 2;
		}

		lcd.setCursor(10, 0);
		lcd.print("      ");
		lcd.setCursor(10, 1);
		lcd.print("      ");

		lastKeyPressed = lcd_key;
		timeOfLastButtonAction = millis();
		break;
	}
	case btnNONE:
	{
		lastKeyPressed = btnNONE;
		break;
	}
	}
}