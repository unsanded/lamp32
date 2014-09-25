
#include "cc3000_spi.h"
bit interrupts_enabled = 0;

uns8 cc3000_spi_start_send(uns16 hci_length) {

uns8 spi_header	[5];
uns16 spi_length = 5 + hci_length;

uns8 padding_byte_required = spi_length & 0x01;
	
//	debug_var("spi_start\nhci_length=", hci_length);
	
	if (padding_byte_required) {
		hci_length++;
//		debug_str("(+1 padding byte) ");
	}
	
	if (cc3000_interrupts_enabled) {
		interrupts_enabled = 1;
		cc3000_interrupt_disable();	
	}
	cc3000_cs_enable();
	
	//("Waiting for IRQ line to go low\r\n");
	while (cc3000_read_irq_pin() == CC3000_IRQ_IDLE);
	//debug_str("Got it\r\n");
	
	cc3000_spi_send(SPI_OPERATION_WRITE);	//0x01
	cc3000_spi_send(hci_length >> 8);	// length msb
	cc3000_spi_send(hci_length & 0xff);	// length lsb
	cc3000_spi_send(0);	// busy 0
	cc3000_spi_send(0);	// busy 1
	return (padding_byte_required);
	// interrupts still off at this point
}

void cc3000_spi_finish_send(uns8 padding_byte_required) {
	
	if (padding_byte_required) {
		cc3000_spi_send(0);
	}
	
	cc3000_cs_disable();
	if (interrupts_enabled) {
		interrupts_enabled = 0;
		cc3000_interrupt_enable();
	}	
}

uns16 cc3000_spi_receive() {

        	cc3000_cs_enable();

	cc3000_spi_send(SPI_OPERATION_READ);
	cc3000_spi_send(0);	// busy 0
	cc3000_spi_send(0);	// busy 1

	uns16 spi_length = cc3000_spi_recv();
	spi_length = spi_length << 8;
	spi_length += cc3000_spi_recv();
	
	if ( spi_length == 0 ) {
            debug_str("wrong length\r\n   ");
            debug_int_hex_16bit(spi_length);
		return(0);
	}	

        int count;
	for (count = 0; count < spi_length; count++) {
		wlan_rx_buffer[count] = cc3000_spi_recv();

        }
	// CS left disabled
        return 0;

}

