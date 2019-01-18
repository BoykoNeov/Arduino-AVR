int read_LCD_buttons()
{
	adc_key_in = analogRead(0);
	if (adc_key_in > 1000) return btnNONE;
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 250)  return btnUP;
	if (adc_key_in < 450)  return btnDOWN;
	if (adc_key_in < 650)  return btnLEFT;
	if (adc_key_in < 850)  return btnSELECT;

	return btnNONE;
}

void PrintNumericValue(uint16_t value)
{
	if (value >= 0)
	{
		if (value < 10)
		{
			lcd.print('0');
		}

		lcd.print(value);
	}
	else
	{
		return;
	}
}