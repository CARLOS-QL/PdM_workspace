/*
 * API_uart.h
 *
 *  Created on: Apr 3, 2026
 *      Author: CARLOSQL
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//#include "main.h"
#include "stm32f4xx_hal.h"

#define UART_MAX_SIZE	256U
#define	UART_TIMEOUT_MS	1000U

typedef bool bool_t;


bool_t uartInit(uint32_t baudRate);
bool_t uartSendString(uint8_t * pstring);
bool_t uartSendStringSize(uint8_t * pstring, uint16_t size);
bool_t uartReceiveStringSize(uint8_t * pstring, uint16_t size);


#endif /* API_INC_API_UART_H_ */
