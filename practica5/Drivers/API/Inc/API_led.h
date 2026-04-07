/*
 * API_GPIO.h
 *
 *  Created on: Apr 6, 2026
 *      Author: CARLOSQL
 */

#ifndef API_INC_API_LED_H_
#define API_INC_API_LED_H_

#include "stdint.h"
#include "stdbool.h"
#include "main.h"


void led_on(void);				// Enciende led
void led_off(void);				// Apaga led
void led_toggle(void);			// Cambia de estado led
bool led_status(void);			// Retorna estado de led

#endif /* API_INC_API_LED_H_ */
