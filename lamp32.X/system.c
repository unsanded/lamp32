/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <limits.h>
#include "main.h"        /* variables/params used by system.cpp           */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.cpp                                         */
/*                                                                            */
/******************************************************************************/


void delay_ms(unsigned count){
    delay_us(count*1000);
}
void delay_us(unsigned count){
    unsigned startTime=ReadCoreTimer();
    unsigned endTime= startTime + (count* SYS_FREQ/1E6);
    if(endTime>UINT_MAX-100)// margin for safety. we don't want to wait a whole round
        endTime=0;
    debug_str("delay from ");
    debug_int_hex_16bit(startTime>>16);
    debug_int_hex_16bit(startTime>>00);
    debug_str("\r\nuntil      ");
    debug_int_hex_16bit(endTime>>16);
    debug_int_hex_16bit(endTime>>00);
    debug_nl();
    unsigned time;
    while((time=ReadCoreTimer())<endTime || (time>startTime && endTime<startTime) )//the second check is because the coreTimer regularly overflows
    {
        int c;
         for(c=0; c<10; c++)
            Nop();
    }
    debug_str(" done \r\n");

}

/* <Initialize variables in system.hpp and put code for system functs here.>  */
