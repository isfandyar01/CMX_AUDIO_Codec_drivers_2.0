/*
 * functional_image_alog.h
 *
 *  Created on: Nov 4, 2022
 *      Author: Isfandyar Qureshi
 */

#ifndef INC_FUNCTIONAL_IMAGE_ALOG_H_
#define INC_FUNCTIONAL_IMAGE_ALOG_H_


#include <stdbool.h>
typedef bool fi_error_check;

void General_Reset_to_CMX72619();

fi_error_check Read_audio_fifo_out_level_reg_for_3_device_check_word();

void enable_codec_boot_control_gpios();

void set_boot_en1_en2_high();
void set_boot_en1_en2_low();
void set_boot_en1_en2_for_serial_mem();

fi_error_check Write_block();

#endif /* INC_FUNCTIONAL_IMAGE_ALOG_H_ */
