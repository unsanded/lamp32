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


#ifndef __cc3000_h
#define __cc3000_h

#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif


#define int8  signed char
#define int16 signed short
#define int32 signed int

#define uns8  unsigned char
#define uns16 unsigned short
#define uns32 unsigned int

#define bit bool



#define	MAC_ADDR_LEN	6

#define EFAIL -1

#define TINY_CC3000_MAXIMAL_RX_SIZE 44
#define TINY_CC3000_MAXIMAL_TX_SIZE 59





/*
Buffer size increased to 130 following the add_profile() with WEP security
  which requires TX buffer size of 130 bytes: 
  HEADERS_SIZE_EVNT + WLAN_ADD_PROFILE_WEP_PARAM_LEN + MAX SSID LEN + 4 * MAX KEY LEN = 130
  MAX SSID LEN = 32 
  MAX SSID LEN = 13 (with add_profile only ascii key setting is supported, 
  therfore maximum key size is 13)*/

// Given the maximal data size MAX_DATAthat that is expected to be received by application, the required buffer is:
// Using recv() or recvfrom():
// max(CC3000_MINIMAL_RX_SIZE, MAX_DATA + HEADERS_SIZE_DATA + fromlen + ucArgsize + 1)
// Using gethostbyname() with minimal buffer size will limit the host name returned to 99 bytes only.  
// The 1 is used for the overrun detection
#define CC3000_MINIMAL_RX_SIZE      (130+1)
#define CC3000_MAXIMAL_RX_SIZE      (1519 + 1)

// Given the maximal data size MAX_DATA, the required buffer is:
// Using Sendto:
// max(CC3000_MINIMAL_TX_SIZE, MAX_DATA + SPI_HEADER_SIZE + SOCKET_SENDTO_PARAMS_LEN + SIMPLE_LINK_HCI_DATA_HEADER_SIZE + 1)
// Using Send():
// max(CC3000_MINIMAL_TX_SIZE, MAX_DATA + SPI_HEADER_SIZE + HCI_CMND_SEND_ARG_LENGTH + SIMPLE_LINK_HCI_DATA_HEADER_SIZE + 1)
// The 1 is used for the overrun detection

#define	CC3000_MINIMAL_TX_SIZE      (130 + 1)  
#define	CC3000_MAXIMAL_TX_SIZE      (1519 + 1)

//#define CC3000_TX_BUFFER_SIZE 250 Not required for this implementation
#define CC3000_RX_BUFFER_SIZE 250

#define CC3000_IRQ_ACTIVE 0
#define CC3000_IRQ_IDLE   1

typedef union _ip_addr_t {
	uns8 ip[4];
        uns32 bytes;
} ip_addr_t;


//extern uns8 wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
extern uns8 wlan_rx_buffer[CC3000_RX_BUFFER_SIZE];
//extern uns16 spi_length;
extern uns8 free_buffers;
extern uns16 buffer_length;


extern bit cc3000_smart_config_complete;
extern bit cc3000_connected;
extern bit cc3000_dhcp_complete;
extern bit cc3000_interrupts_enabled;

/** 
 
    \brief Set up the cc3000 pins ready for I/O
 
    Configure pins for cc3000 I/O
 
*/
void cc3000_setup_io(void);

/** 
 
    \brief Initialise any hardware required to communicate with the cc3000
    
    Get any hardware ready for talking to the cc3000. In our case, we simply call the SPI hardware initialisation.
 
 
*/
void cc3000_init();

/** 
 
    \brief Enable interrupts from the IRQ line
 
	Enable interrupts from the IRQ line.

*/

void cc3000_interrupt_enable();

/** 
 
    \brief Disable interrupts from the IRQ line
 
*/
void cc3000_interrupt_disable();

/** 
 
    \brief Disable the module by pulling EN line low

*/

void cc3000_module_disable();

/** 
 
    \brief Enable the module by pulling EN line high
 


*/

void cc3000_module_enable();

/** 
 
    \brief Find out the value of the IRQ pin
 
	Query the value of the IRQ pin

*/

uns8 cc3000_read_irq_pin();

/** 
 
    \brief Enable CS pin
 
	Enable CS pin (set low)

*/

void cc3000_cs_enable();

/** 
 
    \brief Disable CS pin
 
	Disable CS pin (set high)

*/

void cc3000_cs_disable();


/** 
 
    \brief Interrupt service routine
 
	To be called when IRQ line goes high (and not waiting for it to happen), 
	this routine should call HCI/SPI receive
	
*/

void cc3000_handle_isr();

// convert between stream / internal data formats
uns16 stream_to_uns16(uns8 *i);
uns32 stream_to_uns32(uns8 *i);
uns8 * uns32_to_stream( uns8 * i, uns32 n);
uns8 * uns16_to_stream( uns8 * i, uns16 n);

#ifdef	__cplusplus
}
#endif


#endif
