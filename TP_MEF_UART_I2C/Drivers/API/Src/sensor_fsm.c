/*
 * sensor_fsm.c
 *
 *  Created on: Apr 12, 2026
 *      Author: Carlos Quispe
 */


#include "sensor_fsm.h"
#include "mcp9700a.h"
#include "uart_drv.h"
#include "main.h"

#define SENSOR_PERIOD_MS   500U

typedef enum
{
	STATE_IDLE = 0,
	STATE_READ,
	STATE_CONTROL,
	STATE_OUTPUT,
	STATE_ERROR
} sensor_state_t;

static sensor_state_t currentState = STATE_IDLE;

static uint32_t lastTick = 0;

static float temperature = 0.0f;
static float setpoint = 30.0f;
static float hysteresis = 1.0f;

static uint8_t outputState = 0;


/**
 * @brief Inicializa la máquina de estados del sensor.
 * Configura el estado inicial del sistema en IDLE y almacena
 * el tiempo actual como referencia para la ejecución periódica
 * de la lectura del sensor.
 */
void sensorFsmInit(void)
{
	currentState = STATE_IDLE;
	lastTick = HAL_GetTick();
}

/**
 * @brief Configura el setpoint del sistema.
 * Establece la temperatura objetivo utilizada por el control ON/OFF.
 * @param sp Valor del setpoint en grados Celsius.
 */
void sensorSetSetpoint(float sp)
{
	setpoint = sp;
}

/**
 * @brief Configura la histéresis del sistema.
 * Define el margen alrededor del setpoint para evitar oscilaciones
 * @param hyst Valor de histéresis en grados Celsius.
 */
void sensorSetHysteresis(float hyst)
{
	hysteresis = hyst;
}

/**
 * @brief Obtiene el valor actual del setpoint.
 * @return Valor del setpoint en grados Celsius.
 */
float sensorGetSetpoint(void)
{
    return setpoint;
}

/**
 * @brief Obtiene el valor actual de la histéresis.
 * @return Valor de la histéresis en grados Celsius.
 */
float sensorGetHysteresis(void)
{
    return hysteresis;
}


/**
 * @brief Ejecuta la máquina de estados del sensor.
 * Implementa un sistema no bloqueante que realiza:
 * - Lectura periódica de la temperatura
 * - Evaluación de control ON/OFF con histéresis
 * - Actualización de la salida (LED/relé)
 * - Envío de datos por UART
 */

void sensorFsmUpdate(void)
{
	switch (currentState)
	{
	case STATE_IDLE:

		if ((HAL_GetTick() - lastTick) >= SENSOR_PERIOD_MS)
		{
			lastTick = HAL_GetTick();
			currentState = STATE_READ;
		}
		break;

	case STATE_READ:

		if (mcp9700aReadTemperature(&temperature) != MCP9700A_OK)
		{
			currentState = STATE_ERROR;
		}
		else
		{
			currentState = STATE_CONTROL;
		}
		break;

	case STATE_CONTROL:

		if (temperature >= (setpoint + hysteresis))
		{
			outputState = 1;
		}
		else if (temperature <= (setpoint - hysteresis))
		{
			outputState = 0;
		}

		currentState = STATE_OUTPUT;
		break;

	case STATE_OUTPUT:

		char msg[64];
		sprintf(msg, "Temp: %.2f C\r\n", temperature);
		uartSendString((uint8_t*) msg);


		if (outputState)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);   // LED ON
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // LED OFF
		}

		currentState = STATE_IDLE;
		break;

	case STATE_ERROR:

		/* Manejo simple: intentar nuevamente */
		currentState = STATE_IDLE;
		break;

	default:
		currentState = STATE_IDLE;
		break;
	}
}




