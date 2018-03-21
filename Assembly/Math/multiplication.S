// all based on examples from https://sites.google.com/site/avrasmintro/home/2b-basic-math

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#define F_CLK 16000000

#include <avr/io.h>


  .global MultiplyTwo8UnsignedBitValuesUsingMul
  .global MultiplyTwo8SsignedBitValuesUsingMuls
  .global Multiply8bitSignedWithUnsignedUsingMulsu
  .global Multiply8BitBy2UsingLslAndRol
  .global ManualMultiplyTwo8BitNumbers

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

  ManualMultiplyTwo8BitNumbers:
  MUL8x8:
    ldi r20,8        //Load bit counter into C
    lsr R24          //Shift low-bit of multiplier into Carry

  LOOP:
    brcc SKIP        //If carry is zero then skip addition 
    add r25,r22      //Add multiplicand to answer

  SKIP:
    ror r25          //Shift low-bit of high-byte 
    ror r24          //of answer into low-byte (and since this originally holds the multiplier, the same operaion feeds the carry and at the time of the counter reaching 0, all original bits of the multiplier have been shifted
    dec r20          //Decrement bit-counter
    brne LOOP        //Check if done all eight bits
    ret
