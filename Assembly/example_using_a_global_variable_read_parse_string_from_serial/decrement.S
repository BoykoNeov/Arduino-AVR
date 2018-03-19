#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#define F_CLK 16000000

#include <avr/io.h>

.extern longvar

.global DecrementBy1
  
  DecrementBy1:
  
  lds r18, longvar   // lds - LoadDirect from Dataspace (the data space consists of the register file, I/O memory and internal SRAM)
  lds r19, longvar+1
  lds r20, longvar+2
  lds r21, longvar+3
  movw r24, r20      // moves the variable to result return registers r25:r22
  movw r22, r18
  dec r22            // decrements the low byte by 1 (generaly, the whole number is decremented by 1, but funny things happen, when the low byte is 0 ;) )
  ret
