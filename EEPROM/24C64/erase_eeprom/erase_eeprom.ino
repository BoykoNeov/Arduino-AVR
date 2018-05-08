#include <Wire.h>    
 
#define disk1 0x50    //Address of 24LC256 eeprom chip
 
void setup(void)
{
  Wire.begin();  

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  unsigned int address = 0;

  for (; address < 32768; address++)
  {
     writeEEPROM(disk1, address, 255); 
  }
}
 
void loop()
  {
    //blink when ready
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(500);                      
  digitalWrite(LED_BUILTIN, LOW);   
  delay(500);  
  }
 
void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
 
  delay(5);
}
