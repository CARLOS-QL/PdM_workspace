/*
 * API_delay.c
 *
 *  Created on: Mar 22, 2026
 *      Author: CARLOSQL
 */

#include "API_delay.h"


/**
 * @brief Inicializa la estructura de retardo en estado detenido.
 */
void delayInit(delay_t *delay, tick_t duration)
{
    if (delay == NULL) {
        return;
    }

    delay->duration = duration;
    delay->running  = false;
    delay->startTime = 0;
}

/**
 * @brief Devuelve el estado de ejecución del retardo
 */
bool_t delayIsRunning(delay_t *delay)
{
    bool_t ret = false;

    if (delay != NULL)
    {
    	/* Se retorna una copia del estado interno para mantener encapsulación */
        ret = delay->running;
    }

    return ret;
}

/**
 * @brief Evalúa el retardo no bloqueante y devuelve true cuando vence.
 */

bool_t delayRead(delay_t *delay)
{
	bool_t ret = false;

    if (delay != NULL)
    {
    	if (delay->running == false)
    	{
    		delay->startTime = HAL_GetTick();
    		delay->running = true;
    	}
    	else
    	{
    		if ((HAL_GetTick() - delay->startTime) >= delay->duration)
    		{
    			delay->running = false;
    			ret = true;
    		}
    	}
    }
    return ret;
}

/**
 * @brief Actualiza la duración del retardo.
 */
void delayWrite(delay_t *delay, tick_t duration)
{
    if (delay == NULL) {
        return;
    }

    delay->duration = duration;
    delay->running = false;
    delay->startTime = 0;
}
