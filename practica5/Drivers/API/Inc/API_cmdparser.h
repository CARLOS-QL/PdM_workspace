/*
 * API_cmdparser.h
 *
 *  Created on: Apr 4, 2026
 *      Author: Carlos Quispe
 */

#ifndef API_INC_API_CMDPARSER_H_
#define API_INC_API_CMDPARSER_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "API_uart.h"
#include "API_led.h"
#include "main.h"

#define CMD_MAX_LINE     16    // incluye '\0'
#define CMD_MAX_TOKENS   3     // COMANDO + máximo 2 argumentos

/**
 * @brief Estados de resultado del procesamiento de comandos.
 * Este tipo enum define los posibles resultados luego de analizar
 * y procesar un comando recibido por la interfaz (por ejemplo UART).
 */

typedef enum {
    CMD_OK = 0,
    CMD_ERR_OVERFLOW,
    CMD_ERR_SYNTAX,
    CMD_ERR_UNKNOWN,
    CMD_ERR_ARG
} cmd_status_t;

/**
 * @brief Tipos de comandos soportados por el sistema.
 * Este enum representa los comandos que pueden ser interpretados
 * por el parser, generalmente recibidos desde una interfaz de comunicación.
 */

typedef enum {
    CMD_LED_NONE = 0,
	CMD_LED_ON,
    CMD_LED_OFF,
    CMD_LED_TOGGLE
} cmd_led_t;


/**
 * @brief Inicializa el módulo parser de comandos
 */
void cmdParserInit(void);

/**
 * @brief Máquina de estados del parser. Debe ser llamada periódicamente desde el bucle
 *        Procesa hasta 16 bytes por invocación (no bloqueante).
 */
void cmdPoll(void);

/**
 * @brief Imprime por UART la lista de comandos disponibles
 */
void cmdPrintHelp(void);


#endif /* API_INC_API_CMDPARSER_H_ */
