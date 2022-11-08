/*
 * cbus.h
 *
 *  Created on: Oct 26, 2022
 *      Author: Isfandyar Qureshi
 */

#ifndef INC_CBUS_H_
#define INC_CBUS_H_

#include"stm32l4a6xx.h"
#include "stm32l4xx.h"
#include <stdbool.h>

#define CBUS_SPEED 10000000
#define SSP_DR_BITMASK(n)   ((n) & 0xFFFF)

void CBUS_INIT();

void Cbus_Config(SPI_TypeDef *SPI_PORT);

void CBUS_CHIP_SElECT_LOW();
void CBUS_CHIP_SElECT_HIGH();
void CBUS_CHIP_SElECT_OUPUT_CODEC_LOW();
void CBUS_CHIP_SElECT_OUPUT_CODEC_HIGH();
void enable_clock_gpio_spi();

void set_spi_portg_alternate_fun6();

void set_cbus_pins();




void CbusWriteRegister(SPI_TypeDef *SPI_PORT, uint8_t Addr,uint16_t data);
//void Cbus_Write_Word_stream(SPI_TypeDef *SPI_PORT, uint8_t Addr,uint16_t *pdata,uint16_t num_of_bytes_to_write);

//void Cbus_Read_Word_stream(SPI_TypeDef *SPI_PORT,uint8_t Addr,uint16_t *pdata,uint16_t num_of_bytes_to_read);

void Cbus_Write_Word_stream(SPI_TypeDef *SPI_PORT, uint8_t Addr,uint16_t *pdata,uint16_t num_of_bytes_to_write);

void Cbus_Read_Word_stream(SPI_TypeDef *SPI_PORT,uint8_t Addr,uint16_t *pdata,uint16_t num_of_bytes_to_read);

uint8_t Cbus_Read_Byte(SPI_TypeDef * SPI_PORT ,uint8_t Addr);

uint16_t Cbus_Read_Word(SPI_TypeDef * SPI_PORT ,uint8_t Addr);

void Cbus_Write_no_data(SPI_TypeDef *SPI_PORT,uint8_t addr);


uint32_t CBUS_READ_TWO_WORDS(SPI_TypeDef *SPI_PORT,uint8_t addr);



static inline FlagStatus CBUS_BUSY(SPI_TypeDef *SPI_PORT) {
	return (SPI_PORT->SR & (1 << 7)) ? SET : RESET;
}

static inline FlagStatus CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_TypeDef *SPI_PORT) {
	return (SPI_PORT->SR & (1 << 0)) ? SET : RESET;
}

static inline uint16_t CBUS_ReceiveFrame(SPI_TypeDef *SPI_PORT) {
	uint16_t RX_data;
	return RX_data =(uint16_t)(SSP_DR_BITMASK(*((volatile uint8_t*) &(SPI_PORT->DR))));
	//return RX_data=SPI_PORT->DR;

	//return (uint16_t) (SSP_DR_BITMASK(SPI_PORT->DR));
}


static inline void CBUS_SendFrame(SPI_TypeDef *SPI_PORT, uint8_t tx_data)
{
	*((volatile uint8_t*) &(SPI_PORT->DR)) =SSP_DR_BITMASK(tx_data); //(uint16_t*) *((volatile uint8_t*) &
}






static inline FlagStatus CBUS_TX_BUFFER_EMPTY_CHECK(SPI_TypeDef *SPI_PORT) {
	return (SPI_PORT->SR & (1 << 1)) ? SET : RESET;
}


static inline uint16_t swap_uint16( uint16_t val )
{
    return (val << 8) | (val >> 8 );
}

static inline void CBUS_ENABLE(SPI_TypeDef *SPI_PORT){
	SPI_PORT->CR1 |= SPI_CR1_SPE;
}

static inline void CBUS_DISABLE(SPI_TypeDef *SPI_PORT){
	SPI_PORT->CR1 &= ~SPI_CR1_SPE;
}





static inline  uint16_t CBUS_Receive_BYTE_Frame(SPI_TypeDef *SPI_PORT)
{
	return (uint16_t) (SSP_DR_BITMASK(SPI_PORT->DR));
}















#endif /* INC_CBUS_H_ */
