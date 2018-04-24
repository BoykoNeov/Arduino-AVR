// Write 0s , because it this state, all bits are charged - test for EEPROM holding charge/data
// Usually, when it fails, 0s change to anything else
#include "Wire.h"   
 
#define disk1 0x50    //Address of 24LC256 eeprom chip
 
void setup(void)
{
  Wire.begin();  

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  unsigned int eePageAddress = 40;
  const int arraySize = 64;

// this works, using modified library
  byte data[arraySize];
  for(byte i = 0; i < arraySize; i++)
  {
    data[i] = i + 10;
  }

  writeEEPROMpage(disk1, eePageAddress, data, arraySize); 
}
 
void loop()
  {
    //blink when ready
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(500);                      
  digitalWrite(LED_BUILTIN, LOW);   
  delay(500);  
  }

void writeEEPROMpage(int deviceAddress, unsigned int eePageAddress, byte data[], byte dataLength)
{
  Wire.beginTransmission(deviceAddress);
  Wire.write((int)(eePageAddress >> 8));   // MSB
  Wire.write((int)(eePageAddress & 0xFF)); // LSB
  
  for(byte i = 0; i < dataLength; i++)
  {
    Wire.write(data[i]);
  }

  Wire.endTransmission();
}
 
//void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
//{
//  Wire.beginTransmission(deviceaddress);
//  Wire.write((int)(eeaddress >> 8));   // MSB
//  Wire.write((int)(eeaddress & 0xFF)); // LSB
//  Wire.write(data);
//  Wire.endTransmission();
// 
//  delay(5);
//}
