    /******************************************************************************/
/*  Files to Include                                                          */
/******************************************************************************/

#include <exception>        /* Includes C++ try/catch functions               */

#include "debug.h"
#include "user.hpp"         /* User funct/params, such as InitApp             */

#include "main.h"

#include "system.h"
#include "socket.hpp"
#include "HttpServer.h"
#include "Pages.h"       /* System funct/params, like osc/periph config    */

/* All the files in the C++ standard library declare its entities
 * within the std namespace. */
using namespace std;        /* use the standard namespace                     */





/******************************************************************************/
/* Global Variable/Classes Declaration                                        */
/******************************************************************************/

/* TODO add variables and classes. These can be organized many ways.  */

/* i.e. INT32 <variable_name>; */

/* Simple C++ class example */
/*
 * class c_rectangle
 * {
 *     UINT32 width, height;
 *   public:
 *     // The class constructor, note no return parameter
 *     c_rectangle(int,int); 
 *     // area() may be called with a c_rectangle type variable var.area();
 *     UINT32 area() {return (width*height);}
 * };
 *
 * // class constructor, :: is the scope operator
 * c_rectangle::c_rectangle(int w,int h) 
 * {
 *    width=w;
 *    height=h;
 * } */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/





INT main(void)
{



#ifndef PIC32_STARTER_KIT
    /*The JTAG is on by default on POR.  A PIC32 Starter Kit uses the JTAG, but
    for other debug tool use, like ICD 3 and Real ICE, the JTAG should be off
    to free up the JTAG I/O */
    DDPCONbits.JTAGEN = 0;
#endif

    /*Refer to the C32/XC32 peripheral library documentation for more
    information on the SYTEMConfig function.

    This function sets the PB divider, the Flash Wait States, and the DRM
    /wait states to the optimum value.  It also enables the cacheability for
    the K0 segment.  It could has side effects of possibly alter the pre-fetch
    buffer and cache.  It sets the RAM wait states to 0.  Other than
    the SYS_FREQ, this takes these parameters.  The top 3 may be '|'ed
    together:

    SYS_CFG_WAIT_STATES (configures flash wait states from system clock)
    SYS_CFG_PB_BUS (configures the PB bus from the system clock)
    SYS_CFG_PCACHE (configures the pCache if used)
    SYS_CFG_ALL (configures the flash wait states, PB bus, and pCache)*/

    /* TODO Add user clock/system configuration code if appropriate.  */
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);



    /* Initialize I/O and Peripherals for application */
    InitApp();

    /*Configure Multivector Interrupt Mode.  Using Single Vector Mode
    is expensive from a timing perspective, so most applications
    should probably not use a Single Vector Mode*/
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);

    char      buf[1024];



    ip_addr_t addr;
    addr.ip[0]=192;
    addr.ip[1]=168;
    addr.ip[2]=1;
    addr.ip[3]=59;

    delay_ms(5000);
    {
        HttpServer server;

  //      socket s(SOCK_STREAM);
//        s.listen(90);

        int n=0;
        addPages(server);
        while(1)
        {
//            socket ss(s.accept());

/*            if(ss){
                char input[100];
                int ilen=0;
                while(ilen<10){
                    debug_nl();
                    ilen+=ss.recv(&input[ilen],90-ilen, 10);
                    debug_int_hex(ilen);
                    delay_ms(1000);
                }
                input[ilen]=0;
                debug_str(input);
                ss << "HTTP/1.1 200 OK\r\n"
//                        <<"Content-Length: 10\r\n"
//                        <<"\r\nHello world"
                        ;

            }
 */
            delay_ms(100);
            server.tick();

        }
    }
}


void cc3000_callback_connect_status(uns8 connected, uns8 dhcp){

    if(connected)
        debug_str("Connected\r\n");
    if(dhcp)
       debug_str("dhcp\r\n");


}
