# spi_cbus
 cbus_for_codec_cmx7261


it uses stm32L4a6gz on spi3 bus to communicate with cmx7261 audio codec

pin used are from portG of stm32 and are commented in cbus.c file

cbus.c file contains all the spi settings 

best spi speed is 2-4mhz 

this code and operate on two codecs using single spi bus