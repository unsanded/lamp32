#ifndef __cc3000_spi_h
#define __cc3000_spi_h

#include "cc3000.h"

#ifdef	__cplusplus
extern "C" {
#endif


	#include "spi_hw.h"
        #define cc3000_spi_setup_io()	spi_hw_setup_io()
        #define cc3000_spi_hw_init()    spi_hw_init()
        #define cc3000_spi_send(x)      spi_hw_master_transmit(x)
        #define cc3000_spi_wait_done()  spi_hw_wait_done();
        #define cc3000_spi_recv()	spi_hw_master_receive()


#define SPI_OPERATION_WRITE  0x01
#define SPI_OPERATION_READ  0x03


uns8 cc3000_spi_start_send(uns16 hci_length);
void cc3000_spi_finish_send(uns8 padding_byte_required);
uns16 cc3000_spi_receive();

#ifdef	__cplusplus
}
#endif

#endif