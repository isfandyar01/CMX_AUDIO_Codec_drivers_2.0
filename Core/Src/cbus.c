/*
 * cbus.c
 *
 *  Created on: Oct 26, 2022
 *      Author: Isfandyar Qureshi
 */
#include "cbus.h"
#include "timer6_delay.h"
//working



void CBUS_INIT() {

	// pg9 sck pg10 miso pg11 mosi pg0 cs pg0

	enable_clock_gpio_spi();	//enabled G clocks // enabled SPI3 clock;
	set_cbus_pins(); // set sck mosi miso chip select  alternate function and speed and output type
	set_spi_portg_alternate_fun6(); //

}


void enable_clock_gpio_spi() {

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN; //enabled G clocks
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN; // enabled SPI3 clock;
}
//working
void set_cbus_pins() {

	// chip select PG0 chip select output
	GPIOG->MODER |= GPIO_MODER_MODE0_0;
	GPIOG->MODER &= ~(GPIO_MODER_MODE0_1);
	// pull up-down settings
	GPIOG->PUPDR |= GPIO_PUPDR_PUPD0_1;
	GPIOG->PUPDR &= ~ GPIO_PUPDR_PUPD0_0;
	// very high speed
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_1;
	GPIOG->OSPEEDR |= ( GPIO_OSPEEDR_OSPEED0_0);

	// chip select PG1 chip select output
	GPIOG->MODER |= GPIO_MODER_MODE1_0;
	GPIOG->MODER &= ~(GPIO_MODER_MODE1_1);
	// very high speed
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED1_1;
	GPIOG->OSPEEDR |= ( GPIO_OSPEEDR_OSPEED1_0);

	// output push pull
	GPIOG->OTYPER |= (GPIO_OTYPER_OT_0);
	//pulled up
	GPIOG->PUPDR |= GPIO_PUPDR_PUPD0;
	GPIOG->PUPDR &= ~GPIO_PUPDR_PUPD1;

	// PC9 alternate function SCK

	GPIOG->MODER &= ~(GPIO_MODER_MODER9_0);
	GPIOG->MODER |= GPIO_MODER_MODER9_1;
	//  very high speed
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_1;
	GPIOG->OSPEEDR |= ( GPIO_OSPEEDR_OSPEED9_0);

	//sck ends

	// PG11 alternate function mosi
	//mosi config starts

	GPIOG->MODER |= GPIO_MODER_MODE11_1;
	GPIOG->MODER &= ~(GPIO_MODER_MODE11_0);

	// high speed
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED11_1;
	GPIOG->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEED11_0);
	// pushpull
	GPIOG->OTYPER &= ~(GPIO_OTYPER_IDR_11);

	//mosi config ends

	// PG10 alternate function miso
	GPIOG->MODER |= GPIO_MODER_MODE10_1;
	GPIOG->MODER &= ~(GPIO_MODER_MODE10_0);
	//  high speed
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED10_1;
	GPIOG->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEED10_0);
	// push pull
	GPIOG->OTYPER &= ~(GPIO_OTYPER_IDR_10);
	// pulled down
	GPIOG->PUPDR |= GPIO_PUPDR_PUPD10_1;
	GPIOG->PUPDR &= ~(GPIO_PUPDR_PUPD10_0);
	//miso_ends

}

//working
void set_spi_portg_alternate_fun6() {
	// pg9 10 11 alternation function 6 for spi3 0110
	GPIOG->AFR[1] |= (GPIO_AFRH_AFSEL9_1);
	GPIOG->AFR[1] |= (GPIO_AFRH_AFSEL9_2);
	GPIOG->AFR[1] |= (GPIO_AFRH_AFSEL10_1);
	GPIOG->AFR[1] |= (GPIO_AFRH_AFSEL10_2);
	GPIOG->AFR[1] |= (GPIO_AFRH_AFSEL11_1);
	GPIOG->AFR[1] |= (GPIO_AFRH_AFSEL11_2);
}
//working

//working
void CBUS_CHIP_SElECT_LOW() {
	GPIOG->BSRR |= GPIO_BSRR_BR0;

}
//working
void CBUS_CHIP_SElECT_HIGH() {
	//GPIOG->ODR |=GPIO_ODR_OD0;
	GPIOG->BSRR |= GPIO_BSRR_BS0;
	Delay_US(10);
}

void CBUS_CHIP_SElECT_OUPUT_CODEC_LOW() {
	GPIOG->BSRR |= GPIO_BSRR_BR1;
}

void CBUS_CHIP_SElECT_OUPUT_CODEC_HIGH() {
	GPIOG->BSRR |= GPIO_BSRR_BS1;
	Delay_US(10);
}

