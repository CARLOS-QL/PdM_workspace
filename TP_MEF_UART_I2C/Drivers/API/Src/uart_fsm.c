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


#define UART_RX_BUFFER_SIZE   			64U
#define UART_TX_MESSAGE_BUFFER_SIZE   	96U


typedef enum
{
    UART_STATE_IDLE = 0,
    UART_STATE_RECEIVING,
    UART_STATE_PROCESS,
    UART_STATE_ERROR
} uart_state_t;

static uart_state_t currentState;
static uart_error_t statusErrorUart;						// Variable del tipo de Error




static uint8_t rxByte = 0U;
static uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
static uint8_t rxIndex = 0U;
static bool_t isCommandPending = false;		// Bandera para comando PROCESS, EXEC, ERROR del MEF

/* Funciones privadas */
static void uartResetBuffer(void);
static void uartProcessCommand(void);
static void uartSendHelp(void);
static void uartSendStatus(void);
static void uartError(uart_error_t errorType);


void uartFsmInit(void)
{
    currentState = UART_STATE_IDLE;
    statusErrorUart = UART_OK;
    rxByte = 0U;
    rxIndex = 0U;
    uartResetBuffer();
}


void uartFsmUpdate(void)
{
	uint8_t charReceived = uartReceiveStringSize(&rxByte, 1U);

	if (!charReceived && !isCommandPending) {	// Sale de funcion si no datos Rx y badera falso
		return;
	}
	//uartSendStringSize(&rxByte, 1U);				// Eco de caracteresa

	switch (currentState) {
		case UART_STATE_IDLE:

			// Ignorar múltiples espacios, tabs al inicio o #
			if (rxByte == ' ' || rxByte == '\t') {
				break;				// Para continuar en IDLE
			}
			// Ignorar las las líneas que empicen con #
			if (rxByte == '#' || rxByte == '/') {
				break;				// Para continuar en IDLE
			}
			// Si el dato es diferente de
			if (rxByte != 'r' && rxByte != '\n') {
				rxBuffer[rxIndex++] = rxByte;	//Recibe el primer carcter
				currentState = UART_STATE_RECEIVING;
			}

			break;

		case UART_STATE_RECEIVING:

			if (rxByte == '\r' || rxByte == '\n') {		// Si llega ENTER cambia de estado
				rxBuffer[rxIndex++] = '\0';				// Se agrega final de trama
				isCommandPending = true;				// Activamos bandera para ir alos otros estados
				currentState = UART_STATE_PROCESS;		// Trama lista para ser procesada

			} else {
				if (rxIndex < (CMD_MAX_LINE -1)) {
					rxBuffer[rxIndex++] = rxByte;
				} else {
					isCommandPending = true;				// Activamos bandera para ir alos otros estados
					currentState = UART_STATE_ERROR;		// Si hay error cambiamos de estado Error
					statusErrorUart = UART_ERR_OVERFLOW;	// Indicamos error por desboramiento
				}
			}

			break;

		case UART_STATE_PROCESS:

			//uartSendString((uint8_t *)rxBuffer);
			uartProcessCommand();
			if (statusErrorUart) {
				currentState = UART_STATE_ERROR;			// Si hay error cambiamos de estado Error
			} else {
				statusErrorUart = UART_OK;					// No hubo error
				rxIndex = 0;								// Contador reseteado
				rxBuffer[0] = '\0';							// Limpiar Buffer
				isCommandPending = false;					// Limpiamos bandera
				currentState = UART_STATE_IDLE;				// Volver al estado IDLE
				//state = CMD_EXEC;							// Estado ejecutar
			}
			//currentState = UART_STATE_IDLE;
			break;

		case UART_STATE_ERROR:
			uartSendString((uint8_t *)"ERROR");
			uartError(statusErrorUart);			// Mandamos mensaje dependiendo del error
			statusErrorUart = UART_OK;			// Limpia Error, estado inicial
			isCommandPending = false;			// Limpiamos bandera
			rxIndex = 0;						// contador reseteado
			rxBuffer[0] = '\0';					// Limpiar Buffer
			currentState = UART_STATE_IDLE;		// Estado inicial
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

    char uartTxBuffer[UART_TX_MESSAGE_BUFFER_SIZE];
    float value = 0.0f;

    /* SET SP <valor> */
    if (strncmp((char *)rxBuffer, "SET SP=", 7) == 0)
    {
        value = (float)atof((char *)&rxBuffer[7]);
        sensorSetSetpoint(value);
        snprintf(uartTxBuffer, sizeof(uartTxBuffer), "OK: SP=%.2f\r\n", sensorGetSetpoint());
        uartSendString((uint8_t *)uartTxBuffer);
    }
    /* SET HYST <valor> */
    else if (strncmp((char *)rxBuffer, "SET HYST=", 9) == 0)
    {
        value = (float)atof((char *)&rxBuffer[9]);
        sensorSetHysteresis(value);
        snprintf(uartTxBuffer, sizeof(uartTxBuffer), "OK: HYST=%.2f\r\n", sensorGetHysteresis());
        uartSendString((uint8_t *)uartTxBuffer);
    }
    /* GET SP */
    else if (strcmp((char *)rxBuffer, "GET SP") == 0)
    {
        snprintf(uartTxBuffer, sizeof(uartTxBuffer), "SP=%.2f\r\n", sensorGetSetpoint());
        uartSendString((uint8_t *)uartTxBuffer);
    }
    /* GET HYST */
    else if (strcmp((char *)rxBuffer, "GET HYST") == 0)
    {
        snprintf(uartTxBuffer, sizeof(uartTxBuffer), "HYST=%.2f\r\n", sensorGetHysteresis());
        uartSendString((uint8_t *)uartTxBuffer);
    }
    /* STATUS */
    else if (strcmp((char *)rxBuffer, "GET TEMP") == 0)
    {
    	snprintf(uartTxBuffer, sizeof(uartTxBuffer), "TEMP=%.2f\r\n", sensorGetTemperature());
    	uartSendString((uint8_t *)uartTxBuffer);
    }
    /* STATUS */
    else if (strcmp((char *)rxBuffer, "STATUS") == 0)
    {
	   uartSendStatus();
    }
    /* HELP */
    else if (strncmp((char *)rxBuffer, "HELP", 4) == 0)
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
    char uartTxBuffer[UART_TX_MESSAGE_BUFFER_SIZE];

    snprintf(uartTxBuffer, sizeof(uartTxBuffer),
             "SP=%.2f HYST=%.2f\r\n",
             sensorGetSetpoint(),
             sensorGetHysteresis());

    uartSendString((uint8_t *)uartTxBuffer);
}



/*********************************************************************************
 * @brief  Envía el mensaje de error correspondiente por UART.
 * @note   Esta función centraliza todas las salidas del protocolo para asegurar
 * que todas las respuestas terminen en \r\n según el requerimiento.
 * @param  errorType: Código del error o estado a informar (tipo cmd_status_t).
 * @retval Ninguno
 *********************************************************************************/

static void uartError(uart_error_t errorType){

	switch (errorType) {
		case UART_OK:
			// Comando ejecutado con éxito
			uartSendString((uint8_t *)"OK\r\n");
			break;

		case UART_ERR_OVERFLOW:
			// Cuando el dataIndex supera UART_MAX_SIZE
			uartSendString((uint8_t *)"ERROR: Exedio cantidad de caracteres\r\n");
			break;

		case UART_ERR_SYNTAX:
			// Error en la formación de la trama, caracteres no imprimibles
			uartSendString((uint8_t *)"ERROR: Error de sitaxis\r\n");
			break;

		case UART_ERR_UNKNOWN:
			// Comando recibido no coincide con ninguan en el parser
			char msgError[64];
			//sprintf(msgError, "Error: '%s' is unknown command\r\n", (char *)dataBuffer);
			uartSendString((uint8_t *)msgError);
			break;

		case UART_ERR_ARG:
			uartSendString((uint8_t *)"ERROR: Argumento Invalido\r\n");
			break;
		default:
			// Algun error no conocido
			uartSendString((uint8_t *)"ERROR: Desconocido\r\n");
			break;
	}
}


