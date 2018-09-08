#include <math.h>
#include <BigNumber.h>

void setup() {
  Serial.begin(9600);

  double test = 0;
  double input1 = 0;
  double input2 = 0;

// double acos (double __x)
// The acos() function computes the principal value of the arc cosine of __x. The returned value is in the range [0, pi] radians.
// A domain error occurs for arguments not in the range [-1, +1].
Serial.println("Acos");
printDouble(acos(-1));
Serial.println();
printDouble(acos(0));
Serial.println();
printDouble(acos(0.32354));
Serial.println();
printDouble(acos(1));
Serial.println();
Serial.println(acos(-2));
Serial.println(acos(2));
Serial.println();

// double fabs (double __x)
// The fabs() function computes the absolute value of a floating-point number __x.
Serial.println("fabs");
printDouble(fabs(-252.3453));
Serial.println();

// double fmod (double __x, double__y)
// The function fmod() returns the floating-point remainder of __x / __y.
Serial.println("fmod");
printDouble(fmod(3.5, 1.5555));
Serial.println();

BigNumber::begin ();  // initialize library
BigNumber a, b = 2, c;
BigNumber::setScale (50);
a = BigNumber (1) / BigNumber (3);
c = b.sqrt ();
Serial.print ("1/3 = ");
printBignum (a);
Serial.print ("sqrt(2) = ");
printBignum (c);
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
