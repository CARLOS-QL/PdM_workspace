/*
 * uart_fsm.h
 *
 *  Created on: Apr 12, 2026
 *      Author: Carlos Quispe
 */

#ifndef API_INC_UART_FSM_H_
#define API_INC_UART_FSM_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Inicializa la máquina de estados de recepción UART.
 */
void uartFsmInit(void);

/**
 * @brief Ejecuta la máquina de estados de recepción UART.
 */
void uartFsmUpdate(void);

#endif /* API_INC_UART_FSM_H_ */
