/*
 * functional_image_alog.c
 *
 *  Created on: Nov 4, 2022
 *      Author: Isfandyar Qureshi
 */


#include <functional_image_array.h>
#include"functional_image_alog.h"
#include "cbus.h"
#include "CODEC_REG.h"


void enable_codec_boot_control_gpios()
{   //PG2 as output for boot control
	GPIOG->MODER |= GPIO_MODER_MODE2_0;
	GPIOG->MODER &= ~(GPIO_MODER_MODE2_1);
	// high speed
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_1;
	GPIOG->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEED2_0);

	// PG3 as output for boot control
	GPIOG->MODER |= GPIO_MODER_MODE3_0;
	GPIOG->MODER &= ~(GPIO_MODER_MODE3_1);
	// high speed
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED3_1;
	GPIOG->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEED3_0);

}


void set_boot_en1_en2_high(){

	GPIOG->BSRR |= GPIO_BSRR_BS2;
	GPIOG->BSRR |= GPIO_BSRR_BS3;
}


void set_boot_en1_en2_low(){

	GPIOG->BSRR &=~ GPIO_BSRR_BS2;
	GPIOG->BSRR &=~ GPIO_BSRR_BS3;
}

void set_boot_en1_en2_for_serial_mem(){

	GPIOG->BSRR |= GPIO_BSRR_BS2;
	GPIOG->BSRR &=~ GPIO_BSRR_BS3;
}

void General_Reset_to_CMX72619()
{
	// setting booten1 and booten2 to 1 for cbus boot mode
	CBUS_CHIP_SElECT_LOW();
	Cbus_Write_no_data(SPI3, General_Reset);
	CBUS_CHIP_SElECT_HIGH();
	//Cbus_Write_no_data(SPI3,General_Reset);
}

fi_error_check Read_audio_fifo_out_level_reg_for_3_device_check_word(){
	volatile uint8_t check_word_level;
    uint16_t wData;
	CBUS_CHIP_SElECT_LOW();
	check_word_level=Cbus_Read_Byte(SPI3,Audio_out_fifo_level);
	CBUS_CHIP_SElECT_HIGH();


	 while(check_word_level != 3){
		CBUS_CHIP_SElECT_LOW();
		check_word_level = Cbus_Read_Byte(SPI3, Audio_out_fifo_level);
		CBUS_CHIP_SElECT_HIGH();
		 if(check_word_level>=3)
		 	{
				for (int i = 0; i<3; i++)
					{
						//Read bootloader garbage words from CBUS FIFO
					    CBUS_CHIP_SElECT_LOW();
						wData = Cbus_Read_Byte(SPI3,Audio_out_fifo_data_word);
						CBUS_CHIP_SElECT_HIGH();
					}
		 		return true;
                break;
		 	}
		 else{
			 return false;
		 }
	 }
}


fi_error_check Write_block(){
    uint16_t wData;
    uint8_t audio_in_fifo_level;
//    uint32_t CHECKSUM_BLOCK_1=(DB1_CHK_HI<<16|DB1_CHK_LO );
    uint32_t CHECKSUM_BLOCK_2 =(DB2_CHK_HI<<16|DB2_CHK_LO);
    uint32_t CHECKSUM;






	CBUS_CHIP_SElECT_HIGH();
	// writing data block 1 length
	CBUS_CHIP_SElECT_LOW();
	CbusWriteRegister(SPI3, Audio_in_fifo_data_word, DB1_LEN);
	CBUS_CHIP_SElECT_HIGH();

	// writing data block 1 ptr
	CBUS_CHIP_SElECT_LOW();
	CbusWriteRegister(SPI3, Audio_in_fifo_data_word, DB1_PTR);
	CBUS_CHIP_SElECT_HIGH();



	//checking audio in fifo level but not using it
	CBUS_CHIP_SElECT_LOW();
	audio_in_fifo_level = Cbus_Read_Byte(SPI3, Audio_in_fifo_level);
	CBUS_CHIP_SElECT_HIGH();
	Delay_US(10);

    // pushing whole data block 1 arrays in audio in fifo
	CBUS_CHIP_SElECT_LOW();
	Cbus_Write_Word_stream(SPI3, Audio_in_fifo_data_word, db1, DB1_LEN);
	HAL_Delay(2);
	CBUS_CHIP_SElECT_HIGH();
	HAL_Delay(10);

	CBUS_CHIP_SElECT_HIGH();


	// retrieving the checksum
	CBUS_CHIP_SElECT_LOW();
	CHECKSUM = CBUS_READ_TWO_WORDS(SPI3, Audio_out_fifo_data_word) << 24;
	CHECKSUM |= CBUS_READ_TWO_WORDS(SPI3, Audio_out_fifo_data_word);
	CBUS_CHIP_SElECT_HIGH();
/// end of block 1

// start of block 2

	// writing data block 2 length
	Delay_US(10);
	CBUS_CHIP_SElECT_LOW();
	CbusWriteRegister(SPI3, Audio_in_fifo_data_word, DB2_LEN);
	CBUS_CHIP_SElECT_HIGH();

	// writing data block 2 ptr
	CBUS_CHIP_SElECT_LOW();
	CbusWriteRegister(SPI3, Audio_in_fifo_data_word, DB2_PTR);
	CBUS_CHIP_SElECT_HIGH();


	// pushing whole data block 2 arrays in audio in fifo
	CBUS_CHIP_SElECT_LOW();
	Cbus_Write_Word_stream(SPI3, Audio_in_fifo_data_word, db2, DB2_LEN);
	HAL_Delay(2);
	CBUS_CHIP_SElECT_HIGH();

// end of block 2

/// writing activation  len
	CBUS_CHIP_SElECT_LOW();
	CbusWriteRegister(SPI3, Audio_in_fifo_data_word,ACTIVATE_LEN);
	CBUS_CHIP_SElECT_HIGH();

///	writing activation ptr
	CBUS_CHIP_SElECT_LOW();
	CbusWriteRegister(SPI3, Audio_in_fifo_data_word, ACTIVATE_PTR);
	CBUS_CHIP_SElECT_HIGH();


	HAL_Delay(10);

	// retrieving the checksum
	CBUS_CHIP_SElECT_LOW();
	CHECKSUM = CBUS_READ_TWO_WORDS(SPI3, Audio_out_fifo_data_word) << 24;
	CHECKSUM |= CBUS_READ_TWO_WORDS(SPI3, Audio_out_fifo_data_word);
	CBUS_CHIP_SElECT_HIGH();

if (CHECKSUM==CHECKSUM_BLOCK_2)
{


	Delay_US(200);


	while(1){
			if((wData & 0x4000) != 0x4000){
				Delay_US(8);
				CBUS_CHIP_SElECT_LOW();
				wData=Cbus_Read_Byte(SPI3, IRQ_Status)<<8;
				CBUS_CHIP_SElECT_HIGH();


			}
			if((wData & 0x4000) == 0x4000){
				HAL_Delay(3);
				CBUS_CHIP_SElECT_LOW();
				wData = CBUS_READ_TWO_WORDS(SPI3,Audio_out_fifo_data_word)<<8;
				wData |= Cbus_Read_Byte(SPI3,Audio_out_fifo_data_word);

				CBUS_CHIP_SElECT_HIGH();
				break;
				return true;
			}

		}

}

else {

	return false;
}

}








