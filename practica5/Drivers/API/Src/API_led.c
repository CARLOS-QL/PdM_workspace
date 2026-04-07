/*
 * API_GPIO.c
 *
 *  Created on: Apr 6, 2026
 *      Author: Carlos Quispe
 */

#include <API_led.h>


/**
 * @brief Enciende el LED.
 * Coloca el pin asociado al LED en nivel alto (GPIO_PIN_SET),
 * lo que provoca que el LED se encienda.
 */
void led_on(void){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}


/**
 * @brief Apaga el LED.
 * Coloca el pin asociado al LED en nivel bajo (GPIO_PIN_RESET),
 * lo que provoca que el LED se apague.
 */

void led_off(void){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}


/**
 * @brief Cambia el estado actual del LED.
 * Invierte el estado lógico del pin asociado al LED:
 */
void led_toggle(void){
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

/**
 * @brief Obtiene el estado actual del LED.
 * @return true  El pin está en nivel alto (LED encendido*)
 * @return false El pin está en nivel bajo (LED apagado*)
 */
bool led_status(void){
	GPIO_PinState ledStatus = HAL_GPIO_ReadPin(LED_GPIO_Port, LED_Pin);
	return ledStatus;
}

