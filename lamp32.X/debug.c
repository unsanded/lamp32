#include "debug.h"
#include <xc.h>
#include <plib.h>

void uart_init(){
        UARTConfigure(UART_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
        UARTSetFifoMode(UART_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
        UARTSetLineControl(UART_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
        UARTSetDataRate(UART_MODULE_ID, 50E6, 115200);
        UARTEnable(UART_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    }

void uart_puts(const char* str)
    {
    while(*str)
    {
        uart_putc(*str);

            str++;
    }

        while(!UARTTransmissionHasCompleted(UART_MODULE_ID));

}

void uart_putc(char c)
{
     while(!UARTTransmitterIsReady(UART_MODULE_ID));
    UARTSendDataByte(UART_MODULE_ID, c);

}

    char uart_getc(){
        while (!UARTReceivedDataIsAvailable(UART_MODULE_ID));

        return UARTGetDataByte(UART_MODULE_ID);

    }



void uart_send_int(int data)
{
    if(!data)
        uart_putc('0');

    while(data){
        uart_putc((data%10) | '0');
        data/=10;
    }
}
void uart_send_hex08(int data)
{
    char nibbles=2;
    while(nibbles--){
        char nibble=(data&0xf0)>>4;
        if(nibble>9)
            uart_putc(nibble + ('A'-10));
        else
            uart_putc(nibble | '0');
        data<<=4;
    }
}
void uart_send_hex16(int data)
{
    char nibbles=4;
    while(nibbles--){
        char nibble=(data&0xf000)>>12;
        if(nibble>9)
            uart_putc(nibble + ('A'-10));
        else
            uart_putc(nibble | '0');
        data<<=4;
    }
}
