#include <Wire.h>    
 
#define disk1 0x50    //Address of 24LC256 eeprom chip
 
void setup(void)
{
  Serial.begin(9600);
  Wire.begin();    
}
 
void loop()
{
   for(unsigned int i = 0; i < 8192; i++)
   {
       Serial.print("address: ");
       Serial.print(i);
       Serial.print(" ");
       Serial.print("value: ");
       Serial.println(readEEPROM(disk1, i), DEC);
   }

   delay(3600000);
}
 
byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
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

