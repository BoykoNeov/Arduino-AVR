// function to display a big number and free it afterwards
void PrintBignumToTFT (BigNumber n, uint8_t textSize)
{
  char * s = n.toString ();
  Serial.print("BigNum: ");
  printBignumToSerial(n);

  Serial.print("String from BigNum: ");
  String str = String(s);
  Serial.println(str);
  
  uint8_t sLength = strlen(str.c_str());
  Serial.print("String length: ");
  Serial.println(sLength);
  char notReversed[sLength + 1];
  char reversed[sLength + 1];

  str.toCharArray(notReversed, sLength + 1);
  
  for (uint8_t i = 0; i < sLength; i++)
  {
    reversed[sLength -1 -i] = notReversed[i];
  }

  reversed[sLength] = 0;

  String reversedString = String(reversed);
  Serial.print("reversed string: ");
  Serial.println(reversedString);
  Serial.println();

  for (uint8_t i = 0; i < sLength + 1; i++)
  {
    byte n = notReversed[i];
    Serial.print(n);
    Serial.print(" ");
  }
  
  Serial.println();

  for (uint8_t i = 0; i < sLength + 1; i++)
  {
    byte n = reversed[i];
    Serial.print(n);
    Serial.print(" ");
  }

  Serial.println();
  Serial.println(); 
  
  tft.drawString(10, 0, reversed, WHITE, textSize);
  delay(3000);
  free (s);
}  // end of printBignum

// function to display a big number and free it afterwards
void printBignumToSerial (BigNumber n)
{
  char * s = n.toString ();
  Serial.println (s);
  free (s);
}  // end of printBignum
