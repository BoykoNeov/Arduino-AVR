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

uint32_t on_Time = 0;
uint32_t off_Time = 0;
uint8_t lastKeyPressed = btnNONE;
uint32_t timeOfLastChange = 0;

void setup()
{
	delay(30);
	lcd.begin(16, 2);
	delay(30);
	lcd.setCursor(0, 0);
	Serial.begin(9600);
}

void loop()
{
	lcd.setCursor(0, 0);

	uint32_t on_Time_in_Seconds = on_Time / 1000;
	uint8_t on_Time_Hours = on_Time_in_Seconds / 3600;
	uint8_t remainingMinutes = (on_Time_in_Seconds % 3600) / 60;
	uint8_t remainingSeconds = (on_Time_in_Seconds % 3600) % 60;

	PrintNumericValue(on_Time_Hours);
	lcd.print(":");
	PrintNumericValue(remainingMinutes);
	lcd.print(":");
	PrintNumericValue(remainingSeconds);

	lcd.setCursor(0, 1);

	lcd_key = read_LCD_buttons();

	uint8_t multiplier = 1;

	if ((lastKeyPressed != lcd_key) && (millis() - timeOfLastChange > 2000))
	{
		multiplier = 10;
		lastKeyPressed = btnNONE;
		Serial.println("multiply");
	}

	switch (lcd_key)
	{
		if (lastKeyPressed == lcd_key) {
			break;
		}

	case btnRIGHT:
	{
		on_Time += 2000 * multiplier;
		lcd.print("right");
		lastKeyPressed = lcd_key;
		timeOfLastChange = millis();
		break;
	}
	case btnLEFT:
	{
		on_Time -= 2000 * multiplier;
		lcd.print("left");
		lastKeyPressed = lcd_key;
		timeOfLastChange = millis();
		break;
	}
	case btnUP:
	{
		lcd.print("UP    ");
		lastKeyPressed = lcd_key;
		timeOfLastChange = millis();
		break;
	}
	case btnDOWN:
	{
		lcd.print("DOWN  ");
		lastKeyPressed = lcd_key;
		timeOfLastChange = millis();
		break;
	}
	case btnSELECT:
	{
		lcd.print("SELECT");
		lastKeyPressed = lcd_key;
		timeOfLastChange = millis();
		break;
	}
	case btnNONE:
	{
		lcd.print("NONE  ");
		break;
	}
	}
}
