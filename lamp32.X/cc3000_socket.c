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

#include "cc3000_socket.h"
#include "cc3000.h"
#include "cc3000_hci.h"
#include <string.h>

int cc3000_socket(uns8 domain, uns8 type, uns8 protocol)
{

hci_status_t status;



	cc3000_hci_start_command(HCI_COMMAND_SOCKET, 3*4);

	cc3000_hci_send_uns32(domain);
	cc3000_hci_send_uns32(type);
	cc3000_hci_send_uns32(protocol);

	cc3000_hci_end_command();

	status = cc3000_hci_wait_for(HCI_EVENT_SOCKET, 0);

        int sd=0;
        sd|=((int) wlan_rx_buffer[8]<<24);
        sd|=((int) wlan_rx_buffer[7]<<16);
        sd|=((int) wlan_rx_buffer[6]<<8 );
        sd|=((int) wlan_rx_buffer[5]    );
        return sd;
}

int32 cc3000_accept(uns8 sd)
{

hci_status_t status;



	cc3000_hci_start_command(HCI_COMMAND_ACCEPT, 4);

	cc3000_hci_send_uns32(sd);

	cc3000_hci_end_command();

	status = cc3000_hci_wait_for(HCI_EVENT_ACCEPT, 0);

        int newsd=0;
        newsd|=((int) wlan_rx_buffer[12]<<24);
        newsd|=((int) wlan_rx_buffer[11]<<16);
        newsd|=((int) wlan_rx_buffer[10]<<8 );
        newsd|=((int) wlan_rx_buffer[ 9]    );

        return newsd;
}


hci_status_t cc3000_connect(uns8 sd, uns16 port, ip_addr_t addr)
{

    debug_str("connecting");
	cc3000_hci_start_command(HCI_COMMAND_CONNECT, 20);
        cc3000_hci_send_uns32(sd);
	cc3000_hci_send_uns32(0x00000008);	// magic
	cc3000_hci_send_uns32(8);	// addrlen
	cc3000_hci_send_uns16(AF_INET /*stSockAddr.sin_framily */);
	cc3000_hci_send_uns16(htons(port) /*stSockAddr.sin_port */);
	cc3000_hci_send_uns32( * ((uns32*) &addr) /*stSockAddr.sin_addr*/);
	cc3000_hci_end_command();

	return cc3000_hci_wait_for(HCI_EVENT_CONNECT, 0);

}
hci_status_t cc3000_setsockopt(uns8 sd, uns32 level, uns32 optname, const uns8* optval, uns32 optlen)
{

	cc3000_hci_start_command(HCI_COMMAND_SETSOCKOPT, 20+optlen);
        cc3000_hci_send_uns32(sd);
	cc3000_hci_send_uns32(level);
	cc3000_hci_send_uns32(optname);
	cc3000_hci_send_uns16(0x00000008);
	cc3000_hci_send_uns16(optlen);
        while(optlen--)
            cc3000_hci_send_uns8(*optval++);

        cc3000_hci_end_command();

	return cc3000_hci_wait_for(HCI_EVENT_SETSOCKOPT, 0);

}

hci_status_t cc3000_listen(uns8 sd)
{

	cc3000_hci_start_command(HCI_COMMAND_LISTEN, 8);
        cc3000_hci_send_uns32(sd);
	cc3000_hci_send_uns32(0);
	cc3000_hci_end_command();

	return cc3000_hci_wait_for(HCI_EVENT_LISTEN, 0);

}


hci_status_t cc3000_bind(uns8 sd, ip_addr_t addr, uns16 port)
{

	cc3000_hci_start_command(HCI_COMMAND_BIND, 28);
        cc3000_hci_send_uns32(sd);
	cc3000_hci_send_uns32(0x00000008);
	cc3000_hci_send_uns32(2+2+4+8);
	cc3000_hci_send_uns16(AF_INET);
	cc3000_hci_send_uns16(htons(port));
	cc3000_hci_send_uns32(
                                * ((uns32*) &addr)
                );
        cc3000_hci_send_uns32(0);
        cc3000_hci_send_uns32(0);

	cc3000_hci_end_command();

	return cc3000_hci_wait_for(HCI_EVENT_BIND, 0);

}


#define POS_RECV_PARAMS_SD 			POS_HCI_STATUS 				+ 1
#define POS_RECV_PARAMS_NUM_BYTES 	POS_RECV_PARAMS_SD			+ 4
#define POS_RECV_PARAMS_FLAGS 		POS_RECV_PARAMS_NUM_BYTES	+ 4


bit interrupts_enabled;

