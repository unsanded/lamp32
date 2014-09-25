/*

Copyright (c) 2013, Embedded Adventures, www.embeddedadventures.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
 
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

Contact us at admin@embeddedadventures.com

*/

#include "cc3000_hci.h"
#include "cc3000.h"
#include "cc3000_spi.h"

#include "debug.h"

#define HCI_COMMAND_HEADER_LENGTH 4
#define HCI_DATA_HEADER_LENGTH 5


uns8 padding_byte_required;

/*

HCI Command

uns8 	HCI message type
uns16 	HCI opcode (LSB first)
uns8	HCI arguments length
uns8    HCI arguments (HCI arguments length bytes long)

*/

void cc3000_hci_send_command(uns16 opcode, uns8 args_length, uns8 *args) {
	
uns8 padding_byte;
	padding_byte = cc3000_spi_start_send(HCI_COMMAND_HEADER_LENGTH + args_length);
//	debug_str("\r\nHCI header:\r\n");
	cc3000_spi_send(HCI_TYPE_COMMAND);
	cc3000_spi_send(opcode & 0xff);
	cc3000_spi_send(opcode >> 8);
	cc3000_spi_send(args_length);	
	if (args_length > 0) {
                int count;
		for ( count = 0; count < args_length; count++) {
			cc3000_spi_send(args[count]);
		}
	}	
//	debug_str("SPI padding:\r\n");
	cc3000_spi_finish_send(padding_byte);
}


uns8 cc3000_hci_start_command(uns16 opcode, uns8 args_length) {
	
	
	padding_byte_required = cc3000_spi_start_send(HCI_COMMAND_HEADER_LENGTH + args_length);
	
	cc3000_spi_send(HCI_TYPE_COMMAND);
	cc3000_spi_send(opcode & 0xff);
	cc3000_spi_send(opcode >> 8);
	cc3000_spi_send(args_length);
	
	return padding_byte_required;
}

void cc3000_hci_end_command() {
	cc3000_spi_finish_send(padding_byte_required);
}

uns8 cc3000_hci_start_data(uns8 opcode, uns8 args_length, uns16 data_length) {

uns16 payload_length;

	payload_length = args_length + data_length;

        padding_byte_required = cc3000_spi_start_send(HCI_DATA_HEADER_LENGTH + payload_length);

	cc3000_spi_send(HCI_TYPE_DATA);
	cc3000_spi_send(opcode);
	cc3000_spi_send(args_length);
	cc3000_spi_send(payload_length & 0xff);
	cc3000_spi_send(payload_length >> 8);
	
	return padding_byte_required;
}

		
void cc3000_hci_end_data() {
	cc3000_spi_finish_send(padding_byte_required);
}

void cc3000_hci_send_uns32(uns32 u) {

	
	cc3000_spi_send((uns8) (u & 0xff));
	
	cc3000_spi_send((uns8) ((u >> 8 ) & 0xff));
	
	cc3000_spi_send((uns8) ((u >> 16 ) & 0xff));
	
	cc3000_spi_send((uns8) ((u >> 24 ) & 0xff));

}

void cc3000_hci_send_uns16(uns16 u) {

	
	cc3000_spi_send((uns8) (u & 0xff));
	
	cc3000_spi_send((uns8) ((u >> 8 ) & 0xff));

}

void cc3000_hci_send_uns8(uns8 u) {

	cc3000_spi_send(u);
	
}

/*

HCI Data

uns8 	HCI message type
uns8 	HCI opcode (LSB first) [Yes, inconcistanct with HCI command, which takes a 16bit opcode]
uns8	HCI arguments length
uns16   HCI data length + arguments length (payload length)
uns8    HCI arguments (HCI arguments length bytes long)
uns8	HCI data (data length bytes long)

*/
void cc3000_hci_send_data(uns8 opcode, uns8 args_length, uns8 *args, uns16 data_length, uns8 *data) {

uns8 padding_byte;
uns16 payload_length;

	payload_length = args_length + data_length;

	//debug_str("\r\n---\nSending data opcode = ");
	debug_int_hex(opcode);
	debug_nl();

//	debug_str("\r\nSPI header:\r\n");
	padding_byte = cc3000_spi_start_send(HCI_DATA_HEADER_LENGTH + payload_length);

//	debug_str("\r\nHCI header:\r\n");
	cc3000_spi_send(HCI_TYPE_DATA);
	cc3000_spi_send(opcode);
	cc3000_spi_send(args_length);
	cc3000_spi_send(payload_length & 0xff);
	cc3000_spi_send(payload_length >> 8);
	if (args_length > 0) {
            uns8 count;
		for (count = 0; count < args_length; count++) {
			cc3000_spi_send(args[count]);
			debug_int_hex(args[count]);
			debug_putc(' ');
		}
	}
	
	if (data_length > 0) {
                int count;
		for (count = 0; count < data_length; count++) {
			cc3000_spi_send(data[count]);
			debug_int_hex(data[count]);
			debug_putc(' ');
		}
	}
		
		
	cc3000_spi_finish_send(padding_byte);
}

