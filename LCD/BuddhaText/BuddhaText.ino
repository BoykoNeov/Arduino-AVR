// Displays random famoous quotes on the LCD
// This has the side intent of testing flash size limit in Arduino in practice

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

const long initialTextPosition = -31;
long textPosition = initialTextPosition;
byte lastRandomQuote = 255;
char buffer[800];

const char message1[] PROGMEM = { "Peace comes from within. Do not seek it without.   Buddha"};
const char message2[] PROGMEM = { "We are what we think. All that we are arises with our thoughts. With our thoughts, we make the world.   Buddha" };
const char message3[] PROGMEM = { "What we think, we become.    Buddha" };
const char message4[] PROGMEM = { "Thousands of candles can be lighted from a single candle, and the life of the candle will not be shortened. Happiness never decreases by being shared.   Buddha" };
const char message5[] PROGMEM = {"Holding on to anger is like grasping a hot coal with the intent of throwing it at someone else; you are the one who gets burned.   Buddha"};
const char message6[] PROGMEM = {"Health is the greatest gift, contentment the greatest wealth, faithfulness the best relationship.   Buddha"};
const char message7[] PROGMEM = {"You will not be punished for your anger, you will be punished by your anger.   Buddha"};
const char message8[] PROGMEM = {"You can search throughout the entire universe for someone who is more deserving of your love and affection than you are yourself, and that person is not to be found anywhere. You yourself, as much as anybody in the entire universe deserve your love and affection.   Buddha"};
const char message9[] PROGMEM = {"Just as treasures are uncovered from the earth, so virtue appears from good deeds, and wisdom appears from a pure and peaceful mind. To walk safely through the maze of human life, one needs the light of wisdom and the guidance of virtue.   Buddha"};
const char message10[] PROGMEM = {"It is a man's own mind, not his enemy or foe, that lures him to evil ways   Buddha"};
const char message11[] PROGMEM = {"Even death is not to be feared by one who has lived wisely.   Buddha"};
const char message12[] PROGMEM = {"Unity can only be manifested by the binary. Unity itself and the idea of Unity are already two.   Buddha"};
const char message13[] PROGMEM = {"I never see what has been done; I only see what remains to be done.   Buddha"};
const char message14[] PROGMEM = {"To live a pure unselfish life, one must count nothing as one's own in the midst of abundance.   Buddha"};
const char message15[] PROGMEM = {"The foolish man conceives the idea of 'self.' The wise man sees there is no ground on which to build the idea of 'self;' thus, he has a right conception of the world and well concludes that all compounds amassed by sorrow will be dissolved again, but the truth will remain.   Buddha"};
const char message16[] PROGMEM = {"He who gives away shall have real gain. He who subdues himself shall be free; he shall cease to be a slave of passions. The righteous man casts off evil, and by rooting out lust, bitterness, and illusion do we reach Nirvana.   Buddha"};
const char message17[] PROGMEM = {"Of those beings who live in ignorance, shut up and confined, as it were, in an egg, I have first broken the eggshell of ignorance and alone in the universe obtained the most exalted, universal Buddhahood.   Buddha"};
const char message18[] PROGMEM = {"I do not believe in a fate that falls on men however they act; but I do believe in a fate that falls on them unless they act.   Buddha"};
const char message19[] PROGMEM = {"Charity bestowed upon those who are worthy of it is like good seed sown on a good soil that yields an abundance of fruits. But alms given to those who are yet under the tyrannical yoke of the passions are like seed deposited in a bad soil. The passions of the receiver of the alms choke, as it were, the growth of merits.   Buddha"};
const char message20[] PROGMEM = {"Let my skin and sinews and bones dry up, together with all the flesh and blood of my body! I welcome it! But I will not move from this spot until I have attained the supreme and final wisdom.   Buddha"};
const char message21[] PROGMEM = {"With fools, there is no companionship. Rather than to live with men who are selfish, vain, quarrelsome, and obstinate, let a man walk alone.   Buddha"};
const char message22[] PROGMEM = {"Whatever words we utter should be chosen with care for people will hear them and be influenced by them for good or ill.   Buddha"};
const char message23[] PROGMEM = {"The memory of everything is very soon overwhelmed in time.     Marcus Aurelius"};
const char message24[] PROGMEM = {"You have power over your mind - not outside events. Realize this, and you will find strength.    Marcus Aurelius"};
const char message25[] PROGMEM = {"Dwell on the beauty of life. Watch the stars, and see yourself running with them.    Marcus Aurelius"};
const char message26[] PROGMEM = {"The happiness of your life depends upon the quality of your thoughts.    Marcus Aurelius"};
const char message27[] PROGMEM = {"Everything we hear is an opinion, not a fact. Everything we see is a perspective, not the truth    Marcus Aurelius"};
const char message28[] PROGMEM = {"Waste no more time arguing about what a good man should be. Be one.    Marcus Aurelius"};
const char message29[] PROGMEM = {"When you arise in the morning think of what a privilege it is to be alive, to think, to enjoy, to love ...    Marcus Aurelius"};
const char message30[] PROGMEM = {"If you are distressed by anything external, the pain is not due to the thing itself, but to your estimate of it; and this you have the power to revoke at any moment.    Marcus Aurelius"};
const char message31[] PROGMEM = {"Accept the things to which fate binds you, and love the people with whom fate brings you together,but do so with all your heart.    Marcus Aurelius"};
const char message32[] PROGMEM = {"The best revenge is to be unlike him who performed the injury.    Marcus Aurelius"};
const char message33[] PROGMEM = {"If someone is able to show me that what I think or do is not right, I will happily change, for I seek the truth, by which no one was ever truly harmed. It is the person who continues in his self-deception and ignorance who is harmed.    Marcus Aurelius"};
const char message34[] PROGMEM = {"Never let the future disturb you. You will meet it, if you have to, with the same weapons of reason which today arm you against the present.    Marcus Aurelius"};
const char message35[] PROGMEM = {"If it is not right do not do it; if it is not true do not say it.    Marcus Aurelius"};
const char message36[] PROGMEM = {"The object of life is not to be on the side of the majority, but to escape finding oneself in the ranks of the insane.    Marcus Aurelius"};
const char message37[] PROGMEM = {"I have often wondered how it is that every man loves himself more than all the rest of men, but yet sets less value on his own opinion of himself than on the opinion of others.    Marcus Aurelius"};
const char message38[] PROGMEM = {"Whenever you are about to find fault with someone, ask yourself the following question: What fault of mine most nearly resembles the one I am about to criticize?    Marcus Aurelius"};
const char message39[] PROGMEM = {"When another blames you or hates you, or people voice similar criticisms, go to their souls, penetrate inside and see what sort of people they are. You will realize that there is no need to be racked with anxiety that they should hold any particular opinion about you.    Marcus Aurelius"};
const char message40[] PROGMEM = {"Do not act as if you were going to live ten thousand years. Death hangs over you. While you live, while it is in your power, be good.    Marcus Aurelius"};
const char message41[] PROGMEM = {"When you wake up in the morning, tell yourself: the people I deal with today will be meddling, ungrateful, arrogant, dishonest, jealous and surly. They are like this because they can't tell good from evil. But I have seen the beauty of good, and the ugliness of evil, and have recognized that the wrongdoer has a nature related to my own - not of the same blood and birth, but the same mind, and possessing a share of the divine. And so none of them can hurt me. No one can implicate me in ugliness. Nor can I feel angry at my relative, or hate him. We were born to work together like feet, hands and eyes, like the two rows of teeth, upper and lower. To obstruct each other is unnatural. To feel anger at someone, to turn your back on him: these are unnatural.    Marcus Aurelius"};
const char message42[] PROGMEM = {"Here is a rule to remember in future, when anything tempts you to feel bitter: not 'This is misfortune,' but 'To bear this worthily is good fortune.'    Marcus Aurelius"};
const char message43[] PROGMEM = {"You are a little soul carrying about a corpse, as Epictetus used to say.    Marcus Aurelius"};
const char message44[] PROGMEM = {"Do not indulge in dreams of having what you have not, but reckon up the chief of the blessings you do possess, and then thankfully remember how you would crave for them if they were not yours.    Marcus Aurelius"};
const char message45[] PROGMEM = {"What we do now echoes in eternity.    Marcus Aurelius"};
const char message46[] PROGMEM = {"Think of yourself as dead. You have lived your life. Now, take what's left and live it properly. What doesn't transmit light creates its own darkness.    Marcus Aurelius"};
const char message47[] PROGMEM = {"Live a good life. If there are gods and they are just, then they will not care how devout you have been, but will welcome you based on the virtues you have lived by. If there are gods, but unjust, then you should not want to worship them. If there are no gods, then you will be gone, but will have lived a noble life that will live on in the memories of your loved ones.    Marcus Aurelius"};
const char message48[] PROGMEM = {"There are things in life you cannot choose: how you feel. Max Payne"};
const char message49[] PROGMEM = {"The past is a puzzle, like a broken mirror. As you piece it together, you cut yourself, your image keeps shifting. And you change with it. It could destroy you, drive you mad. It could set you free. Max Payne"};
const char message50[] PROGMEM = {"There are no choices. Nothing but a straight line. The illusion comes afterwards, when you ask 'why me?' and 'what if?'. When you look back and see the branches, like a pruned bonsai tree, or forked lightning. If you had done something differently, it wouldn't be you, it would be someone else looking back, asking a different set of questions.   Max Payne"};
const char message51[] PROGMEM = {"I must not fear. Fear is the mind-killer. Fear is the little-death that brings total obliteration. I will face my fear. I will permit it to pass over me and through me. And when it has gone past I will turn the inner eye to see its path. Where the fear has gone there will be nothing. Only I will remain.   Bene Gesserit Litany Against Fear."};
const char message52[] PROGMEM = {"What is great in man is that he is a bridge and not a goal.   Friedrich Nietzsche"};
const char message53[] PROGMEM = {"No shepherd, and one herd! Everyone wants the same, everyone is the same: whoever feels different goes wilingly into the madhouse.   Friedrich Nietzsche"};
const char message54[] PROGMEM = {"How can I tell that the past isn't a fiction designed to account for the discrepancy between my immediate physical sensations and my state of mind?   Douglas Adams"};
const char message55[] PROGMEM = {"Remember that time slurs over everything, let all deeds fade, blurs all writings and kills all memories. Exempt are only those which dig into the hearts of men by love.   Aristotle"};
const char message56[] PROGMEM = {"Eternity is in love with the productions of time.   William Blake"};
const char message57[] PROGMEM = {"Being with you and not being with you is the only way I have to measure time.   Jorge Luis Borges"};
const char message58[] PROGMEM = {"Time forks perpetually toward innumerable futures.   Jorge Luis Borges"};
const char message59[] PROGMEM = {"When Youth and Pleasure meet To chase the glowing Hours with flying feet.   Lord Byron"};
const char message60[] PROGMEM = {"Swerve me? The path to my fixed purpose is laid with iron rails, whereon my soul is grooved to run. Over unsounded gorges, through the rifled hearts of mountains, under torrents' beds, unerringly I rush! Naught's an obstacle, naught's an angle to the iron way!  'Moby Dick' Herman Melville"};
const char message61[] PROGMEM = {"I do not live for what the world thinks of me, but for what I think of myself.    Jack London"};
const char message62[] PROGMEM = {"Life is not always a matter of holding good cards, but sometimes, playing a poor hand well.   Jack London"};
const char message63[] PROGMEM = {"Sunshine cannot bleach the snow, Nor time unmake what poets know.   Ralph Waldo Emerson"};

