#include <BigNumber.h>
#include <avr/pgmspace.h>
const char E_String[] PROGMEM =  { "2.7182818284590452353602874713526624977572470936999595749669676277240766303535"};
const char Pi_String[] PROGMEM = { "3.1415926535897932384626433832795028841971693993751058209749445923078164062862" };

void setup() 
{
  
Serial.begin(9600);

BigNumber::begin ();  // initialize library
BigNumber::setScale (78);

char bufer[78];
strcpy_P(bufer, E_String);
BigNumber e = BigNumber(bufer);

strcpy_P(bufer, Pi_String);
BigNumber pi = BigNumber(bufer);

BigNumber test = "2.124";
Serial.println(sineFromDegrees (BigNumber(test), 76));
test *= pi / BigNumber(180);
Serial.println(sineFromRadians (test, 76));

test = "2.124";
Serial.println(cosineFromDegrees (BigNumber(test), 76));
test *= pi / BigNumber(180);
Serial.println(cosineFromRadians (test, 76));
}

void loop() {
  

}

void printDouble( double val){
 // prints val with number of decimal places determine by precision
 // precision is a number from 0 to 6 indicating the desired decimial places
 // example: lcdPrintDouble( 3.1415, 2); // prints 3.14 (two decimal places)
 byte precision = 6;

 if(val < 0.0){
   Serial.print('-');
   val = -val;
 }

 Serial.print (int(val));  //prints the int part
 if( precision > 0) {
   Serial.print("."); // print the decimal point
   unsigned long frac;
   unsigned long mult = 1;
   byte padding = precision -1;
   while(precision--)
 mult *=10;

   if(val >= 0)
frac = (val - int(val)) * mult;
   else
frac = (int(val)- val ) * mult;
   unsigned long frac1 = frac;
   while( frac1 /= 10 )
padding--;
   while(  padding--)
Serial.print("0");
   Serial.print(frac,DEC) ;
 }
}

// function to display a big number and free it afterwards
void printBignum (BigNumber n)
{
  char * s = n.toString ();
  Serial.println (s);
  free (s);
}  // end of printBignum
