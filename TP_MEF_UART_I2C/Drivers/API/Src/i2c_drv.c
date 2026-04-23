/*
 * i2c_drv.c
 *
 *  Created on: Apr 23, 2026
 *      Author: Carlos Quispe
 */

#include "i2c_drv.h"
#include "lcd_pcf857.h"

extern I2C_HandleTypeDef hi2c1;

/**
  * @brief  LCD_Write_Byte Funciòn que saca el byte a escribir al LCD por I2C, en caso de trabajar de forma paralela reemplazar por
 * 			GPIO. Si hay un error queda el LED2 encendido y el micro en error
  * @param  uint8_t valor a sacar por el puerto
  * @return void
  * @author Carlos Quispe
  * @version 1.0
  * @date 16/4/2025
*/


void LCD_Write_Byte(uint8_t valor){
	if(HAL_I2C_Master_Transmit (&hi2c1,LCD_DIR<<1,&valor, sizeof(valor),HAL_MAX_DELAY)!=HAL_OK)Error_Handler();
}
