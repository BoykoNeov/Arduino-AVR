// calculate sine of x with 'precision' iterations
BigNumber sineFromRadians (const BigNumber x, BigNumber precision)
{
  BigNumber val = 1;
  const BigNumber one = 1, two = 2;
  while (precision > 0)
  {
    val = one - (((val * x) * x) / (two * precision) / (two * precision + one));
    precision--;
  }
  val = x * val;
  return val;
} // end of function sine

BigNumber sineFromDegrees (const BigNumber y, BigNumber precision)
{
  char bufer[78];
  strcpy_P(bufer, Pi_String);
  BigNumber pi = BigNumber(bufer);
  BigNumber x = (pi / BigNumber(180)) * y;
  BigNumber val = 1;
  const BigNumber one = 1, two = 2;
  while (precision > 0)
  {
    val = one - (((val * x) * x) / (two * precision) / (two * precision + one));
    precision--;
  }
  val = x * val;
  return val;
} // end of function sine

BigNumber cosineFromRadians (const BigNumber x, BigNumber precision)
{
  char bufer[78];
  strcpy_P(bufer, Pi_String);
  BigNumber pi = BigNumber(bufer);
  x = (pi / BigNumber(2)) - x;
  
  BigNumber val = 1;
  const BigNumber one = 1, two = 2;
  while (precision > 0)
  {
    val = one - (((val * x) * x) / (two * precision) / (two * precision + one));
    precision--;
  }
  val = x * val;
  return val;
} // end of function sine

BigNumber cosineFromDegrees (const BigNumber y, BigNumber precision)
{
  char bufer[78];
  strcpy_P(bufer, Pi_String);
  BigNumber pi = BigNumber(bufer);
  
  BigNumber x = (pi / BigNumber(180)) * (BigNumber(90) - y);
  BigNumber val = 1;
  const BigNumber one = 1, two = 2;
  while (precision > 0)
  {
    val = one - (((val * x) * x) / (two * precision) / (two * precision + one));
    precision--;
  }
  val = x * val;
  return val;
} // end of function sine

BigNumber degreesToRadians (const BigNumber y)
{
  
}

