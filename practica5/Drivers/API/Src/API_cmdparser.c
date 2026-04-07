/*
 * API_cmdparser.c
 *
 *  Created on: Apr 4, 2026
 *      Author: Carlos Quispe
 */


#include <API_cmdparser.h>

/******** Estados privados de la MEF **********/
typedef enum{
    CMD_IDLE,
    CMD_RECEIVING,
    CMD_PROCESS,
	CMD_EXEC,
    CMD_ERROR,
} cmd_state_t;

/*------ Prototipo de variables privadas ---------*/

static cmd_led_t stateLed;					// Tipo de ejecución del led
static cmd_state_t state;					// Variable del MDF del parser
static cmd_status_t statusError;			// Variable del tipo de Error
static uint8_t dataRx;						// Byte de recepción de UART
static uint8_t dataIndex;					// Indice para recepción de trama
static uint8_t dataBuffer[CMD_MAX_LINE];	// Buffer de recepción de trama
static bool_t isCommandPending = false;		// Bandera para comando PROCESS, EXEC, ERROR del MEF

uint32_t newBaud;
/*------ Prototipo de funciones privadas ---------*/

static void cmdError(cmd_status_t errorType);
static void cmdProcessLine(void);
static void ledExc(void);


/****************************************************
 * @brief Inicializa el módulo parser de comandos
 ***************************************************/

void cmdParserInit(void) {
	state = CMD_IDLE;			// Valor por defecto MDF
	statusError = CMD_OK;		// Valor por defecto OK
	stateLed = CMD_LED_NONE;	// Valor por defecto NONE
	dataRx = 0;					// Inicializa en 0
	dataIndex = 0;				// Contador de caracteres en 0
}

/*************************************************************************************
 * @brief  Analizador de comandos (Parser) y ejecutor de acciones.
 * - Es de tipo 'static' para mantener el encapsulamiento del módulo.
 * - Utiliza 'strncmp' para una comparación segura limitando la cantidad de caracteres.
 * - Gestiona el encendido, apagado y toggle del LED de la placa.
 * - Permite consultar el estado actual del LED y la configuración de velocidad (Baudrate).
 * - Si el comando no coincide con ninguno de la lista, activa el estado de error.
 * @param  None
 * @retval None
 *************************************************************************************/

static void cmdProcessLine(void) {

	if (strncmp((char *)dataBuffer, "HELP", 4) == 0) {

		cmdPrintHelp();				// Imprimir lista de comandos

	} else if (strncmp((char *)dataBuffer, "LED ON", 6) == 0) {

		stateLed = CMD_LED_ON;		// Para activar led en CMD_EXEC

	} else if (strncmp((char *)dataBuffer, "LED OFF", 7) == 0) {

		stateLed = CMD_LED_OFF;		// Para apagar led en CMD_EXEC

	} else if (strncmp((char *)dataBuffer, "LED TOGGLE", 10) == 0) {

		stateLed = CMD_LED_TOGGLE;	// Para toggle led en CMD_EXEC

	} else if (strncmp((char *)dataBuffer, "STATUS", 6) == 0) {

		bool_t statusLed= led_status();
		if (statusLed) {
			uartSendString((uint8_t *)"LED encendido\r\n");
		}else{
			uartSendString((uint8_t *)"LED apagado\r\n");
		}

	} else if (strncmp((char *)dataBuffer, "BAUD?", 5) == 0) {

		uint32_t baud = huart2.Init.BaudRate;
		char msgBaud[20];
		sprintf(msgBaud, "Baudrate: %lu\r\n", baud);
		uartSendString((uint8_t *)msgBaud);

	} else if (strncmp((char *)dataBuffer, "BAUD=", 5) == 0) {
		newBaud = (uint32_t) atoi((char *)dataBuffer + 5);
		if (newBaud >= 9600 && newBaud <= 921600) {

		}else {
			char valorErroneo[32];
			strncpy(valorErroneo, (char *)(dataBuffer + 5), sizeof(valorErroneo) - 1);
			valorErroneo[sizeof(valorErroneo) - 1] = '\0';
			char msg[100];
			sprintf(msg, "ERROR: '%s' no es un baudrate valido\r\n", valorErroneo);
			uartSendString((uint8_t *)msg);
		}

	} else {
		statusError = CMD_ERR_UNKNOWN;
	}
}

/**************************************************************************************
 * @brief Máquina de estados del parser. Debe ser llamada periódicamente desde el bucle
 *        Procesa hasta MAX_LINE bytes por invocación (no bloqueante).
 *************************************************************************************/

