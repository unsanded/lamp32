/* 
 * File:   debug.h
 * Author: david
 *
 * Created on 16 September 2014, 23:15
 */

#ifndef DEBUG_H
#define	DEBUG_H

#define UART_MODULE_ID UART2


#ifdef	__cplusplus
extern "C" {
#endif

    void uart_init();
    void uart_putc(char c);
    void uart_puts(const char* str);
    char uart_getc();



    void uart_send_int(int data);
    void uart_send_hex08(int data);
    void uart_send_hex16(int data);

     #define debug_str(x) uart_puts(x)
    #define debug_int_hex(x) uart_send_hex08(x)
    #define debug_int(x) uart_send_int(x)
    #define debug_var(x) {uart_puts( #x ); uart_putc('='); uart_send_hex16(((int)x)>>16); uart_send_hex16((int)x);uart_putc('\n');}

    #define debug_nl() uart_putc('\r');uart_putc('\n')
    #define debug_int_hex_16bit(x) uart_send_hex16(x)
    #define debug_int_hex_32bit(x) {uart_send_hex16(x<<16);uart_send_hex16(x)}
    #define debug_putc(x) uart_putc(x)



#ifdef	__cplusplus
}
#endif

#endif	/* DEBUG_H */