// working configs cbus baud rate and other settings
void Cbus_Config(SPI_TypeDef *SPI_PORT) {

	//// fclk mhz 40mhz /64 ///10mhz for uncompressed clean voice
	SPI_PORT->CR1 &= ~ SPI_CR1_BR_2;

	SPI_PORT->CR1 |= SPI_CR1_BR_1;

	SPI_PORT->CR1 &= ~ SPI_CR1_BR_0;

	//SPI_PORT-> mode 3

	SPI_PORT->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;

	// FULL duplex
	SPI_PORT->CR1 &= ~ SPI_CR1_RXONLY;
	// msb first
	SPI_PORT->CR1 &= ~ SPI_CR1_LSBFIRST;

	//software slave
	SPI_PORT->CR1 |= SPI_CR1_SSM;
	SPI_PORT->CR1 |= SPI_CR1_SSI;

	// master mode enabled
	SPI_PORT->CR1 |= SPI_CR1_MSTR;

	// 8BIT DATA

	SPI_PORT->CR2 |= (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2);

	//SPI_PORT->

}

void Cbus_Write_no_data(SPI_TypeDef *SPI_PORT, uint8_t addr) {
	while (CBUS_BUSY(SPI_PORT))
		;
	CBUS_SendFrame(SPI_PORT, addr);
	return;
}

void CbusWriteRegister(SPI_TypeDef *SPI_PORT, uint8_t Addr, uint16_t data) ///working
{
	uint16_t swapped = swap_uint16(data);
	uint8_t address_and_Value[3];
	uint8_t address = Addr;
	address_and_Value[2] = address;
	address_and_Value[1] = (swapped & 0xff);
	address_and_Value[0] = ((swapped >> 8) & (0xff));
	//while(CBUS_BUSY(SPI_PORT));

	while (CBUS_BUSY(SPI_PORT)) {
	};
	__disable_irq();
	CBUS_SendFrame(SPI_PORT, address_and_Value[2]);
	CBUS_SendFrame(SPI_PORT, address_and_Value[1]);
	CBUS_SendFrame(SPI_PORT, address_and_Value[0]);
	__enable_irq();
	while (CBUS_BUSY(SPI_PORT)) {
	};
	__enable_irq();
	return;
}

void Cbus_Write_Word_stream(SPI_TypeDef *SPI_PORT, uint8_t Addr,
		uint16_t *pdata, uint16_t num_of_bytes_to_write) {

	uint8_t address = Addr;
	uint16_t i = 0;
	uint16_t Data;
	bool iIrqDisabler;

	while (CBUS_BUSY(SPI_PORT))
		;
	__disable_irq();
	iIrqDisabler = true;
	CBUS_SendFrame(SPI_PORT, address);
	for (i = 0; i < num_of_bytes_to_write; ++i) {
		if (!CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT)) {
			if (iIrqDisabler != false) {
				__enable_irq();
				iIrqDisabler = false;
			}
			while (!CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT));
		}

		Data = (pdata[i] >> 8);
		CBUS_SendFrame(SPI_PORT, Data);
		Data = pdata[i];
		while (!CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT));
		CBUS_SendFrame(SPI_PORT, Data);
	}
	if (iIrqDisabler != false) {
		__enable_irq();
	}

	return;

}

void Cbus_Read_Word_stream(SPI_TypeDef *SPI_PORT, uint8_t Addr, uint16_t *pdata,
		uint16_t num_of_bytes_to_read) {

	uint16_t address;
	uint16_t received_data;
	uint16_t wVal;
	bool iIRQ_Disabler;
	int iOut;
	int iOcount;
	int iLsb;
	int iIn;

	address = Addr;

	while (CBUS_BUSY(SPI_PORT))
		;
	while (CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT)) // check existing data and dump it
	{

		received_data = CBUS_ReceiveFrame(SPI_PORT); // dumping junk data

	}
	__disable_irq();
	iIRQ_Disabler = true;
	CBUS_SendFrame(SPI_PORT, address);
	iOut = 0;
	iOcount = num_of_bytes_to_read << 1; /* Twice as many bytes need sending */
	iIn = -1;

	iLsb = 0; /* Not looking for the LSB */
	while (1) {
		if (iOut < iOcount) {
			if (CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT)) // if tx buffer empty send frame
					{

				CBUS_SendFrame(SPI_PORT, iOut + 1);
				iOut += 1;

			}

			else {
				if (iIRQ_Disabler != false) {
					__enable_irq();
					iIRQ_Disabler = 0;
				}
			}
		}

		else {
			if (iIRQ_Disabler != false) {
				__enable_irq();
				iIRQ_Disabler = false;
			}
		}
		if (CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT)) // if not empty receive frames
				{
			wVal = CBUS_ReceiveFrame(SPI_PORT);
			if (iIn >= 0) /* Reading actual data */
			{
				if (iLsb == 0) {
					received_data = wVal << 8;
					iLsb = 1;
				} else {
					received_data |= wVal;
					pdata[iIn] = received_data;
					iIn += 1;
					if (iIn >= num_of_bytes_to_read) {
						break; /* We have all the data */
					}
					iLsb = 0;
				}
			} else {
				iIn += 1;
			}
		}
	}

	return;
}

