/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#ifdef __XC32
    #include <xc.h>          /* Defines special funciton registers, CP0 regs  */
#endif

#include <plib.h>            /* Include to use PIC32 peripheral libraries     */
#include <stdint.h>          /* For UINT32 definition                         */
#include <stdbool.h>         /* For true/false definition                     */
#include <exception>         /* Includes C++ try/catch functions              */

#include "user.hpp"          /* variables/params used by user.cpp             */

#include "cc3000.h"
#include "cc3000_wlan.h"
#include "cc3000_general.h"
#include "cc3000_nvmem.h"

/* All the files in the C++ standard library declare its entities
 * within the std namespace. */
using namespace std;        /* use the standard namespace                     */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* TODO Initialize User Ports/Peripherals/Project here.  Users may want to
 * initialize peripheral classes here.  */

void InitApp(void)
{
    PPSInput (2,U2RX,RPB5  ); // Assign RPB5 as input pin for U2RX
    PPSOutput(4,RPB14,U2TX );

    uart_init();


    //spi config
    PPSOutput(3, RPB2, SDO2);// mosi
    PPSInput (3, SDI2, RPA2);// miso
    ANSELBbits.ANSB3=0; //irq

    ANSELAbits.ANSA1=0;
    //a2 has no analog function


    SPI2CONbits.CKP=0;
    SPI2CONbits.CKE=0;

    SPI2CONbits.MODE16=0;
    SPI2CONbits.MODE32=0;
    SPI2CONbits.MSTEN=1;


    SpiChnSetBitRate(SPI_CHANNEL2, SYS_FREQ, 1E6);


    SpiChnEnable(SPI_CHANNEL2, 1);



    cc3000_setup_io();
    cc3000_init();
    cc3000_general_startup(0);

    uns8 major, minor;
    cc3000_nvmem_read_sp_version(&major, &minor);
    debug_var(major);
    debug_var(minor);


    cc3000_wlan_set_connection_policy(fast_connect_enabled, open_ap_auto_connect_disabled, use_profiles_enabled);




    con_status_enum s=con_disconnected;

    WDTCONbits.ON=0;
    do{
        delay_ms(1000);
        cc3000_wlan_get_status(&s);
        if(s==con_disconnected)
                cc3000_wlan_connect((unsigned char*) "Superduperfrappalicious", WLAN_SEC_WPA2, (unsigned char*) "weetenvanavondbijdeturk");
    }while(s!=con_connected);

    cc3000_wlan_set_event_mask(HCI_EVENT_WLAN_KEEPALIVE);
}
