/*
 * adc_drv.c
 *
 *  Created on: Apr 12, 2026
 *      Author: Carlos Quispe
 */

#include "adc_drv.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

/**
 * @brief Ejecuta una conversión ADC en modo polling.
 * @details
 * - Inicia la conversión ADC
 * - Espera a que finalice (polling)
 * - Lee el valor convertido
 * - Detiene la conversión
 */

adc_status_t adcInit(void)
{
	ADC_ChannelConfTypeDef sConfig =
	{ 0 };

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		return ADC_ERROR_INIT;
	}

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. **/

	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		return ADC_ERROR_CONFIG;
	}
	return ADC_OK;
}

/**
 * @brief Convierte la lectura ADC a voltaje.
 * @details
 * El valor digital se escala utilizando la resolución del ADC
 * (12 bits) y el voltaje de referencia del sistema.
 */

adc_status_t adcReadRaw(uint32_t *rawValue)
{
	if (rawValue == NULL)					// Validar el puntero de salida
	{
		return ADC_ERROR_NULL_POINTER;
	}

	if (HAL_ADC_Start(&hadc1) != HAL_OK)	// iniciar conversión del ADC
	{
		return ADC_ERROR_START;
	}

	if (HAL_ADC_PollForConversion(&hadc1, ADC_TIMEOUT_MS) != HAL_OK)
	{
		(void) HAL_ADC_Stop(&hadc1);			// Detiene la conversión
		return ADC_ERROR_TIMEOUT;
	}

	*rawValue = HAL_ADC_GetValue(&hadc1);	// Se obtiene el valor convertido
	if (HAL_ADC_Stop(&hadc1) != HAL_OK)		// Detiene la conversión
	{
		return ADC_ERROR_STOP;
	}

	return ADC_OK;							// Conversión exitosa
}