void cmdPoll(void) {

	bool_t charReceived = uartReceiveStringSize(&dataRx, 1U);

	if (!charReceived && !isCommandPending) {	// Sale de funcion si no datos Rx y badera falso
		return;
	}

	uartSendStringSize(&dataRx, 1);				// Eco de caracteresa

	switch (state) {
		case CMD_IDLE:

			// Ignorar múltiples espacios, tabs al inicio o #
			if (dataRx == ' ' || dataRx == '\t') {
				break;				// Para continuar en IDLE
			}
			// Ignorar las las líneas que empicen con #
			if (dataRx == '#' || dataRx == '/') {
				break;				// Para continuar en IDLE
			}
			// Si el dato es diferente de
			if (dataRx != 'r' && dataRx != '\n') {
				dataBuffer[dataIndex++] = dataRx;	//Recibe el primer carcter
				state = CMD_RECEIVING;
			}
			break;

		case CMD_RECEIVING:

			if (dataRx == '\r' || dataRx == '\n') {	// Si llega ENTER cambia de estado
				dataBuffer[dataIndex++] = '\0';		// Se agrega final de trama
				isCommandPending = true;			// Activamos bandera para ir alos otros estados
				state = CMD_PROCESS;				// Trama lista para ser procesada

			} else {
				if (dataIndex < (CMD_MAX_LINE -1)) {
					dataBuffer[dataIndex++] = dataRx;
				} else {
					isCommandPending = true;			// Activamos bandera para ir alos otros estados
					state = CMD_ERROR;					// Si hay error cambiamos de estado Error
					statusError = CMD_ERR_OVERFLOW;		// Indicamos error por desboramiento
				}
			}
			break;

		case CMD_PROCESS:

			cmdProcessLine();				// Procesa la trama
			if (statusError) {
				state = CMD_ERROR;			// Si hay error cambiamos de estado Error
			} else {
				statusError = CMD_OK;		// No hubo error
				dataIndex = 0;				// Contador reseteado
				dataBuffer[0] = '\0';		// Limpiar Buffer
				state = CMD_EXEC;			// Estado ejecutar
			}
			break;

		case CMD_EXEC:

			ledExc();					// Ejecutar comandos de led
			isCommandPending = false;	// Limpiamos bandera
			dataIndex = 0;				// Contador reseteado
			dataBuffer[0] = '\0';		// Limpiar Buffer
			state = CMD_IDLE;			// Estado inicial
			break;

		case CMD_ERROR:
			cmdError(statusError);			// Mandamos mensaje dependiendo del error
			statusError = CMD_OK;			// Limpia Error, estado inicial
			isCommandPending = false;		// Limpiamos bandera
			dataIndex = 0;					// contador reseteado
			dataBuffer[0] = '\0';			// Limpiar Buffer
			state = CMD_IDLE;				// Estado inicial
			break;

		default:

			break;
	}
}

/*************************************************************
 * @brief Imprime por UART la lista de comandos disponibles
 ************************************************************/

void cmdPrintHelp(void) {

	uartSendString((uint8_t *)"Comandos Aceptador por el Programa\r\n");
	uartSendString((uint8_t *)"* HELP 		-> Para visualizar comandos válidos\r\n");
	uartSendString((uint8_t *)"* LED ON 	-> Para encedner LED verde\r\n");
	uartSendString((uint8_t *)"* LED OFF 	-> Para apagar LED verde\r\n");
	uartSendString((uint8_t *)"* LED TOGGLE	-> Para cambiar el estado de LED verde\r\n");
	uartSendString((uint8_t *)"* STATUS 	-> Para devolver el estado del LED\r\n");
	uartSendString((uint8_t *)"* BAUD? 		-> Para visualizar el Baudrate actual\r\n");
}

/*********************************************************************************
 * @brief  Envía el mensaje de error correspondiente por UART.
 * @note   Esta función centraliza todas las salidas del protocolo para asegurar
 * que todas las respuestas terminen en \r\n según el requerimiento.
 * @param  errorType: Código del error o estado a informar (tipo cmd_status_t).
 * @retval Ninguno
 *********************************************************************************/

static void cmdError(cmd_status_t errorType){

	switch (errorType) {
		case CMD_OK:
			// Comando ejecutado con éxito
			uartSendString((uint8_t *)"OK\r\n");
			break;

		case CMD_ERR_OVERFLOW:
			// Cuando el dataIndex supera UART_MAX_SIZE
			uartSendString((uint8_t *)"ERROR: line too long\r\n");
			break;

		case CMD_ERR_SYNTAX:
			// Error en la formación de la trama, caracteres no imprimibles
			uartSendString((uint8_t *)"ERROR: syntax error\r\n");
			break;

		case CMD_ERR_UNKNOWN:
			// Comando recibido no coincide con ninguan en el parser
			char msgError[64];
			sprintf(msgError, "Error: '%s' is unknown command\r\n", (char *)dataBuffer);
			uartSendString((uint8_t *)msgError);
			break;

		case CMD_ERR_ARG:
			uartSendString((uint8_t *)"ERROR: unknown command\r\n");
			break;
		default:
			// Algun error no conocido
			uartSendString((uint8_t *)"ERROR: unknown\r\n");
			break;
	}
}

/*********************************************************************************
 * @brief  Ejecuta el comando sobre el led
 * @note   ON, OFF, TOGGLE del led
 * @retval Ninguno
 *********************************************************************************/

static void ledExc(void){

	switch (stateLed) {
		case CMD_LED_ON: led_on(); break;
		case CMD_LED_OFF: led_off(); break;
		case CMD_LED_TOGGLE: led_toggle(); break;
		default:
			break;
	}
	stateLed = CMD_LED_NONE;
}
