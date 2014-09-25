/* 
 * File:   main.h
 * Author: david
 *
 * Created on 17 September 2014, 16:18
 */

#ifndef MAIN_H
#define	MAIN_H

#define CC3000_SPI_CHANNEL 2




#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif



#include <plib.h>           /* Include to use PIC32 peripheral libraries      */
#include <stdint.h>         /* For UINT32 definition                          */
#include <stdbool.h>        /* For true/false definition                      */

#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For UINT32 definition                         */
#include <stdbool.h>         /* For true/false definition                     */


#include "debug.h"


#ifdef	__cplusplus
#include <exception>         /* Includes try/catch functions                  */
extern "C" {
#endif

#define SYS_FREQ     50E6L
#define FCY          SYS_FREQ


void delay_ms(unsigned count);
void delay_us(unsigned count);


#define CC3000_EN LATAbits.LATA1
#define CC3000_EN_TRIS TRISAbits.TRISA1


#define CC3000_INT PORTBbits.RB3
#define CC3000_INT_TRIS TRISBbits.TRISB3

#define CC3000_CS LATAbits.LATA3
#define CC3000_CS_TRIS TRISAbits.TRISA3





#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

