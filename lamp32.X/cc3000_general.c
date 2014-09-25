

#include "cc3000_general.h"
#include "cc3000_hci.h"
#include "cc3000_spi.h"


uns8 cc3000_general_read_buffer_size(uns8 *free_buffers, uns16 *buffer_length) {

uns8 status;
uns8 *ptr;

	cc3000_hci_send_command(HCI_COMMAND_READ_BUFFER_SIZE, 0, 0);
	cc3000_hci_wait_for(HCI_EVENT_READ_BUFFER_SIZE,0);
	
	status = wlan_rx_buffer[4];
	ptr = &wlan_rx_buffer[5];
	*free_buffers = *ptr++;
	*buffer_length = stream_to_uns16(ptr);
	return status;
	
}

void cc3000_general_startup(uns8 patches_request) {
	
	debug_str("--- cc3000_startup\r\n");
	
	cc3000_smart_config_complete = 0;
	cc3000_connected = 0;
	cc3000_dhcp_complete = 0;
	
	cc3000_module_disable();
	delay_ms(200);

	debug_str("Waiting for IRQ line to go high\r\n");
	// todo: put timeout here
	while (cc3000_read_irq_pin() != 1);

	cc3000_module_enable();
	
	//("Waiting for IRQ line to go low\r\n");
	// todo: put timeout here
	while (cc3000_read_irq_pin() != 0);
	
	cc3000_cs_enable();
	
	// Time for the magic start up sequence
	delay_us(50);
	debug_str("Sending start command\r\n");
	cc3000_spi_send(SPI_OPERATION_WRITE);	//0x01
	cc3000_spi_send(0);	// length msb
	cc3000_spi_send(5);	// length lsb
	cc3000_spi_send(0);	// busy 0
	
	delay_us(50);
	
	cc3000_spi_send(0);	// busy 1
	cc3000_spi_send(HCI_TYPE_COMMAND);
	cc3000_spi_send(HCI_COMMAND_SIMPLE_LINK_START & 0xff);
	cc3000_spi_send(HCI_COMMAND_SIMPLE_LINK_START >> 8);
	cc3000_spi_send(1);		// 1 byte payload
	cc3000_spi_send(0); // no patches

        delay_us(50);

	cc3000_cs_disable();
	
	debug_str("Waiting for IRQ line to go low (active)\r\n");
	while (cc3000_read_irq_pin() != 0 );
        
	cc3000_hci_receive();
	
        //debug_str("Waiting for IRQ line to go high (in active)\r\n");
	// todo: add timeout here
 	while (cc3000_read_irq_pin() != 1);
	
	// now turn interrupts on
	cc3000_interrupt_enable();

	// grab the free buffers and buffer length for future use
	
	uns8 status = cc3000_general_read_buffer_size(&free_buffers, &buffer_length);
	
	//debug_var("status=", status);
	//debug_str(" Free buffers= ");
	//debug_int(free_buffers);
	//debug_str(" Buffer length = ");
	//debug_int_hex_16bit(buffer_length);
	//debug_nl();
	//debug_str("startup complete\r\n");
}

void cc3000_general_shutdown() {

//	debug_str("\r\nSHUTDOWN---\r\n");
	cc3000_module_disable();
	delay_ms(200);

//	debug_str("Waiting for IRQ line to go high\r\n");
	while (cc3000_read_irq_pin() != 1);
	//debug_str("SHUTDOWN complete\r\n");
}	
