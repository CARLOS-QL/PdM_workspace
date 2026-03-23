/*
 * API_delay.h
 *
 *  Created on: Mar 22, 2026
 *      Author: CARLOSQL
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include <stddef.h>


/**
 * @brief  Tipo de dato para manejo de tiempo en ticks
 */

typedef uint32_t tick_t;
typedef bool bool_t;


typedef struct {
    tick_t startTime;	// Tiempo inicial
    tick_t duration;	// Duración del retardo
    bool_t running;		// Estado del retardo
} delay_t;

/*******************************************************************
 * @brief  Indica si el retardo se encuentra en ejecución
 * @param  delay: Puntero a la estructura delay_t
 * @retval true  si el retardo está corriendo
 * @retval false si el retardo no está corriendo o el puntero es NULL
 ******************************************************************/
bool_t delayIsRunning(delay_t *delay);


/*******************************************************************
 * @brief  Inicializa la estructura de retardo
 * @param  delay: Puntero a la estructira delay_t
 * @param  duration: Tiempo de retardo en ticks
 * @retval None
 ******************************************************************/
void delayInit(delay_t *delay, tick_t duration);


/*******************************************************************
 * @brief  Verifica si el tiempo de retardo ha terminado
 * @param  delay: Puntero a la estructura delay_t
 * @retval true si el retardo termino
 ******************************************************************/
bool_t delayRead(delay_t *delay);


/*******************************************************************
 * @brief  Actualiza el tiempo de retardo
 * @param  delay: Puntero a la estructura delay_t
 * @param  duration: Nuevo tiempo de retardo en ticks
 * @retval None
 ******************************************************************/
void delayWrite(delay_t *delay, tick_t duration);



#endif /* API_INC_API_DELAY_H_ */
