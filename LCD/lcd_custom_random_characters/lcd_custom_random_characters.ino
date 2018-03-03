byte cursor_flag = 0;
byte incomingByte = 0;  
byte cursor_position = 0;
byte position_check = 0;

byte rs_select = 8; //rs select
byte enable = 9; //enable
byte d4 = 4; //d4
byte d5 = 5; //d5
byte d6 = 6; //d6
byte d7 = 7; //d7
        
byte pinArray[] = {rs_select, d7, d6, d5, d4};

void setup() 
{
   Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps

   pinMode(rs_select, OUTPUT) ; // rs_select
   pinMode(enable, OUTPUT) ; // enable
   pinMode(d4, OUTPUT) ; // d4
   pinMode(d5, OUTPUT) ; // d5
   pinMode(d6, OUTPUT) ; //d6
   pinMode(d7, OUTPUT) ; //d7

  digitalWrite (rs_select, LOW);
  delay (400);  //initial wait for power-up

  incomingByte = B00110000; // initial reset (dali e neobhodim?)
  write_high_bits();
  delay (40);

  write_high_bits();
  delay (40);

  write_high_bits();
  delay (40);
                                  //set to 4 bit operation, only the lower 4 bits matter, nibble command
  incomingByte = B00000010 ;      //set to 4 bit operation
  write_low_bits() ;              //set to 4 bit operation
  delay (40);

  incomingByte = B00101000 ; // (bit 5) function set Sets the data width (DL) d4 =0 (bit 4) , number of lines (bit 3, d3) 1 line = 1 , and 5x7 font (bit 2)
  write_bits();
  delay (40);

  incomingByte = B00001100 ; // command bit 3,  Display ON (bit 2), Cursor On (bit 1), Cursor Blinking (bit 0);
  write_bits();
  delay (40);

  incomingByte = B00000110;  //Entry Mode, Increment cursor position, No display shift
  write_bits();
  delay (20);

  clear_custom_char();
  delay (10);

 //  set_custom_char();

  delay(10);
  clear_screen();
  
// write_custom_c();
//digitalWrite (rs_select, LOW);
//incomingByte = B11000000;
//write_bits();
//delay (40);
}


void clear_screen() 
{
  digitalWrite (rs_select, LOW);
  incomingByte = B00000001;
  write_bits();
  delay (40);
}  // setup end

void write_bits() 
{
  write_high_bits();
  write_low_bits();
}

void write_low_bits() 
{
  int array_loop=0 ;
  for (int bit_loop=3;  bit_loop >-1; bit_loop--) //loop for reading the lower 4 bits
  {                             
    digitalWrite (pinArray[array_loop+1], bitRead(incomingByte, bit_loop));      //output the read 4 bits to pins d7 to d4
    array_loop++ ;
  }
  
    digitalWrite (enable, LOW);
    delayMicroseconds(4);
    digitalWrite (enable, HIGH);                   // pulse enable
    delayMicroseconds(4);
    digitalWrite (enable, LOW);
    delayMicroseconds(4);
}


void write_high_bits() 
{
  int array_loop=0;
  for (int bit_loop=7; bit_loop >3; bit_loop--)  //loop for reading the higher 4 bits
  {                            
    digitalWrite (pinArray[array_loop+1], bitRead(incomingByte, bit_loop));      //output the read 4 bits to pins d7 to d4
    array_loop++ ;
  }
  
  digitalWrite (enable, LOW);
  delayMicroseconds(4);
  digitalWrite (enable, HIGH);                   // pulse enable
  delayMicroseconds(4);
  digitalWrite (enable, LOW);
  delayMicroseconds(4);
}
        
void write_custom_c()  
{
  digitalWrite (rs_select, HIGH);
  for (byte i=0; i<8; i++) 
  {
    incomingByte = i;
    write_bits();
  }
}

