// With the modified wire.h (with increased buffer to 66) writing whole pages now works
#include "Wire.h"   
 
#define disk1 0x50    //Address of 24C64 eeprom chip
 
void setup(void)
{
  Wire.begin();
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  unsigned int eePageAddress = 0;
  const int arraySize = 32;

// this works, using modified library
  byte data[arraySize];
  for(byte i = 0; i < arraySize; i++)
  {
    data[i] = i;
  }

  unsigned long resultInMillis = millis();

  for (int page = 0; page < 256; page++)
  {
    writeEEPROMpage(disk1, eePageAddress, data, arraySize);
    eePageAddress += 32;
    delay(5);
  }

  resultInMillis = millis() - resultInMillis;
  Serial.println(resultInMillis);   
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
