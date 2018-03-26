#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0
#define F_CLK 16000000

#include <avr/io.h>

.global DelayCyclesX4
.global DelayCyclesX6

    // Takes as input (uint16) x cycles and returns after (x times 4 cycles) + around 9;
  DelayCyclesX4:
    ldi r26, 1
  loopX4Micro:
    sub r24, r26
    sbc r25, r1
    brne loopX4Micro
    ret

    // Takes as input (uint32) x cycles and returns after (x times 6 cycles) + around 11, 0 is MaxValue;
  DelayCyclesX6:
    ldi r26, 1
  loopXXMicro:
    sub r22, r26
    sbc r23, r1
    sbc r24, r1
    sbc r25, r1
    brne loopXXMicro
    ret
