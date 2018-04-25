// results:
// Millis readinig 64 bytes chunks: 3473
// Millis readinig byte by byte: 18067

#include "Wire.h"    
 
#define disk1 0x50    //Address of 24LC256 eeprom chip
 
void setup(void)
{
  Serial.begin(9600);
  Wire.begin();    
}
 
void loop()
{
  const int byteSizeLimit = 64;
  byte readbytes[byteSizeLimit];
  unsigned int addressToStartReading = 0;

  unsigned long startMillis = millis();
  for (int page = 0; page < 512; page++)
  {
    readEEPROM(disk1, page, readbytes, byteSizeLimit);
    addressToStartReading += 64;
  }

   startMillis = millis() - startMillis;
   Serial.print("Millis readinig 64 bytes chunks: ");
   Serial.println(startMillis);

  startMillis = millis();
  for(unsigned int i = 0; i < 32768; i++)
  {
    byte test = readEEPROMbyte(disk1, i);
  }

   startMillis = millis() - startMillis;
   Serial.print("Millis readinig byte by byte: ");
   Serial.println(startMillis); 
  
   delay(3600000);
}
 
void readEEPROM(int deviceaddress, unsigned int eeaddress, byte bufer[], byte bufferLength ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress, bufferLength);

  for (byte j = 0; j < bufferLength; j++)
  {
    if(Wire.available())
    {
       bufer[j] = Wire.read();
    }
  }
}

byte readEEPROMbyte(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}
