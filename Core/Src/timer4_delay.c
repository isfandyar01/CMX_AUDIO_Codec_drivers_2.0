/*
 * timer4_delay.c
 *
 *  Created on: Nov 11, 2022
 *      Author: Isfandyar Qureshi
 */


#include <timer6_delay.h>

volatile int myTicks=0;
void enable_timer6(){

	RCC->APB1ENR1|=RCC_APB1ENR1_TIM6EN;
	TIM6->PSC =80-1;
	TIM6->ARR =0xffff;
	TIM6->CR1 |= TIM_CR1_CEN;

	while (!(TIM6->SR & (1<<0)));

}



void Delay_US(uint16_t US){
	TIM6->CNT = 0;
	while (TIM6->CNT < US);
}

