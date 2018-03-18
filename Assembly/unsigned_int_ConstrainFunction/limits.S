#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#define F_CLK 16000000

#include <avr/io.h>

// functions takes 3 unsigned ints as inputs A, B, C (in that order)
// if A < B , returns B
// if A > C , returns C
// else returns A
// depeneding of case, takes at most 24 cycles (including function call and return) - around 1.5 microseconds at 16 Mhz
.global ConstrainAssembly
  
ConstrainAssembly:
  movw r18, r22 // copy word B to registers 18 and 19
  sub r18, r24 // substract low byte r18-r24, save at r18
  sbc r19, r25 // substract with carry high byte r19-r25 , save at r19 - these 2 ops substract word B - A
  brpl returnB // branch if minus (it is minus if B < A), so we must return B

   // compare A to C, copying A into a spare register, and substracting C
   
  movw r18, r24 // copy a to r18-r19
  sub r18, r20 // substract low byte
  sbc r19, r21 // substract with carry high byte
  brpl returnC
  ret

returnB:
  movw r24, r22 // copy B to return register r24 and r25
  ret // return B

returnC:
  movw r24, r20 // copy C to return register r24 and r25
  ret
