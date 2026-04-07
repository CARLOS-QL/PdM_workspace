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
---

## 📂 Estructura del proyecto
Siguiendo las buenas prácticas de encapsulamiento y modularización, el proyecto se organiza en:

* API_uart: Manejo de comunicación UART
* API_cmdparser: Parser de comandos mediante MEF
* API_led: Envoltorio (wrapper) para el control del LED
* API_delay: Manejo de tiempos no bloqueantes (HAL_GetTick)
---

##  Modificaciones Implementadas

✔ UART mejorada

La función de inicialización ahora recibe el baudrate:
```c
bool_t uartInit(uint32_t baudRate);
```
Esto permite mayor flexibilidad en la configuración de la comunicación.

------------------------------------------------------------

✔ Funciones UART con retorno de estado

Se modificaron las siguientes funciones para retornar bool_t, permitiendo validar si la operación fue exitosa:

```c
bool_t uartInit(uint32_t baudRate);
bool_t uartSendString(uint8_t * pstring);
bool_t uartSendStringSize(uint8_t * pstring, uint16_t size);
bool_t uartReceiveStringSize(uint8_t * pstring, uint16_t size);
```

✔ Mejora la robustez del sistema

✔ Permite manejo de errores


✔ Uso de isCommandPending

Se implementó la variable:

```c
isCommandPending
```

Para permitir la transición a los estados:

- CMD_PROCESS
- CMD_EXEC
- CMD_ERROR

## 🔄 Funcionamiento de la MEF

La MEF del parser permite:

* Recepción de caracteres por UART
* Construcción de líneas completas
* Validación de comandos
* Ejecución de acciones

Estados principales:

* CMD_IDLE
* CMD_RECEIVING
* CMD_PROCESS
* CMD_EXEC
* CMD_ERROR
* 
💡 Comandos soportados
* HELP
* LED ON
* LED OFF
* LED TOGGLE
* STATUS
* BAUD?
* BAUD=XXXX
  
🧪 Características del sistema
* Comunicación UART en polling (sin interrupciones)
* Validación de parámetros
* Manejo de errores
* Arquitectura modular
* Uso de variables static para encapsulamiento

## 🛠️ Requisitos de Hardware
* **Placa**: STM32 NUCLEO-F4 (STM32F446RE).
* **Pulsador**: Botón de usuario integrado (BUTTON_Pin / PC13).
* **LED**: LED de usuario integrado (LED_Pin / PA5).