void loop() 
{        
// Serial.print ("cursor_position = ");
// Serial.println (cursor_position);

// serial_bit_control();
// write_millis();         //write millis on the screen
// serial_char_as_is();     //write serial characters as they are  
// serial_bit_control();
          
  display_random_char();
  delay (0);
  if ((cursor_position >= 33) && (cursor_flag==1)) 
  {
    randomize_custom_char(); 
  }
  
  if ((cursor_position > 15) && (cursor_position <33) && (cursor_flag==0))  //end of the line, set cursor to next line
  {             
    delay (0);
    digitalWrite(rs_select, LOW);
    incomingByte = B11000000;
    cursor_position++;
    cursor_flag = 1 ;
    write_bits();                   
  }
  
  if ((cursor_position >= 33) && (cursor_flag==1))    // end of second line, set cursor to beginning
  {                
    delay (0);
    digitalWrite(rs_select, LOW);
    incomingByte = B10000000;
    write_bits();
    cursor_position = 0 ;
    cursor_flag = 0 ;
    delay (0);
                 // clear_screen() ; 
   }          
}      


 //     void serialFlush(){
 // while(Serial.available() > 0) {
 //   char t = Serial.read();
 // }  }

 void display_random_char()     //displays random character on the LCD
{                    
    for (byte i=0; i<16; i++) 
    { 
      digitalWrite (rs_select , HIGH);
      delay (1);
      incomingByte = random(8);
      write_bits();
      cursor_position++;
      delay (1);
    }
    
    delay (0);
}

void serial_bit_control() 
{       
   if (Serial.available() > 0)    // send data only when you receive data
   {
     digitalWrite (rs_select, LOW);
     for (int read_loop=0; read_loop <5; read_loop++) 
     {
       incomingByte = (Serial.read());                   
       incomingByte = (incomingByte - 48);    // read 1 or 0
       digitalWrite (pinArray[read_loop], incomingByte);   //write low or high to the pins
       delay (10);                     
      }
      
      if (incomingByte > 49 ) //perform clear screen if receive other caracters other than 1 or 0 (actually a byte above 49)
      {                     
      Serial.print ("incomingByte : ");
      Serial.println (incomingByte);
      clear_screen();
      write_custom_c();
      }
      
      digitalWrite (enable, LOW);       //pulse
      delayMicroseconds(4);
      digitalWrite (enable, HIGH);
      delayMicroseconds(4);
      digitalWrite (enable, LOW);
      delayMicroseconds(4);
                
      Serial.print("rs_select = ");               //feedback on what's happening
      Serial.println(digitalRead(rs_select));
                
      Serial.print("d4 = ");
      Serial.println (digitalRead(d4));
               
      Serial.print ("d5 = ");
      Serial.println (digitalRead(d5));
                
      Serial.print ("d6 = ");
      Serial.println (digitalRead(d6));
                
      Serial.print ("d7 = ");
      Serial.println (digitalRead(d7));
      //     position_checker_for_serial_bit();
      }
}
                

// void position_checker_for_serial_bit()
// {                 if (digitalRead(rs_select) == HIGH) {
//                 position_check ++; }
//            
//                 if (position_check == 2) {             // when rs_select is twice high, this means that a character was displayed and 1 is added to position
//                 cursor_position ++ ;
//                 position_check = 0; }
// }

void serial_char_as_is() //function definition of writing serial bytes as they are
{                        
   if (Serial.available() > 0) 
   {
    digitalWrite (rs_select, HIGH);
    incomingByte = (Serial.read());
    write_bits();
    cursor_position++;
    }
}

void write_millis() 
{
   digitalWrite (rs_select, HIGH);
   String m_string =  String(millis(), DEC);
   for (int i=0; i < (m_string.length()); i++) 
   {
      incomingByte = m_string[i];
      write_bits();
      delay(1);
   }

   delay (540);
   digitalWrite(rs_select, LOW);
   incomingByte = B10000000;
   write_bits();
//  clear_screen();      
}

void set_custom_char() 
{
   for (byte i=0 ; i<8; i++)
   {
   incomingByte = (B01000000 | (i << 3)) ;
   incomingByte = (incomingByte | i);
   digitalWrite (rs_select, LOW);
   write_bits();
   digitalWrite (rs_select, HIGH);
   incomingByte = i;
   write_bits();
   }
}

void clear_custom_char() 
{
    for (byte i=0 ; i<8; i++) 
    {
    incomingByte = (B01000000 | (i << 3)) ;
    digitalWrite (rs_select, LOW);
    write_bits();
    digitalWrite (rs_select, HIGH);
    incomingByte = B00000000;
      for (byte ix=0; ix<8; ix++) 
      {
        write_bits();
      }
    }
}

void randomize_custom_char() 
{
    for (byte i=0 ; i<8; i++) 
    {
    incomingByte = (B01000000 | (i << 3)) ;
    digitalWrite (rs_select, LOW);
    write_bits();
    digitalWrite (rs_select, HIGH);
       for (byte ix=0; ix<8; ix++) 
       {
         incomingByte = random(256);
         write_bits();
         delay(1);
       }
    }
}
