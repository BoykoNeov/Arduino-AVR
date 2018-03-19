#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#define F_CLK 16000000

#include <avr/io.h>


  .global MultiplyTwo8UnsignedBitValuesUsingMul
  .global MultiplyTwo8SsignedBitValuesUsingMuls

  MultiplyTwo8UnsignedBitValuesUsingMul:
  
  mul r24, r22
  movw r24, r0
  sub r1, r1    //set r1 to 0
  ret

  MultiplyTwo8SsignedBitValuesUsingMuls:
  muls r24, r22 // multiply
  movw r24, r0  // move to return register
  sub r1, r1    // set r1 to 0
  ret           // return
