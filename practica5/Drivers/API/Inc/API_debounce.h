/*
 * API_debounce.h
 *
 *  Created on: Mar 26, 2026
 *      Author: CARLOS
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_


#include "stdint.h"
#include "stdbool.h"

typedef bool bool_t;

/******** Prototipo de funciones públicas *********/

/**
 * @brief  Inicializa la máquina de estados de anti-rebote.
 * @param  None
 * @retval None
 */
void debounceFSM_init(void);

/**
 * @brief  Actualiza la lógica de la máquina de estados.
 * @param  None
 * @retval None
 */
void debounceFSM_update(void);


/**
 * @brief  Verifica si se ha detectado una pulsación válida.
 * @param  None
 * @retval bool_t: true si la tecla fue presionada, false en caso contrario.
 */
bool_t readKey(void);


#endif /* API_INC_API_DEBOUNCE_H_ */
