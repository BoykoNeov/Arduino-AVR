/****************************************************************************
    LCD_AVR_4d.ino  - Use an HD44780U based LCD with an Arduino
 
    Copyright (C) 2013 Donald Weiman    (weimandn@alfredstate.edu)
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/****************************************************************************
         File:    LCD_AVR_4d.ino
         Date:    September 16, 2013
 
       Target:    Arduino (ATmega328)
     Compiler:    Arduino IDE (v1.0.5)
       Author:    Donald Weiman
       
      Summary:    4-bit data interface, busy flag not implemented.
                  Any LCD pin can be connected to any available I/O port.
                  Includes a simple write string routine.
*/
/******************************* Program Notes ******************************
 
            This program uses an 4-bit data interface but does not use the
              busy flag to determine when the LCD controller is ready.  The
              LCD RW line (pin 5) is not connected to the uP and it must be
              connected to GND for the program to function.
 
            All time delays are longer than those specified in most datasheets
              in order to accommodate slower than normal LCD modules.  This
              requirement is well documented but almost always ignored.  The
              information is in a note at the bottom of the right hand
              (Execution Time) column of the instruction set.
 
  ***************************************************************************
 
            The four data lines as well as the two control lines may be
              implemented on any available I/O pin of any port.  These are
              the connections used for this program:
 
            The numbers shown next to the Arduino are the Arduino pin 
            numbers, not the IC pin numbers. 
 
                 -----------                   ----------
                |  Arduino  |                 |   LCD    |
                |           |                 |          |
                |        PD7|7  ------------->|D7        |
                |        PD6|6  ------------->|D6        |
                |        PD5|5  ------------->|D5        |
                |        PD4|4  ------------->|D4        |
                |           |                 |D3        |
                |           |                 |D2        |
                |           |                 |D1        |
                |           |                 |D0        |
                |           |                 |          |
                |        PB1|9  ------------->|E         |
                |           |         GND --->|RW        |
                |        PB0|8  ------------->|RS        |
                 -----------                   ----------
 
  **************************************************************************/

// LCD interface (should agree with the diagram above)
//   make sure that the LCD RW pin is connected to GND
uint8_t  lcd_D7_ArdPin   =  7;                  // lcd D7 connection
uint8_t  lcd_D6_ArdPin   =  6;
uint8_t  lcd_D5_ArdPin   =  5;
uint8_t  lcd_D4_ArdPin   =  4;

uint8_t  lcd_E_ArdPin    =  9;                  // lcd Enable pin
uint8_t  lcd_RS_ArdPin   =  8;                  // lcd Register Select pin

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

// Program ID
uint8_t program_author[]   = "Donald Weiman";
uint8_t program_version[]  = "LCD_AVR_4d (ino)";
uint8_t program_date[]     = "Sep 16, 2013";


/******************************* Main Program Code *************************/

void setup (void)
{
// configure the microprocessor pins for the data lines
    pinMode(lcd_D7_ArdPin, OUTPUT);                 // 8 data lines - output
    pinMode(lcd_D6_ArdPin, OUTPUT);
    pinMode(lcd_D5_ArdPin, OUTPUT);
    pinMode(lcd_D4_ArdPin, OUTPUT);

// configure the microprocessor pins for the control lines
    pinMode(lcd_E_ArdPin, OUTPUT);                  // E line - output
    pinMode(lcd_RS_ArdPin, OUTPUT);                 // RS line - output

// initialize the LCD controller as determined by the defines (LCD instructions)
    lcd_init_4d();                                  // initialize the LCD display for an 8-bit interface
      
// display the first line of information
    lcd_write_string_4d(program_author);

// set cursor to start of second line
    lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
    delayMicroseconds(80);                          // 40 uS delay (min)
 
// display the second line of information
    lcd_write_string_4d(program_version);
}

// endless loop
void loop()
  {  
  }
/******************************* End of Main Program Code ******************/

