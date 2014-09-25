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


#include "cc3000.h"

//#include "debug.h"
#include "cc3000_hci.h"
#include "cc3000_general.h"
#include "cc3000_spi.h"


uns8 wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];
uns16 spi_length;

uns8 free_buffers;
uns16 buffer_length;

bit cc3000_smart_config_complete = 0;
bit cc3000_connected = 0;
bit cc3000_dhcp_complete = 0;
bit cc3000_interrupts_enabled = 0;

void cc3000_setup_io() {
    debug_str("a");
    CC3000_CS=1;
    debug_str("b");
    CC3000_CS_TRIS=0;
    debug_str("c");

    debug_str("d");
    CC3000_EN=0;
    debug_str("e");
        CC3000_EN_TRIS=0;
    debug_str("f");
	cc3000_spi_setup_io();
    debug_str("g");
}

void cc3000_init() {
	cc3000_spi_hw_init();
}

void cc3000_interrupt_enable() {
	//debug_str("Interupt ENable\r\n");
//        IOCB0=1;
//        INT0IE=1;
	cc3000_interrupts_enabled = 1;
}

void cc3000_interrupt_disable() {
//	debug_str("Interupt DISable\r\n");
	cc3000_interrupts_enabled = 0;
}

void cc3000_module_enable() {
//	debug_str("Module ENable\r\n");
        CC3000_EN=1;
}

void cc3000_module_disable() {
//	debug_str("Module DISable\r\n");
	cc3000_interrupt_disable();
       CC3000_EN=0;
}

void cc3000_handle_isr() {
	cc3000_hci_receive();
}	


uns8 cc3000_read_irq_pin() {
	if (CC3000_INT) {
		return 1;
	} else {
		return 0;
	}
}

void cc3000_cs_enable() {

//	debug_str("CS ENable\r\n");
    CC3000_CS=0;
}

void cc3000_cs_disable() {

        spi_hw_wait_done();
        CC3000_CS=1;
//        debug_str("CS DISable\r\n");
}

	


uns8 * uns32_to_stream( uns8 * i, uns32 n) {
	
	*(i)++ = (uns8) (n & 0xff);			// lsb
	*(i)++ = (uns8) ((n >> 8 ) & 0xff);
	*(i)++ = (uns8) ((n >> 16) & 0xff);
	*(i)++ = (uns8) ((n >> 24) & 0xff);	// msb
	return i;
}

uns8 * uns16_to_stream( uns8 * i, uns16 n) {
	
	*(i)++ = (uns8) (n & 0xff);				// lsb first
	*(i)++ = (uns8) ((n >> 8 ) & 0xff);		// msb second

	return i;
}

uns16 stream_to_uns16(uns8 * i) {
	return	(uns16)(*i);
}

// we ARE little endian!
								
uns32 stream_to_uns32(uns8 * i) {
	return	(uns32) *i;
}





