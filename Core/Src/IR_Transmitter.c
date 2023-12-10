/*
 * IR_Transmitter.c
 *
 *  Created on: Dec 7, 2023
 *      Author: Mega-PC
 */


#include "IR_Transmitter.h"
void Transmit_IR(uint32_t p){
	int i=31;
	uint32_t temp=p;
	do
	{
		if((temp & 0x0001) == 1)
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
		}
		temp=(temp>>1);
		i--;
	}
	while (i!=0);

}