int32 cc3000_recv(uns8 sd, uns8 *buf, uns16 len, uns8 flags, int timeout){

	
	interrupts_enabled = cc3000_interrupts_enabled;
	if (interrupts_enabled) {
		cc3000_interrupt_disable();
	}
	
	cc3000_hci_start_command(HCI_COMMAND_RECV, 3*4);

	cc3000_hci_send_uns32(sd);
	cc3000_hci_send_uns32(len);
	cc3000_hci_send_uns32(flags);
	
	cc3000_hci_end_command();
	//debug_str("\r\nWaiting for response...");

	hci_status_t status = cc3000_hci_wait_for(HCI_EVENT_RECV, timeout);

        if(status<0){
            debug_str("RECV TIMEOUT \r\n");
            return 0;
        }
	// In return, we get 
	// int32 SocketDescriptor (8 bits will do us) - 5
	// int32 NumberOfBytes	(16 bits will do us)
	// int32 Flags
	
	// We will just assume that we are dealing with the socket we first thought of
	// In a more complex case of multiple sockets, we would need to separate this out
	// and probably have a more complex model (ie, a state machine)
	
	// uns8  sd2 = wlan_rx_buffer[POS_RECV_PARAMS_SD];
	int32 num_bytes = stream_to_uns16(&wlan_rx_buffer[POS_RECV_PARAMS_NUM_BYTES]);
	//uns8  flags2 = wlan_rx_buffer[POS_RECV_PARAMS_FLAGS];
	
	debug_var(sd);
	debug_var(num_bytes);
	
	// In case the number of bytes is more then zero - read data
	
	if (num_bytes > 0) {

//		debug_str("*^*^*^*^*^*^*^*\r\n");
//		debug_str("waiting for data\r\n");
		status = cc3000_hci_wait_for(HCI_DATA_RECV, 0);
		uns8 hci_opcode = wlan_rx_buffer[1]; 		// 8 bit opcode for data
		uns8 hci_args_length = wlan_rx_buffer[2];	// 8 bit args length
		
		debug_str("\r\n opcode = ");
		debug_int_hex(hci_opcode);
		debug_str("\r\n args_length = ");
		debug_int_hex(hci_args_length);
		debug_nl();			
			
		uns16 payload_length = wlan_rx_buffer[3] + (uns16)(wlan_rx_buffer[4] << 8);
		uns16 data_length = payload_length - hci_args_length;
		
//		debug_var(" payload_len = ", payload_length);
//		debug_var("data_len = ", data_length);
//		debug_str("\r\nData=<");

                int count;
		for (count = 0; count < data_length; count++) {
				debug_putc(wlan_rx_buffer[5 + hci_args_length + count]);
				*buf = wlan_rx_buffer[5 + hci_args_length + count];
				buf++;
			}
		
//		debug_str(">\n\r\n");
	}
//	debug_str("returning\r\n");
	
	if (interrupts_enabled) {	// if we had them on, then turn them back on
		cc3000_interrupt_enable();
	}	
	return num_bytes;
}



int16 cc3000_send(uns8 sd, uns8 *buf, uns16 len, uns8 flags) {

uns8 args[16];
uns8 *ptr;

	if (free_buffers == 0) {
		debug_str("Send: No free buffers!\r\n");
		return -2; // no free buffers;
	}
	
	free_buffers--;	// strangely, we the host must keep track of free buffers
	
	cc3000_hci_start_data(HCI_COMMAND_SEND, 16, len);
	cc3000_hci_send_uns32(sd);
	cc3000_hci_send_uns32(16-4);	
	cc3000_hci_send_uns32(len);
	cc3000_hci_send_uns32(flags);
	
	if (len > 0) {
		debug_str("\r\nData:\r\n");
                int count;
		for ( count = 0; count < len; count++) {
			cc3000_hci_send_uns8(buf[count]);
			debug_putc(buf[count]);
		}
	}
	cc3000_hci_end_data();
	
	return len;
}

hci_status_t cc3000_socket_close(uns8 sd) {


	cc3000_hci_start_command(HCI_COMMAND_CLOSE_SOCKET, 4);
	cc3000_hci_send_uns32(sd);
	cc3000_hci_end_command();
	
	hci_status_t status = cc3000_hci_wait_for(HCI_EVENT_CLOSE_SOCKET, 0);
	
	return status;

}

hci_status_t cc3000_socket_mdns_advertise(uns8 mdns_enabled, uns8 *service_name) {

uns8 service_name_length;

	service_name_length = strlen(service_name);
	if (service_name_length > MDNS_DEVICE_SERVICE_MAX_LENGTH) {
		return -1;
	}
	cc3000_hci_start_command(HCI_COMMAND_MDNS_ADVERTISE, 12 + service_name_length);
	cc3000_hci_send_uns32(mdns_enabled);
	cc3000_hci_send_uns32(8);	// magic
	cc3000_hci_send_uns32(service_name_length);
        int count;
	for (count = 0; count < service_name_length; count++) {
		cc3000_hci_send_uns8(service_name[count]);
	}	
	cc3000_hci_end_command();
	
	return cc3000_hci_wait_for(HCI_EVENT_MDNS_ADVERTISE, 0);
	
}