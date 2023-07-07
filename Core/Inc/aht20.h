/*
 * aht20.h
 *
 *  Created on: Jul 1, 2023
 *      Author: tzh15
 */

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

#include "i2c.h"

void AHT20INIT();
void AHT20READ(float* tem,float* hum);


#endif /* INC_AHT20_H_ */
