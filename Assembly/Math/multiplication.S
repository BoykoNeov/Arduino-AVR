#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#define F_CLK 16000000

#include <avr/io.h>


  .global MultiplyTwo8UnsignedBitValuesUsingMul
  .global MultiplyTwo8SsignedBitValuesUsingMuls
  .global Multiply8bitSignedWithUnsignedUsingMulsu
  .global Multiply8BitBy2UsingLslAndRol

  MultiplyTwo8UnsignedBitValuesUsingMul:
  
  mul r24, r22
  movw r24, r0
  clr r1    //set r1 to 0 - clr - Clears a register. This instruction performs an Exclusive OR between a register and itself. This will clear all bits in the register.
  ret

  MultiplyTwo8SsignedBitValuesUsingMuls:
  muls r24, r22 // multiply
  movw r24, r0  // move to return register
  sub r1, r1    // set r1 to 0 - alternative to clr
  ret           // return

  Multiply8bitSignedWithUnsignedUsingMulsu:
  muls r24, r22 // multiply
  movw r24, r0  // move to return register
  eor r1, r1    // set r1 to 0 - alternative to clr (does the same)
  ret           // return

  Multiply8BitBy2UsingLslAndRol:
  lsl r24   // logical shift left - shift left without carring about the carry flag (shift in 0)
  rol r25   // rotate left through carry - shift left, with placing the content of the carry flag into the least significant bit
  dec r22   // decrement by 1 (this is about the second variable - how many times to repeat the operation
  brne Multiply8BitBy2UsingLslAndRol //branches if the zero flag is not set
  ret