/*============================== 4-bit LCD Functions ======================*/
/*
  Name:     lcd_init_4d
  Purpose:  initialize the LCD module for a 8-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_init_4d(void)
{
// Power-up delay
    delay(100);                                     // initial 40 mSec delay

 // IMPORTANT - At this point the LCD module is in the 8-bit mode and it is expecting to receive  
//   8 bits of data, one bit on each of its 8 data lines, each time the 'E' line is pulsed.
//
// Since the LCD module is wired for the 4-bit mode, only the upper four data lines are connected to 
//   the microprocessor and the lower four data lines are typically left open.  Therefore, when 
//   the 'E' line is pulsed, the LCD controller will read whatever data has been set up on the upper 
//   four data lines and the lower four data lines will be high (due to internal pull-up circuitry).
//
// Fortunately the 'FunctionReset' instruction does not care about what is on the lower four bits so  
//   this instruction can be sent on just the four available data lines and it will be interpreted 
//   properly by the LCD controller.  The 'lcd_write_4' subroutine will accomplish this if the 
//   control lines have previously been configured properly.

// Set up the RS and E lines for the 'lcd_write_4' subroutine.
    digitalWrite(lcd_RS_ArdPin, LOW);               // select the Instruction Register (RS low)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    
// Reset the LCD controller
    lcd_write_4(lcd_FunctionReset);                 // first part of reset sequence
    delay(10);                                      // 4.1 mS delay (min)

    lcd_write_4(lcd_FunctionReset);                 // second part of reset sequence
    delayMicroseconds(200);                         // 100uS delay (min)

    lcd_write_4(lcd_FunctionReset);                 // third part of reset sequence
    delayMicroseconds(200);                         // this delay is omitted in the data sheet

// Preliminary Function Set instruction - used only to set the 4-bit mode.
// The number of lines or the font cannot be set at this time since the controller is still in the
//  8-bit mode, but the data transfer mode can be changed since this parameter is determined by one 
//  of the upper four bits of the instruction.    
    
    lcd_write_4(lcd_FunctionSet4bit);               // set 4-bit mode
    delayMicroseconds(80);                          // 40 uS delay (min)   
    
// Function Set instruction
    lcd_write_instruction_4d(lcd_FunctionSet4bit);  // set mode, lines, and font
    delayMicroseconds(80);                          // 40uS delay (min)

// The next three instructions are specified in the data sheet as part of the initialization routine, 
//  so it is a good idea (but probably not necessary) to do them just as specified and then redo them 
//  later if the application requires a different configuration.

// Display On/Off Control instruction
    lcd_write_instruction_4d(lcd_DisplayOff);       // turn display OFF
    delayMicroseconds(80);                          // 40 uS delay (min)

// Clear Display instruction
    lcd_write_instruction_4d(lcd_Clear);            // clear display RAM
    delay(4);                                       // 1.64 mS delay (min)

// ; Entry Mode Set instruction
    lcd_write_instruction_4d(lcd_EntryMode);        // set desired shift characteristics
    delayMicroseconds(80);                          // 40 uS delay (min)
   
// This is the end of the LCD controller initialization as specified in the data sheet, but the display
//  has been left in the OFF condition.  This is a good time to turn the display back ON.

// Display On/Off Control instruction
    lcd_write_instruction_4d(lcd_DisplayOn);        // turn the display ON
    delayMicroseconds(80);                          // 40 uS delay (min)
}

/*...........................................................................
  Name:     lcd_write_string_4d
; Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_write_string_4d(uint8_t theString[])
{
    volatile int i = 0;                             // character counter*/
    while (theString[i] != 0)
    {
        lcd_write_character_4d(theString[i]);
        i++;
        delayMicroseconds(80);                          // 40 uS delay (min)
    }
}

/*...........................................................................
  Name:     lcd_write_character_4d
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_write_character_4d(uint8_t  theData)
{
    digitalWrite(lcd_RS_ArdPin, HIGH);              // select the Data Register (RS high)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    lcd_write_4(theData);                           // write the upper 4-bits of the data
    lcd_write_4(theData << 4);                      // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_instruction_4d
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_write_instruction_4d(uint8_t  theInstruction)
{
    digitalWrite(lcd_RS_ArdPin, LOW);               // select the Instruction Register (RS low)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    lcd_write_4(theInstruction);                    // write the upper 4-bits of the data
    lcd_write_4(theInstruction << 4);               // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_4
  Purpose:  send a nibble of information to the LCD module
  Entry:    (theByte) contains the information to be sent to the desired LCD register
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    the desired information (4-bits) must be in the upper half of (theByte)
            use either time delays or the busy flag
*/
void lcd_write_4(uint8_t  theByte)
{
    digitalWrite(lcd_D7_ArdPin,LOW);                       // assume that data is '0'
    if (theByte & 1<<7) digitalWrite(lcd_D7_ArdPin,HIGH);  // make data = '1' if necessary
          
    digitalWrite(lcd_D6_ArdPin,LOW);                       // repeat for each data bit
    if (theByte & 1<<6) digitalWrite(lcd_D6_ArdPin,HIGH);    

    digitalWrite(lcd_D5_ArdPin,LOW);
    if (theByte & 1<<5) digitalWrite(lcd_D5_ArdPin,HIGH);

    digitalWrite(lcd_D4_ArdPin,LOW);
    if (theByte & 1<<4) digitalWrite(lcd_D4_ArdPin,HIGH);
    
// write the data
                                                    // 'Address set-up time' (40 nS)
    digitalWrite(lcd_E_ArdPin,HIGH);                // Enable pin high
    delayMicroseconds(1);                           // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    digitalWrite(lcd_E_ArdPin,LOW);                 // Enable pin low
    delayMicroseconds(1);                           // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}
