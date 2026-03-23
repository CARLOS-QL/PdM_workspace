# Práctica 3 - Modularización de una librería de retardos no bloqueantes

## Descripción

En esta práctica se implementa un módulo de software para gestionar retardos no bloqueantes, utilizando la función HAL_GetTick() del framework STM32.

El desarrollo se basa en la encapsulación de funciones en una API propia (`API_delay`), promoviendo modularidad, reutilización y buenas prácticas de programación en C.

---

## Objetivo

Implementar un sistema de retardos no bloqueantes a partir de las funciones desarrolladas en la práctica 2, permitiendo la ejecución concurrente de tareas sin detener el flujo del programa.

---

## Estructura del proyecto

El repositorio contiene:

- `Drivers/API/Inc/API_delay.h` → Declaración de funciones públicas
- `Drivers/API/Src/API_delay.c` → Implementación del módulo
- `Core/Src/main.c` → Programa principal
- `README.md` → Documentación del proyecto

Se creó la carpeta `API` dentro de `Drivers`, con subcarpetas `Inc` y `Src`, siguiendo una arquitectura modular.

---

## Definiciones utilizadas

```c
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;
