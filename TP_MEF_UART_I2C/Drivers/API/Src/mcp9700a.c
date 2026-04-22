/*
 * mcp9700a_drv.c
 *
 *  Created on: Apr 12, 2026
 *      Author: Carlos Quispe
 */


#include <mcp9700a.h>
#include "main.h"


/* Constantes del sensor MCP9700A */
static const float adcMaxValue = 4095.0f;
static const float adcVref = 3.3f;

static const float mcp9700aVoffset = 0.5f;
static const float mcp9700aTc = 0.01f;



mcp9700a_status_t mcp9700aReadTemperature(float *temperature)
{
	uint32_t rawValue = 0U;
	float voltage = 0.0f;

	if (temperature == NULL)
	{
		return MCP9700A_ERROR_NULL_POINTER;

	}

	if (adcReadRaw(&rawValue) != ADC_OK)
	{
		return MCP9700A_ERROR_ADC;
	}

	voltage = ((float) rawValue * adcVref) / adcMaxValue;
	*temperature = (voltage - mcp9700aVoffset) / mcp9700aTc;

	return MCP9700A_OK;
}

