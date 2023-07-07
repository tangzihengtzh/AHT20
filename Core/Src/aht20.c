/*
 * aht20.c
 *
 *  Created on: Jul 1, 2023
 *      Author: tzh15
 */

#include "aht20.h"
#define AHT20_ADDRESS 0x70

void AHT20INIT()
{
	uint8_t readbuffer;
	HAL_Delay(40);
	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readbuffer, 1, HAL_MAX_DELAY);
	if(readbuffer & 0x08 == 0x00)
	{
		uint8_t sendbffer[3]={0xBE,0x08,0x00};
		HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendbffer, 3, HAL_MAX_DELAY);
	}
}

void AHT20READ(float *tem,float *hum)
{
	uint8_t sendbffer[3]={0xAC,0x33,0x00};
	uint8_t readbuffer[6];

	HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendbffer, 3, HAL_MAX_DELAY);
	HAL_Delay(75);

	HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, readbuffer, 6, HAL_MAX_DELAY);

	if( (readbuffer[0] & 0x80) == 0x00)
	{
		uint32_t data=0;
		data=((uint32_t)readbuffer[3]>>4) +
				((uint32_t)readbuffer[2]<<4) +
				((uint32_t)readbuffer[1]<<12);
		*hum = data*100.0f/(1<<20);
		data=((uint32_t)(readbuffer[3] & 0x0F)<<16) +
						((uint32_t)readbuffer[4]<<8) +
						((uint32_t)readbuffer[5]);
		*tem=data*200.0f/(1<<20)-50;
	}
	else
	{
		*hum=-1;
		*tem=-1;
	}
}







