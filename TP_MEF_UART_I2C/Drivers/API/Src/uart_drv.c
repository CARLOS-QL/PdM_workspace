/*
 * uart_drv.c
 *
 *  Created on: Apr 12, 2026
 *      Author: Carlos Quispe
 */

#include "uart_drv.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

bool_t uartInit(uint32_t baudRate)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = baudRate;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		return false;
	}
	return true;
}

/**
 * @brief Envía una cadena de caracteres completa por UART
 * @param pstring: Puntero al string (cadena terminada en '\0')
 */

bool_t uartSendString(uint8_t *pstring)
{
	if (pstring == NULL)
	{
		return false;
	}

	uint16_t size = (uint16_t) strlen((char*) pstring);
	if (size == 0U || size > UART_MAX_SIZE)
	{
		return false;
	}

	if (HAL_UART_Transmit(&huart2, pstring, size, TX_TIMEOUT_MS) != HAL_OK)
	{
		return false;
	}
	return true;
}

/**
 * @brief Envía una cantidad específica de bytes por UART.
 * @param pstring Puntero al buffer de datos.
 * @param size Cantidad de bytes a transmitir.
 * @return true  Si la transmisión fue exitosa.
 * @return false Si hubo error (puntero NULL, tamaño inválido o fallo en HAL).
 */

bool_t uartSendStringSize(uint8_t *pstring, uint16_t size)
{
	if (pstring == NULL)
	{
		return false;
	}

	if (size == 0U || size > UART_MAX_SIZE)
	{
		return false;
	}

	if (HAL_UART_Transmit(&huart2, pstring, size, TX_TIMEOUT_MS) != HAL_OK)
	{
		return false;
	}
	return true;
}

/**
 * @brief Recibe una cantidad específica de bytes por UART.
 * @param pstring Puntero al buffer donde se almacenarán los datos recibidos.
 * @param size Cantidad de bytes a recibir.
 * @return true  Si la recepción fue exitosa.
 * @return false Si hubo error (puntero NULL, tamaño inválido o fallo en HAL).
 */

bool_t uartReceiveStringSize(uint8_t *pstring, uint16_t size)
{
	if (pstring == NULL)
	{
		return false;
	}

	if (size == 0U || size > UART_MAX_SIZE)
	{
		return false;
	}

	if (HAL_UART_Receive(&huart2, pstring, size, RX_TIMEOUT_MS) != HAL_OK)
	{
		return false;
	}
	return true;
}

