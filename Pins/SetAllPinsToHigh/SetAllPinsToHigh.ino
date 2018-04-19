void setup() {
  for(byte i = 0; i < 14; i++)
  {
    pinMode(i,OUTPUT);
  }

  for(byte i = 0; i < 14; i++)
  {
    digitalWrite(i,HIGH);
  }

pinMode(A0, OUTPUT);
digitalWrite(A0, HIGH);

pinMode(A1, OUTPUT);
digitalWrite(A1, HIGH);

pinMode(A2, OUTPUT);
digitalWrite(A2, HIGH);

pinMode(A3, OUTPUT);
digitalWrite(A3, HIGH);

pinMode(A4, OUTPUT);
digitalWrite(A4, HIGH);

pinMode(A5, OUTPUT);
digitalWrite(A5, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:

}