const char * const string_table[] PROGMEM =
{
  message1,
  message2,
  message3,
  message4,
  message5,
  message6,
  message7,
  message8,
  message9,
  message10,
  message11,
  message12,
  message13,
  message14,
  message15,
  message16,
  message17,
  message18,
  message19,
  message20,
  message21,
  message22,
  message23,
  message24,
  message25,
  message26,
  message27,
  message28,
  message29,
  message30,
  message31,
  message32,
  message33,
  message34,
  message35,
  message36,
  message37,
  message38,
  message39,
  message40,
  message41,
  message42,
  message43,
  message44,
  message45,
  message46,
  message47,
  message48,
  message49,
  message50,
  message51,
  message52,
  message53,
  message54,
  message55,
  message56,
  message57,
  message58,
  message59,
  message60,
  message61,
  message62,
  message63
};

void setup()
{
  // Setup pins for the LCD
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

  // Start serial communication, useful for debugging
  Serial.begin(9600);

  // analog pin 0 is connected to the buttons of the LCD shield, this is not nessessary just for the LCD operation
  pinMode(A0, INPUT);  

  // use the analog read from pin A1 as a random input
  pinMode(A1, INPUT);

  //Use the read from an unconnected analog pin as random
  randomSeed(analogRead(A1));
}

