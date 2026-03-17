\# Práctica 2 – Retardos no bloqueantes en STM32



En esta práctica se implementa un módulo de software para manejar retardos no bloqueantes utilizando el contador de tiempo del sistema HAL\_GetTick(), y se aplica para controlar el parpadeo de un LED con distintos períodos 1000ms, 200ms, 100ms.





Cada patrón se ejecuta durante 10 cambios de estado (encendido/apagado), luego se pasa al siguiente.

El módulo se basa en una estructura (delay\_t) que permite manejar el estado del retardo.



Se definen los tipos y la estructura en el archivo main.h



\- tick\_t: basado en uint32\_t (requiere <stdint.h>)

\- bool\_t: tipo booleano (requiere <stdbool.h>)



typedef struct {

&#x20;   tick\_t startTime;

&#x20;   tick\_t duration;

&#x20;   bool\_t running;

} delay\_t;



La implementación de las funciones se ubican en el archivo main.c