uns16 cc3000_hci_handle_received_transaction() {

uns8 hci_type;
uns16 hci_opcode, hci_args_length;
		

	hci_type = wlan_rx_buffer[0];
	
	
	switch (hci_type) {
		case HCI_TYPE_EVENT:
			hci_opcode = wlan_rx_buffer[2];
			hci_opcode = hci_opcode << 8;
			hci_opcode = hci_opcode + wlan_rx_buffer[1];
			
			hci_args_length = wlan_rx_buffer[4];
			hci_args_length = hci_args_length << 8;
			hci_args_length = hci_args_length + wlan_rx_buffer[3];
			
//
			switch (hci_opcode) {
				case HCI_EVENT_WLAN_ASYNC_SMART_CONFIG_DONE: 
					cc3000_smart_config_complete = 1;
					debug_str("***Async smart config complete\r\n");
					break;
				case HCI_EVENT_WLAN_KEEPALIVE:  
					debug_str("***Keep alive\r\n"); 
					break;	
				case HCI_EVENT_WLAN_UNSOL_CONNECT:
					cc3000_callback_connect_status(1, 0);
					cc3000_connected = 1;
					debug_str("***Connect!!!!\r\n");
					break;
				case HCI_EVENT_WLAN_UNSOL_DISCONNECT:
					cc3000_callback_connect_status(0, 0);
					cc3000_connected = 0;
					cc3000_dhcp_complete = 0;
					debug_str("***Disconnect\r\n");	
					break;
				case HCI_EVENT_WLAN_UNSOL_DHCP:
					cc3000_callback_connect_status(1, 1);
					cc3000_dhcp_complete = 1;
					debug_str("***DHCP\nIP: ");	

					debug_int(wlan_rx_buffer[8]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[7]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[6]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[5]);
					debug_str("GW: ");

					debug_int(wlan_rx_buffer[16]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[15]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[14]);
					debug_putc('.');
					debug_int(wlan_rx_buffer[13]);
					debug_nl();

					break;
				case HCI_EVENT_WLAN_UNSOL_INIT:
					debug_str("***Init complete\r\n");	//– Notification that the CC3000 device 
					break;
				case HCI_EVENT_WLAN_ASYNC_PING_REPORT:
				
					debug_str("***Ping complete\nPkts SENT:");
					debug_int(wlan_rx_buffer[5]); // - 8 
					debug_str("\r\nPkts RECV:");
					debug_int(wlan_rx_buffer[9]); // - 12
					debug_str("\r\nMin time:");
					debug_int(wlan_rx_buffer[13] + (uns16)(wlan_rx_buffer[14] << 8));
					//debug_str("\r\nMax time:");
					debug_int(wlan_rx_buffer[17] + (uns16)(wlan_rx_buffer[18] << 8));
					//debug_str("\r\nAvg time:");
					debug_int(wlan_rx_buffer[21] + (uns16)(wlan_rx_buffer[22] << 8));
					debug_nl();
				
					break;
				case HCI_EVENT_DATA_UNSOL_FREE_BUFF:
					debug_str("Free buffer!\r\n");
					uns8 buffer_count = wlan_rx_buffer[5]; // 16 bits but who cares

                                        uns8 count;
					for (count = 0; count < buffer_count; count++) {
						uns16 my_buffers_free = wlan_rx_buffer[5+2+2+(count*2)];
						//debug_var(" Buffers_free+ = ", my_buffers_free);
						free_buffers += my_buffers_free;
					}
					debug_nl();
					break;	
			}			
			break;
		case HCI_TYPE_PATCH:
			debug_str("PATCH: ");
			break;
		case HCI_TYPE_DATA:
			//debug_str("\r\nDATA: ");
			hci_opcode = wlan_rx_buffer[1];	// 8 bit opcode
			
			hci_args_length = wlan_rx_buffer[2];	// 8 bit args
			//debug_str(" Data opcode = ");
			//debug_int_hex_16bit(hci_opcode);
		
			break;
		case HCI_TYPE_COMMAND:
			//debug_str("COMMAND: ");
			break;
	}
	return hci_opcode;	
}

uns16 cc3000_hci_receive() {
	
//        debug_str("\r\nSPI Receive\r\n");
	cc3000_spi_receive();
	// now process at higher layer (HCI)
//	debug_str("\r\nNow process in HCI\r\n");
	uns16 hci_opcode = cc3000_hci_handle_received_transaction();
	//debug_var("hci_opcode=", hci_opcode);
	cc3000_cs_disable();
	return hci_opcode;
}

hci_status_t cc3000_hci_wait_for(uns16 wait_for_command, unsigned timeout) {



uns16 command=0;
hci_status_t status;
static bit interrupts_enabled;

	interrupts_enabled = cc3000_interrupts_enabled;
	if (interrupts_enabled) {
		cc3000_interrupt_disable();
	}
	//debug_str("\r\nhci_wait_for: Waiting for irq...\r\n");

	do {
//            delay_ms(1000);

            unsigned oldTime=ReadCoreTimer();
            unsigned diff;
		while (cc3000_read_irq_pin() != 0){
                    unsigned time = ReadCoreTimer();
                    diff+=time-oldTime;
                    oldTime=time;
                    while(oldTime>SYS_FREQ){
                        oldTime-=SYS_FREQ;
                        timeout--;
                        if(timeout==0){
                            return -1;
                        }
                    }

                }
		//debug_str("\r\nGOT IRQ\r\n");
		command = cc3000_hci_receive();

	} while ( command != wait_for_command );

	//debug_int_hex_16bit(wait_for_command);
	//debug_nl();

	// grab status before we enable interrupts
	status = wlan_rx_buffer[POS_HCI_STATUS];
	
	if (interrupts_enabled) {	// if we had them on, then turn them back on

		cc3000_interrupt_enable();
	}	
	return 	status; 
}

