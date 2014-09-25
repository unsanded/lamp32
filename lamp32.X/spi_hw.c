#include "spi_hw.h"

void   spi_hw_setup_io(){

}

void  spi_hw_init(){


}
char spi_hw_master_transmit(char byte) {
    SpiChnPutC(CC3000_SPI_CHANNEL, byte);

    char ret = SpiChnGetC(CC3000_SPI_CHANNEL);
    

    return ret;
}
void spi_hw_wait_done(){
    while(SpiChnIsBusy(CC3000_SPI_CHANNEL));
}

char spi_hw_master_receive()

{
    return spi_hw_master_transmit(0);
}
