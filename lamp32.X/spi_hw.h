/* 
 * File:   spi_hw.h
 * Author: david
 *
 * Created on 17 September 2014, 17:18
 */
#include "main.h"
#ifndef SPI_HW_H
#define	SPI_HW_H

#ifdef	__cplusplus
extern "C" {
#endif

void   spi_hw_setup_io();
void  spi_hw_init();
char spi_hw_master_transmit(char byte);

void spi_hw_wait_done();

char spi_hw_master_receive();


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_HW_H */

