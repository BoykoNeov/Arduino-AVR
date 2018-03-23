// based on examples from https://sites.google.com/site/avrasmintro/home/2b-basic-math

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#define F_CLK 16000000

#include <avr/io.h>

  .global MultiplyTwo8UnsignedBitValuesUsingMul
  .global MultiplyTwo8SsignedBitValuesUsingMuls
  .global Multiply8bitSignedWithUnsignedUsingMulsu
  .global Multiply8BitBy2UsingLslAndRol
  .global ManualMultiplyTwo8BitNumbers
  .global MultiplyTwo16BitUnsignedIntsUsingMul
  .global ManualMultiplyTwo16BitUnsignedInts
  .global ManuallyMultiplyTwo32BitUnsignedInts
  
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
    ldi r20 ,8        //Load bit counter into C
    lsr R24          //Shift low-bit of multiplier into Carry

  LOOP:
    brcc SKIP        //If carry is zero then skip addition 
    add r25 ,r22      //Add multiplicand to answer

  SKIP:
    ror r25          //Shift low-bit of high-byte 
    ror r24          //of answer into low-byte (and since this originally holds the multiplier, the same operaion feeds the carry and at the time of the counter reaching 0, all original bits of the multiplier have been shifted
    dec r20          //Decrement bit-counter
    brne LOOP        //Check if done all eight bits
    ret

 MultiplyTwo16BitUnsignedIntsUsingMul:
    mul r25, r23  // multiply high bytes of multiplier and multiplicand
    movw r20, r0  // move the result into high bytes of the answer
    mul r24, r22  // multiply the low bytes
    movw r18, r0  // move the result into low bytes of answer
    mul r25, r22  // first cross multiply (high of multiplier to low of multiplicand)
    add r19, r0   // add the product to middle bytes
    adc r20, r1
    clr r1        // clears the r1 register, which by convention contains 0
    adc r21, r1   // by adding zero, the carry flag is added to the answer high byte
    mul r24, r23  // repeat with the other cross product
    add r19, r0
    adc r20, r1
    clr r1
    adc r21, r1
    movw r24, r20 // move the result to return registers
    movw r22, r18
    ret

 ManualMultiplyTwo16BitUnsignedInts:
  MUL16x16:
    ldi r26, 16        //load bit counter into counter
    clr 18            //clear answer registers
    clr 19
    clr 20
    clr 21

  LOOP16:
    lsr r25           //shift high-bit of multiplier into carry
    ror r24           //rotate lower bit into carry flag
    brcc SKIP16         //If carry is zero then skip addition 
    add r20, r22      //Add Multiplicand into high bytes of result
    adc r21, r23

  SKIP16:
    ror r21          //rotate high bytes of result into lower bytes
    ror r20          
    ror r19
    ror r18
    dec r26          //Decrement bit-counter
    brne LOOP16        //Check if done all sixteen bits
    movw r24, r20   //move answer into return registers
    movw r22, r18
    ret

    ManuallyMultiplyTwo32BitUnsignedInts:
  MUL32x32:
    push r17
    push r16
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    ldi r26, 32
    movw r16, r24
    movw r14, r22
    movw r12, r20
    movw r10, r18
    clr r25
    clr r24
    clr r23
    clr r22
    clr r21
    clr r20
    clr r19
    clr r18
  LOOP32:
    lsr r17
    ror r16
    ror r15
    ror r14
    brcc SKIP32
    add r22, r10 
    adc r23, r11
    adc r24, r12
    adc r25, r13
  SKIP32:
    ror r25
    ror r24
    ror r23
    ror r22
    ror r21
    ror r20
    ror r19
    ror r18
    dec r26
    brne LOOP32
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop r16
    pop r17
    ret