uint8_t Cbus_Read_Byte(SPI_TypeDef *SPI_PORT, uint8_t Addr) {

	uint8_t address = Addr;
	uint16_t received_data = 0;

	while (CBUS_BUSY(SPI_PORT))
		;
	while (CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT)) /// check for existing data and dump it
	{
		received_data = CBUS_ReceiveFrame(SPI_PORT); /// dump all junk data
	}
	__disable_irq();
	CBUS_SendFrame(SPI_PORT, address);
	CBUS_SendFrame(SPI_PORT, 1);
	__enable_irq();
	while (!CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT)); //stay here until rx fifo empty
	received_data = CBUS_ReceiveFrame(SPI_PORT);

	return received_data;
}

uint32_t CBUS_READ_TWO_WORDS(SPI_TypeDef *SPI_PORT, uint8_t addr) {


	uint8_t address = addr;
	uint32_t received_data;

	while (CBUS_BUSY(SPI_PORT));
	while (CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT)) {
		received_data = CBUS_ReceiveFrame(SPI_PORT); /// dump all junk data
	}
	__disable_irq();
	CBUS_SendFrame(SPI_PORT, address);
	CBUS_SendFrame(SPI_PORT, 1);
	CBUS_SendFrame(SPI_PORT, 2);
	__enable_irq();
    while(!CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT));
    received_data=CBUS_ReceiveFrame(SPI_PORT);
    received_data<<=8;
    while(!CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT));
    received_data|=CBUS_ReceiveFrame(SPI_PORT);
    received_data<<=8;
    while(!CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT));
    received_data |= (CBUS_ReceiveFrame(SPI_PORT) & 0xff);
	return (uint32_t) received_data;

}

uint16_t CBUS_ReceiveFrame(SPI_TypeDef *SPI_PORT)
{

	    static uint16_t RX_data;
	     return RX_data =(uint16_t)(SSP_DR_BITMASK(*((volatile uint8_t*) &(SPI_PORT->DR))));
}





void Cbus_Read_Byte_stream(SPI_TypeDef *SPI_PORT,uint8_t Addr,uint8_t *pdata,uint16_t num_of_bytes_to_read)
{

	uint8_t address;
	uint8_t received_data;
	uint8_t wVal;
	bool iIRQ_Disabler;
	int iOut;
	int iOcount;
	int iLsb;
	int iIn;

	address = Addr;



	ddress = Addr;

	while (CBUS_BUSY(SPI_PORT))
		;
	while (CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT)) // check existing data and dump it
	{

		received_data = CBUS_ReceiveFrame(SPI_PORT); // dumping junk data

	}
	__disable_irq();
	iIRQ_Disabler = true;
	CBUS_SendFrame(SPI_PORT, address);
	iOut = 0;
	iOcount = num_of_bytes_to_read << 1; /* Twice as many bytes need sending */
	iIn = -1;

	while (1) {
		if (iOut < iOcount) {
			if (CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT)) // if tx buffer empty send frame
					{

				CBUS_SendFrame(SPI_PORT, iOut + 1);
				iOut += 1;

			}

			else {
				if (iIRQ_Disabler != false) {
					__enable_irq();
					iIRQ_Disabler = 0;
				}
			}
		}

		else {
			if (iIRQ_Disabler != false) {
				__enable_irq();
				iIRQ_Disabler = false;
			}
		}
		if (CBUS_RX_FIFO_NOT_EMPTY_CHECK(SPI_PORT)) // if not empty receive frames
				{
			wVal = CBUS_ReceiveFrame(SPI_PORT);
			if (iIn >= 0) /* Reading actual data */
			{
				if (iLsb == 0) {
					received_data = wVal;
					iLsb = 1;
				} else {
					received_data |= wVal;
					pdata[iIn] = received_data;
					iIn += 1;
					if (iIn >= num_of_bytes_to_read) {
						break; /* We have all the data */
					}
					iLsb = 0;
				}
			} else {
				iIn += 1;
			}
		}
	}

	return;


}


void Cbus_Write_Byte_stream(SPI_TypeDef *SPI_PORT, uint8_t Addr,uint8_t *pdata,uint16_t num_of_bytes_to_write){


	uint8_t *pdata;
	uint16_t num_of_bytes_to_write; 

	uint8_t address = Addr;
	uint16_t i = 0;
	uint8_t Data;
	bool iIrqDisabler;


	while (CBUS_BUSY(SPI_PORT));
	__disable_irq();
	iIrqDisabler = true;
	CBUS_SendFrame(SPI_PORT, address);
	for (i = 0; i < num_of_bytes_to_write; ++i) {
		if (!CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT)) {
			if (iIrqDisabler != false) {
				__enable_irq();
				iIrqDisabler = false;
			}
			while (!CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT));
		}

		Data = (pdata[i]);
		CBUS_SendFrame(SPI_PORT, Data);
		while (!CBUS_TX_BUFFER_EMPTY_CHECK(SPI_PORT));
	}
	if (iIrqDisabler != false) {
		__enable_irq();
	}






}