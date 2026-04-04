# Práctica 4: Implementación de MEF Anti-rebote (Debounce)

## Autor: Carlos Quispe

## Descripción

Este proyecto implementa una **Máquina de Estados Finitos (MEF)** para eliminar el rebote mecánico de un pulsador mediante software, utilizando controladores (drivers) modulares y retardos no bloqueantes.



## 🎯 Objetivo
El objetivo es validar la pulsación del botón azul de la placa NUCLEO-F4 para cambiar la frecuencia de parpadeo de un LED entre **100 ms** y **500 ms**, garantizando que cada pulsación sea detectada una sola vez sin falsos positivos.

## 📂 Estructura del Módulo `API_debounce`
Siguiendo las buenas prácticas de encapsulamiento y modularización, el proyecto se organiza en:

* **`API_debounce.h`**: Contiene los prototipos de las funciones públicas (`init`, `update`, `readKey`).
* **`API_debounce.c`**: Contiene la implementación de la MEF, los estados privados (`BUTTON_UP`, `BUTTON_FALLING`, `BUTTON_DOWN`, `BUTTON_RAISING`) y las variables de estado protegidas con `static`.
* **`API_delay`**: Módulo desarrollado en la práctica anterior para la gestión de tiempos no bloqueantes utilizando `HAL_GetTick()`.

##  Funcionamiento de la MEF
La lógica de anti-rebote utiliza un tiempo de **40 ms** basado en el módulo `API_delay`.

1. **`debounceFSM_init()`**: Inicializa el sistema en el estado `BUTTON_UP` y resetea las banderas internas.
2. **`debounceFSM_update()`**: Se encarga de monitorear el pin del pulsador y transicionar entre estados para filtrar el ruido mecánico. Debe llamarse periódicamente en el bucle principal.
3. **`readKey()`**: Funciona como una interfaz para el programa principal. Devuelve `true` si se detectó una pulsación válida y resetea automáticamente la variable interna a `false`.

## 🛠️ Requisitos de Hardware
* **Placa**: STM32 NUCLEO-F4 (STM32F446RE).
* **Pulsador**: Botón de usuario integrado (BUTTON_Pin / PC13).
* **LED**: LED de usuario integrado (LED_Pin / PA5).

