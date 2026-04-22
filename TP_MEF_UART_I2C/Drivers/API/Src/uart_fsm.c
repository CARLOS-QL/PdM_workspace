/*
 * uart_fsm.c
 *
 *  Created on: Apr 12, 2026
 *      Author: Carlos Quispe
 */

#include "uart_fsm.h"
#include "uart_drv.h"
#include "sensor_fsm.h"
#include "main.h"


#define UART_RX_BUFFER_SIZE   64U

typedef enum
{
    UART_STATE_IDLE = 0,
    UART_STATE_RECEIVING,
    UART_STATE_PROCESS,
    UART_STATE_ERROR
} uart_state_t;

static uart_state_t currentState = UART_STATE_IDLE;

static uint8_t rxByte = 0U;
static uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
static uint8_t rxIndex = 0U;

/* Funciones privadas */
static void uartResetBuffer(void);
static void uartProcessCommand(void);
static void uartSendHelp(void);
static void uartSendStatus(void);


void uartFsmInit(void)
{
    currentState = UART_STATE_IDLE;
    rxByte = 0U;
    rxIndex = 0U;
    uartResetBuffer();
}


void uartFsmUpdate(void)
{
	switch (currentState) {
		case UART_STATE_IDLE:

		//	if(uartReceiveStringSize(&dataRx, 1U) == UART_OK)
		//	{

		//	}


			break;
		default:
			break;
}

}

static void uartResetBuffer(void)
{
    (void)memset(rxBuffer, 0, sizeof(rxBuffer));
    rxIndex = 0U;
}

static void uartProcessCommand(void)
{
    char msg[80];
    float value = 0.0f;

    /* SET SP <valor> */
    if (strncmp((char *)rxBuffer, "SET SP ", 7) == 0)
    {
        value = (float)atof((char *)&rxBuffer[7]);
        sensorSetSetpoint(value);
        snprintf(msg, sizeof(msg), "OK: SP=%.2f\r\n", sensorGetSetpoint());
        uartSendString((uint8_t *)msg);
    }
    /* SET HYST <valor> */
    else if (strncmp((char *)rxBuffer, "SET HYST ", 9) == 0)
    {
        value = (float)atof((char *)&rxBuffer[9]);
        sensorSetHysteresis(value);
        snprintf(msg, sizeof(msg), "OK: HYST=%.2f\r\n", sensorGetHysteresis());
        uartSendString((uint8_t *)msg);
    }
    /* GET SP */
    else if (strcmp((char *)rxBuffer, "GET SP") == 0)
    {
        snprintf(msg, sizeof(msg), "SP=%.2f\r\n", sensorGetSetpoint());
        uartSendString((uint8_t *)msg);
    }
    /* GET HYST */
    else if (strcmp((char *)rxBuffer, "GET HYST") == 0)
    {
        snprintf(msg, sizeof(msg), "HYST=%.2f\r\n", sensorGetHysteresis());
        uartSendString((uint8_t *)msg);
    }
    /* STATUS */
    else if (strcmp((char *)rxBuffer, "STATUS") == 0)
    {
        uartSendStatus();
    }
    /* HELP */
    else if (strcmp((char *)rxBuffer, "HELP") == 0)
    {
        uartSendHelp();
    }
    else
    {
        uartSendString((uint8_t *)"ERROR: comando invalido\r\n");
    }
}

static void uartSendHelp(void)
{
    uartSendString((uint8_t *)
        "Comandos disponibles:\r\n"
        "SET SP <valor>\r\n"
        "SET HYST <valor>\r\n"
        "GET SP\r\n"
        "GET HYST\r\n"
        "STATUS\r\n"
        "HELP\r\n");
}

static void uartSendStatus(void)
{
    char msg[96];

    snprintf(msg, sizeof(msg),
             "SP=%.2f HYST=%.2f\r\n",
             sensorGetSetpoint(),
             sensorGetHysteresis());

    uartSendString((uint8_t *)msg);
}


