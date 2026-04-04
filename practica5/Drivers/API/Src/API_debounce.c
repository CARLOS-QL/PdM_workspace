/*
 * API_debounce.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Carlos Quispe
 */


#include "API_debounce.h"
#include "API_delay.h"
#include "main.h"


/******** Tiempo de antirebote en ms **********/
#define DEBOUNCE_TIME  40U

/******** Estados privados de la MEF **********/
typedef enum{
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RAISING,
} debounceState_t;

/******* Variables privadas del módulo ********/

static debounceState_t debounceState;		//Variable static
static delay_t debounceDelay;
static bool_t keyPressed = false;

/* Prototipos privados */
static bool_t buttonStateRead(void);
static void buttonPressed(void);
static void buttonReleased(void);

/**
 * @brief  Inicializa la Máquina de Estados Finitos.
 * @details Carga el estado inicial BUTTON_UP, resetea la bandera de tecla
 * Inicializa el retardo no bloqueante
 * @param  None
 * @retval None
 */

void debounceFSM_init(void)
{
    debounceState = BUTTON_UP;
    delayInit(&debounceDelay, DEBOUNCE_TIME);
    keyPressed = false;
}

/**
 * @brief  Actualiza la lógica de la MEF anti-rebote.
 * @details Lee las entradas, resuelve las transiciones de estado según el
 * tiempo de 40ms y actualiza las salidas o eventos (Pressed/Released)
 * Debe llamarse periódicamente en el bucle principal.
 * @param  None
 * @retval None
 */
void debounceFSM_update(void)
{
    switch (debounceState) {

    case BUTTON_UP:
        if (buttonStateRead() == false) {
            debounceState = BUTTON_FALLING;
            delayWrite(&debounceDelay, DEBOUNCE_TIME);
        }
        break;

    case BUTTON_FALLING:
        if (delayRead(&debounceDelay)) {
            if (buttonStateRead() == false) {
                debounceState = BUTTON_DOWN;
                buttonPressed();
            } else {
                debounceState = BUTTON_UP;
            }
        }
        break;

    case BUTTON_DOWN:
        if (buttonStateRead() == true) {
            debounceState = BUTTON_RAISING;
            delayWrite(&debounceDelay, DEBOUNCE_TIME);
        }
        break;

    case BUTTON_RAISING:
        if (delayRead(&debounceDelay)) {
            if (buttonStateRead() == true) {
                debounceState = BUTTON_UP;
                buttonReleased();
            } else {
                debounceState = BUTTON_DOWN;
            }
        }
        break;

    default:
        debounceFSM_init();
        break;
    }
}


/**
 * @brief  Verifica si el pulsador fue presionado.
 * @details Lee la variable interna 'keyPressed'. Si es verdadera, la resetea
 * a falso automáticamente antes de retornar
 * @param  None
 * @retval bool_t: true si la tecla fue presionada, false en caso contrario[cite: 28].
 */
bool_t readKey(void)
{
    bool_t ret = false;

    if (keyPressed == true) {
        ret = true;
        keyPressed = false;
    }

    return ret;
}

/**
 * @brief  Función privada para leer el estado físico del pin del pulsador.
 * @note   Encapsula la dependencia con la biblioteca HAL.
 * @param  None
 * @retval bool_t: true si el botón está en nivel alto (suelto),
 * false si está en nivel bajo (presionado).
 */

static bool_t buttonStateRead(void)
{
    return (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET);
}

/**
 * @brief  Acción a ejecutar cuando se confirma una pulsación válida.
 * @details Activa la bandera interna que será leída por readKey()[cite: 11, 31].
 * @param  None
 * @retval None
 */

static void buttonPressed(void)
{
    keyPressed = true;
}

/**
 * @brief  Acción a ejecutar cuando se confirma que el pulsador se soltó.
 * @param  None
 * @retval None
 */
static void buttonReleased(void)
{

}
