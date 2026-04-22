/*
 * mcp9700a_drv.h
 *
 *  Created on: Apr 12, 2026
 *      Author: CARLOSQL
 */

#ifndef API_INC_MCP9700A_H_
#define API_INC_MCP9700A_H_

#include "stdint.h"
#include "stdbool.h"
#include "adc_drv.h"

typedef enum
{
	MCP9700A_OK = 0, MCP9700A_ERROR_NULL_POINTER, MCP9700A_ERROR_ADC
} mcp9700a_status_t;


/**
 * @brief Lee la temperatura del sensor MCP9700A en grados Celsius.
 * @param temperatureC Puntero donde se almacenará la temperatura.
 * @return Estado de la operación.
 */
mcp9700a_status_t mcp9700aReadTemperature(float *temperature);

#endif /* API_INC_MCP9700A_H_ */
