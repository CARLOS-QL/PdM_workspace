/*
 * i2c_drv.h
 *
 *  Created on: Apr 23, 2026
 *      Author: CARLOSQL
 */

#ifndef API_INC_I2C_DRV_H_
#define API_INC_I2C_DRV_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


//_Bool LCD_HW_init(void);
void LCD_Write_Byte(uint8_t valor);
//void GPIO_I2C(I2C_HandleTypeDef  *hi2c);

#endif /* API_INC_I2C_DRV_H_ */
