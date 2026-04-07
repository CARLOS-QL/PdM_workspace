# Práctica 5: Implementación de UART y Parser de Comandos con MEF

## Autor: Carlos Quispe

## Descripción

Este proyecto implementa una interfaz de comunicación serie utilizando UART en modo polling, junto con un parser de comandos basado en una Máquina de Estados Finitos (MEF).
Se desarrolla una arquitectura modular mediante controladores (drivers) desacoplados, permitiendo la interpretación de comandos recibidos por UART y la ejecución de acciones sobre el hardware (LED).



## 🎯 Objetivo
Implementar:

* Un módulo UART para transmisión y recepción por polling
* Un parser de comandos mediante una MEF
* Control del LED mediante comandos seriales

Todo en un mismo proyecto, integrando los puntos 1 y 2 de la práctica.

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