void loop()
{
	byte quoteToDisplay = lastRandomQuote;

	while (quoteToDisplay == lastRandomQuote) 
	{
		// sizeof(string_table) as upper limit produces incorrect results (as long as my understanding of what it should represent are concerned);
		quoteToDisplay = random(0, 63);
	}

	lastRandomQuote = quoteToDisplay;

	strcpy_P(buffer, (char*)pgm_read_word(&(string_table[quoteToDisplay]))); // Necessary casts and dereferencing 
	Serial.println(quoteToDisplay);
	Serial.println(buffer); //debug check

	unsigned int endOfString = strlen(buffer);
	PrintAStringToLCD(buffer, endOfString);
	delay(2000);
}

void PrintAStringToLCD(char buffer[], unsigned int stringSize)
{
	while (true)
	{
		byte displayChars[32];

		for (byte i = 0; i < sizeof(displayChars); i++)
		{
			if (textPosition < stringSize && textPosition >= 0)
			{
				char myChar = buffer[textPosition];
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

		if (textPosition > stringSize + 1)
		{
			textPosition = initialTextPosition;
			return;
		}

		delay(500);

		for (int i = 0; i < sizeof(displayChars); i++)
		{
			DisplayAChar(displayChars[i]);
			ManageCursor(-1000);
		}
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
