byte cursorFlag = 0;
byte incomingByte = 0;
byte cursorPosition = 0;
byte position_check = 0;

byte resistorSelect = 8; //rs select
byte enable = 9; //enable
byte d4 = 4; //d4
byte d5 = 5; //d5
byte d6 = 6; //d6
byte d7 = 7; //d7

byte pinArray[] = { resistorSelect, d7, d6, d5, d4 };

const long initialTextPosition = -30;
long textPosition = initialTextPosition;

const char signMessage2[] PROGMEM = { "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus sit amet shes bez deset felis id krokos." };

void setup()
{
	pinMode(resistorSelect, OUTPUT); // resistorSelect
	pinMode(enable, OUTPUT); // enable
	pinMode(d4, OUTPUT); // d4
	pinMode(d5, OUTPUT); // d5
	pinMode(d6, OUTPUT); //d6
	pinMode(d7, OUTPUT); //d7

	digitalWrite(resistorSelect, LOW);
	delay(100);  //initial wait for power-up

	incomingByte = B00110000; // initial reset (dali e neobhodim?)
	WriteHighBits();
	delay(1);

	WriteHighBits();
	delay(1);

	WriteHighBits();
	delay(1);
	//set to 4 bit operation, only the lower 4 bits matter, nibble command
	incomingByte = B00000010;      //set to 4 bit operation
	WriteLowBits();              //set to 4 bit operation
	delay(1);

	incomingByte = B00101000; // (bit 5) function set Sets the data width (DL) d4 =0 (bit 4) , number of lines (bit 3, d3) 1 line = 1 , and 5x7 font (bit 2)
	WriteBits();
	delay(1);

	incomingByte = B00001100; // command (bit 3),  Display ON/OFF (bit 2), Cursor ON/OFF (bit 1), Cursor Blinking (bit 0);
	WriteBits();
	delay(1);

	incomingByte = B00000110;  //Sets cursor move direction (bit 1), specifies to shift the display (bit 0). These operations are performed during data read/write
	WriteBits();
	delay(1);
	ClearScreen();
	delay(40);
}

void loop()
{
	PrintAStringToLCD();
}

void PrintAStringToLCD() 
{
	byte displayChars[32];

	for (byte i = 0; i < sizeof(displayChars); i++)
	{
		if (textPosition < sizeof(signMessage2) - 1 && textPosition >= 0)
		{
			char myChar = pgm_read_byte_near(signMessage2 + textPosition);
			displayChars[i] = myChar;
		}
		else
		{
			displayChars[i] = ' ';
		}

		textPosition++;
	}


	textPosition -= sizeof(displayChars);
	textPosition++;

	if (textPosition > sizeof(signMessage2))
	{
		textPosition = initialTextPosition;
	}

	delay(700);

	for (int i = 0; i < sizeof(displayChars); i++)
	{
		DisplayAChar(displayChars[i]);
		ManageCursor(-1000);
	}
}

// Clears screen if reached the end of second line, otherwise moves the cursor to the lower row
// accepts how many miliseconds to wait before clearing the screen, if set to negative value, does not clear the screen
void ManageCursor(int waitMilis)
{
	if ((cursorPosition > 15) && (cursorPosition < 33) && (cursorFlag == 0))  //end of the line, set cursor to next line
	{
		digitalWrite(resistorSelect, LOW);
		incomingByte = B11000000;
		cursorPosition++;
		cursorFlag = 1;
		WriteBits();
	}

	if ((cursorPosition >= 33) && (cursorFlag == 1))    // end of second line, set cursor to beginning
	{
		digitalWrite(resistorSelect, LOW);
		incomingByte = B10000000;
		WriteBits();
		cursorPosition = 0;
		cursorFlag = 0;

		delay(0);

		if (waitMilis > 0)
		{
			ClearScreen();
		}
	}
}

void ShiftDisplayLeft()
{
	incomingByte = B00011000;
	digitalWrite(resistorSelect, LOW);
	WriteBits();
}

void ShiftDisplayRight()
{
	incomingByte = B00011100;
	digitalWrite(resistorSelect, LOW);
	WriteBits();
}

void DisplayAChar(char character)     //displays a character on the LCD
{
	digitalWrite(resistorSelect, HIGH);
	delayMicroseconds(4);
	incomingByte = character;
	WriteBits();
	cursorPosition++;
	delayMicroseconds(4);
}

void ClearScreen()
{
	digitalWrite(resistorSelect, LOW);
	incomingByte = B00000001;
	WriteBits();
	delay(40);
}

void WriteBits()
{
	WriteHighBits();
	WriteLowBits();
}

void WriteLowBits()
{
	byte array_loop = 0;
	for (int bit_loop = 3; bit_loop > -1; bit_loop--) //loop for reading the lower 4 bits
	{
		digitalWrite(pinArray[array_loop + 1], bitRead(incomingByte, bit_loop));      //output the read 4 bits to pins d7 to d4
		array_loop++;
	}

	PulseEnablePin();
}

void PulseEnablePin()
{
	digitalWrite(enable, LOW);
	delayMicroseconds(4);
	digitalWrite(enable, HIGH);
	delayMicroseconds(4);
	digitalWrite(enable, LOW);
	delayMicroseconds(4);
}

void WriteHighBits()
{
	byte array_loop = 0;
	for (int bit_loop = 7; bit_loop > 3; bit_loop--)  //loop for reading the higher 4 bits
	{
		digitalWrite(pinArray[array_loop + 1], bitRead(incomingByte, bit_loop));      //output the read 4 bits to pins d7 to d4
		array_loop++;
	}

	PulseEnablePin();
}