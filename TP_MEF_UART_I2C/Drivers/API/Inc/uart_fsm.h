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

#define CMD_MAX_LINE     16    // incluye '\0'


/**
 * @brief Estados de resultado del procesamiento de comandos.
 * Este tipo enum define los posibles resultados luego de analizar
 * y procesar un comando recibido por la interfaz (por ejemplo UART).
 */

typedef enum {
    UART_OK = 0,
    UART_ERR_OVERFLOW,
    UART_ERR_SYNTAX,
    UART_ERR_UNKNOWN,
    UART_ERR_ARG
} uart_error_t;
/**
 * @brief Inicializa la máquina de estados de recepción UART.
 */
void uartFsmInit(void);

/**
 * @brief Ejecuta la máquina de estados de recepción UART.
 */
void uartFsmUpdate(void);

#endif /* API_INC_UART_FSM_H_ */
